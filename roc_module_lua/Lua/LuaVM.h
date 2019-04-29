#pragma once

#define LM_METATABLE_USERDATA "lm_ud"

class LuaModule;
class LuaFunction;

class LuaVM
{
    LuaModule *m_luaModule;

    lua_State *m_vm;
protected:
    explicit LuaVM(LuaModule *f_module);
    ~LuaVM();

    void LoadScript(const std::string &f_file);

    void DoPulse();

    void CallFunction(const LuaFunction &f_func, const CustomArguments &f_args);

    friend class LuaModule;
    friend class EventHandler;
    friend class TaskHandler;
};
