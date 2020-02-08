#pragma once

class CustomArguments;
class LuaModule;
class LuaFunction;
class LuaArgument;

class LuaVM final
{
    LuaModule *m_luaModule;

    lua_State *m_vm;
public:
    static const char *ms_userdataMetatableName;
protected:
    explicit LuaVM(LuaModule *f_module);
    ~LuaVM();

    void LoadScript(const std::string &f_file);

    void DoPulse();

    void CallFunction(const LuaFunction &f_func, const ROC::ICustomArguments *f_args);
    void CallFunction(const LuaFunction &f_func, const std::vector<LuaArgument> &f_args);

    friend class LuaModule;
    friend class EventHandler;
    friend class TaskHandler;
};
