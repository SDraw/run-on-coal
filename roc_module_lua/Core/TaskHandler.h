#pragma once
#include "Lua/LuaFunction.h"

class Core;

class TaskHandler final
{
    Core* m_luaModule;

    std::map<void*, LuaFunction> m_taskMap;
public:
    bool IsTask(void *f_task);

    void* CreateGeometryTask(const std::string &f_path, const LuaFunction &f_func);
    void* CreateTextureTask(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress, const LuaFunction &f_func);
    void* CreateTextureTask(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress, const LuaFunction &f_func);
protected:
    explicit TaskHandler(Core *f_module);
    ~TaskHandler();

    void ProcessTask(const ROC::ICustomArguments *f_args);

    friend Core;


};
