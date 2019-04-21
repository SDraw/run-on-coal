#include "stdafx.h"

#include "Managers/AsyncManager/AsyncManager.h"
#include "Managers/AsyncManager/AsyncTask.h"
#include "Utils/CustomArguments.h"

#include "Core/Core.h"
#include "Managers/AsyncManager/AsyncGeometryTask.h"
#include "Managers/AsyncManager/AsyncTextureTask.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/EventManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Lua/LuaFunction.h"

ROC::AsyncManager::AsyncManager(Core *f_core)
{
    m_core = f_core;

    m_threadSwitch = true;
    m_loadThread = new std::thread(&ROC::AsyncManager::ExecutionThread, this);

    m_asyncTaskCallback = nullptr;
}
ROC::AsyncManager::~AsyncManager()
{
    m_threadSwitch = false;
    m_loadThread->join();

    for(auto iter : m_preparedTasks) delete iter;
    m_preparedTasks.clear();

    for(auto iter : m_executedTasks) delete iter;
    m_executedTasks.clear();
}

void* ROC::AsyncManager::LoadGeometry(const std::string &f_path)
{
    AsyncTask *l_task = new AsyncGeometryTask(f_path);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}
void* ROC::AsyncManager::LoadGeometry(const std::string &f_path, const LuaFunction &f_callback)
{
    AsyncTask *l_task = new AsyncGeometryTask(f_path);
    l_task->SetLuaCallback(f_callback);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}

void* ROC::AsyncManager::LoadTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress, const LuaFunction &f_callback)
{
    AsyncTask *l_task = new AsyncTextureTask(f_path, f_type, f_filter, f_compress);
    l_task->SetLuaCallback(f_callback);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}
void* ROC::AsyncManager::LoadTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress, const LuaFunction &f_callback)
{
    AsyncTask *l_task = new AsyncTextureTask(f_path, f_filter, f_compress);
    l_task->SetLuaCallback(f_callback);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}
void* ROC::AsyncManager::LoadTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress)
{
    AsyncTask *l_task = new AsyncTextureTask(f_path, f_type, f_filter, f_compress);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}
void* ROC::AsyncManager::LoadTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress)
{
    AsyncTask *l_task = new AsyncTextureTask(f_path, f_filter, f_compress);

    m_preparedTasksMutex.lock();
    m_preparedTasks.push_back(l_task);
    m_preparedTasksMutex.unlock();

    return l_task;
}

void ROC::AsyncManager::ExecutionThread()
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

                Element *l_element = reinterpret_cast<Element*>(iter->GetElement());
                if(l_element) m_core->GetElementManager()->AddElementToSet(l_element);

                if(m_asyncTaskCallback) (*m_asyncTaskCallback)(iter, l_element);

                const LuaFunction &l_luaFuncion = iter->GetLuaCallback();
                if(l_luaFuncion.IsValid())
                {
                    m_luaArguments.Push(reinterpret_cast<void*>(iter));
                    l_element ? m_luaArguments.Push(l_element) : m_luaArguments.Push(false);
                    m_core->GetLuaManager()->CallFunction(l_luaFuncion, m_luaArguments);
                    m_luaArguments.Clear();
                }

                delete iter;
            }
            m_executedTasks.clear();
        }
        m_executedTasksMutex.unlock();
    }
}
