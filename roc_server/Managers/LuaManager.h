#pragma once

namespace ROC
{

class Core;
class EventManager;
class LuaArguments;
class LuaManager
{
    Core *m_core;
    static Core *m_coreStatic;

    lua_State *m_pVM;
    EventManager *m_eventManager;

    LuaManager(const LuaManager& that);
    LuaManager &operator =(const LuaManager &that);
public:
    static inline Core* GetCore() { return m_coreStatic; };
    bool OpenFile(std::string &f_path);
    inline EventManager* GetEventManager() { return m_eventManager; }
protected:
    explicit LuaManager(Core *f_core);
    ~LuaManager();

    void CallFunction(int f_func, LuaArguments *f_args);
    inline void RemoveReference(int f_ref) { luaL_unref(m_pVM, LUA_REGISTRYINDEX, f_ref); }
    friend Core;
    friend EventManager;
};

}
