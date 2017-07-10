#pragma once
#include "Lua/LuaFunction.hpp"

#define ROC_LUA_METATABLE_USERDATA "roc_ud"

namespace ROC
{

class Core;
class EventManager;
class LuaArguments;
class LuaManager final
{
    Core *m_core;
    static Core *s_core;

    lua_State *m_vm;
    EventManager *m_eventManager;

    LuaManager(const LuaManager& that);
    LuaManager &operator =(const LuaManager &that);
public:
    static inline Core* GetCore() { return s_core; }
    inline EventManager* GetEventManager() { return m_eventManager; }

    bool LoadScript(const std::string &f_script, bool f_asFile = true);
protected:
    explicit LuaManager(Core *f_core);
    ~LuaManager();

    static void SetCore(Core *f_core);

    void CallFunction(const LuaFunction &f_func, LuaArguments *f_args);
    inline void RemoveReference(const LuaFunction &f_func) { luaL_unref(m_vm, LUA_REGISTRYINDEX, f_func.m_ref); }

    friend class Core;
    friend class EventManager;
};

}
