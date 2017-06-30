#include "stdafx.h"

#include "Managers/AsyncManager.h"
#include "Elements/Geometry/Geometry.h"
#include "Lua/LuaArguments.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"

ROC::AsyncManager::AsyncManager(Core *f_core)
{
    m_core = f_core;

    m_threadSwitch = true;
    m_loadThread = new std::thread(&ROC::AsyncManager::LoadThread, this);

    m_argument = new LuaArguments();
    m_callback = nullptr;
}
ROC::AsyncManager::~AsyncManager()
{
    m_threadSwitch = false;
    m_loadThread->join();

    m_loadedQueue.clear();

    delete m_argument;
}

void ROC::AsyncManager::AddGeometryToQueue(Geometry *f_geometry, const std::string &f_path)
{
    amLoadQuery l_query;
    l_query.m_geometry = f_geometry;
    l_query.m_path.assign(f_path);

    m_loadingMutex.lock();
    m_loadingQueue.push_back(l_query);
    m_loadingMutex.unlock();
}

void ROC::AsyncManager::LoadThread()
{
    sf::Context l_context;
    l_context.setActive(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::chrono::milliseconds l_sleepTime(10U);
    std::vector<amLoadQuery> l_tempQueue;
    while(m_threadSwitch)
    {
        if(m_loadingMutex.try_lock())
        {
            m_loadingQueue.swap(l_tempQueue);
            m_loadingMutex.unlock();
        }
        if(!l_tempQueue.empty())
        {
            for(auto &iter : l_tempQueue)
            {
                iter.m_result = iter.m_geometry->Load(iter.m_path);
                glFinish();
            }

            m_loadedMutex.lock();
            l_tempQueue.swap(m_loadedQueue);
            m_loadedMutex.unlock();
        }
        std::this_thread::sleep_for(l_sleepTime);
    }
}

void ROC::AsyncManager::DoPulse()
{
    if(m_loadedMutex.try_lock())
    {
        if(!m_loadedQueue.empty())
        {
            for(auto &iter : m_loadedQueue)
            {
                if(iter.m_result) iter.m_geometry->GenerateVAOs();

                if(m_callback) (*m_callback)(iter.m_geometry, iter.m_result);

                m_argument->PushArgument(iter.m_geometry, "Geometry");
                m_argument->PushArgument(iter.m_result);
                m_core->GetLuaManager()->GetEventManager()->CallEvent("onGeometryLoad", m_argument);
                m_argument->Clear();

                if(!iter.m_result) m_core->GetElementManager()->DestroyElement(iter.m_geometry);
            }
            m_loadedQueue.clear();
        }
        m_loadedMutex.unlock();
    }
}
