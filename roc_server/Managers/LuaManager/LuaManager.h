#pragma once

#define ROC_LUA_METATABLE_USERDATA "roc_ud"

namespace ROC
{

class Core;
class EventManager;
class LuaFunction;
class CustomArguments;

class LuaManager final
{
    Core *m_core;

    lua_State *m_vm;
    EventManager *m_eventManager;

    LuaManager(const LuaManager &that) = delete;
    LuaManager& operator=(const LuaManager &that) = delete;
public:
    inline EventManager* GetEventManager() const { return m_eventManager; }

    bool LoadScript(const std::string &f_script, bool f_asFile = true);
protected:
    explicit LuaManager(Core *f_core);
    ~LuaManager();

    void LoadDefaultScripts();

    void DoPulse();
    void CallFunction(const LuaFunction &f_func, const CustomArguments &f_args);

    friend class Core;
    friend class EventManager;
};

}
