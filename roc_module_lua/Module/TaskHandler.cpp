#include "stdafx.h"

#include "TaskHandler.h"
#include "Module/LuaModule.h"

#include "Lua/LuaVM.h"

TaskHandler::TaskHandler(LuaModule *f_module)
{
    m_luaModule = f_module;
}
TaskHandler::~TaskHandler()
{
    m_taskMap.clear();
}

bool TaskHandler::IsTask(void *f_task)
{
    bool l_result = (m_taskMap.find(f_task) != m_taskMap.end());
    return l_result;
}

void* TaskHandler::CreateGeometryTask(const std::string &f_path, const LuaFunction &f_func)
{
    void* l_task = m_luaModule->GetEngineCore()->GetIAsyncManager()->LoadGeometry(f_path);
    m_taskMap.emplace(l_task, f_func);
    return l_task;
}
void* TaskHandler::CreateTextureTask(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress, const LuaFunction &f_func)
{
    void* l_task = m_luaModule->GetEngineCore()->GetIAsyncManager()->LoadTexture(f_path, f_type, f_filter, f_compress);
    m_taskMap.emplace(l_task, f_func);
    return l_task;
}
void* TaskHandler::CreateTextureTask(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress, const LuaFunction &f_func)
{
    void* l_task = m_luaModule->GetEngineCore()->GetIAsyncManager()->LoadTexture(f_path, f_filter, f_compress);
    m_taskMap.emplace(l_task, f_func);
    return l_task;
}

void TaskHandler::ProcessTask(const ROC::ICustomArguments *f_args)
{
    if(f_args->GetArgumentsCount() > 0U)
    {
        void *l_task = f_args->GetArgument(0U)->GetPointer();
        auto iter = m_taskMap.find(l_task);
        if(iter != m_taskMap.end())
        {
            m_luaModule->GetLuaVM()->CallFunction(iter->second, f_args);
            m_taskMap.erase(iter);
        }
    }
}
