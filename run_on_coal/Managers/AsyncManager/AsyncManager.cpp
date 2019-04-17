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

    for(auto iter : m_q0) delete iter;
    m_q0.clear();

    for(auto iter : m_q2) delete iter;
    m_q2.clear();

    delete m_luaArguments;
}

void ROC::AsyncManager::AddGeometryLoad(Geometry *f_geometry, const std::string &f_path)
{
    AsyncGeometryTask *l_task = new AsyncGeometryTask(f_geometry, f_path);

    m_q0Mutex.lock();
    m_q0.push_back(l_task);
    m_q0Mutex.unlock();
}

void ROC::AsyncManager::LoadThread()
{
    sf::Context l_context;
    l_context.setActive(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::chrono::milliseconds l_sleepTime(10U);
    while(m_threadSwitch)
    {
        if(m_q0Mutex.try_lock())
        {
            m_q0.swap(m_q1);
            m_q0Mutex.unlock();
        }
        if(!m_q1.empty())
        {
            for(auto iter : m_q1) iter->Execute();

            m_q2Mutex.lock();
            m_q1.swap(m_q2);
            m_q2Mutex.unlock();
        }
        std::this_thread::sleep_for(l_sleepTime);
    }
}

void ROC::AsyncManager::DoPulse()
{
    if(m_q2Mutex.try_lock())
    {
        if(!m_q2.empty())
        {
            for(auto iter : m_q2)
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
            m_q2.clear();
        }
        m_q2Mutex.unlock();
    }
}
