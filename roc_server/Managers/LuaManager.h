#pragma once
#include "Lua/LuaFunction.hpp"

namespace ROC
{

class Core;
class EventManager;
class LuaArguments;
class LuaManager final
{
    Core *m_core;
    static Core *m_coreStatic;

    lua_State *m_vm;
    EventManager *m_eventManager;

    LuaManager(const LuaManager& that);
    LuaManager &operator =(const LuaManager &that);
public:
    static inline Core* GetCore() { return m_coreStatic; }
    bool OpenFile(const std::string &f_path);
    inline EventManager* GetEventManager() { return m_eventManager; }
protected:
    explicit LuaManager(Core *f_core);
    ~LuaManager();

    void CallFunction(const LuaFunction &f_func, LuaArguments *f_args);
    inline void RemoveReference(const LuaFunction &f_func) { luaL_unref(m_vm, LUA_REGISTRYINDEX, f_func.m_reference); }

    friend class Core;
    friend class EventManager;
};

}
