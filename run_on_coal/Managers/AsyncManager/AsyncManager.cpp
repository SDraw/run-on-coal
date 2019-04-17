#include "stdafx.h"

#include "Managers/AsyncManager/AsyncManager.h"
#include "Managers/AsyncManager/AsyncTask.h"
#include "Managers/AsyncManager/AsyncGeometryTask.h"
#include "Lua/LuaArguments.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/EventManager.h"
#include "Managers/LuaManager/LuaManager.h"

ROC::AsyncManager::AsyncManager(Core *f_core)
{
    m_core = f_core;

    m_threadSwitch = true;
    m_loadThread = new std::thread(&ROC::AsyncManager::LoadThread, this);

    m_luaArguments = new LuaArguments();

    m_geometryCallback = nullptr;
}
ROC::AsyncManager::~AsyncManager()
{
    m_threadSwitch = false;
    m_loadThread->join();

    for(auto iter : m_preparedTasks) delete iter;
    m_preparedTasks.clear();

    for(auto iter : m_executedTasks) delete iter;
    m_executedTasks.clear();

    delete m_luaArguments;
}

void ROC::AsyncManager::AddGeometryLoad(Geometry *f_geometry, const std::string &f_path)
{
    AsyncGeometryTask *l_task = new AsyncGeometryTask(f_geometry, f_path);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();
}

void ROC::AsyncManager::LoadThread()
{
    sf::Context l_context;
    l_context.setActive(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::chrono::milliseconds l_sleepTime(10U);
    while(m_threadSwitch)
    {
        if(m_preparedTasksMutex.try_lock())
        {
            m_preparedTasks.swap(m_executionTasks);
            m_preparedTasksMutex.unlock();
        }
        if(!m_executionTasks.empty())
        {
            for(auto iter : m_executionTasks) iter->Execute();

            m_executedTasksMutex.lock();
            m_executionTasks.swap(m_executedTasks);
            m_executedTasksMutex.unlock();
        }
        std::this_thread::sleep_for(l_sleepTime);
    }
}

void ROC::AsyncManager::DoPulse()
{
    if(m_executedTasksMutex.try_lock())
    {
        if(!m_executedTasks.empty())
        {
            for(auto iter : m_executedTasks)
            {
                iter->PostExecute();

                bool l_result = (iter->GetResult() == AsyncTask::ATR_Success);
                switch(iter->GetType())
                {
                    case AsyncTask::ATT_Geometry:
                    {
                        if(m_geometryCallback) (*m_geometryCallback)(reinterpret_cast<Geometry*>(iter->GetElement()), l_result);
                    } break;
                }

                m_luaArguments->PushArgument(iter->GetElement());
                m_luaArguments->PushArgument(l_result);
                switch(iter->GetType())
                {
                    case AsyncTask::ATT_Geometry:
                        m_core->GetLuaManager()->GetEventManager()->CallEvent(EventManager::EME_onGeometryLoad, m_luaArguments);
                        break;
                }
                m_luaArguments->Clear();

                // Cleanup
                if(!l_result) m_core->GetElementManager()->DestroyElement(iter->GetElement());

                delete iter;
            }
            m_executedTasks.clear();
        }
        m_executedTasksMutex.unlock();
    }
}
