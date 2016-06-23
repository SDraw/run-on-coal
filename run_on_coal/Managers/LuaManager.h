#pragma once

namespace ROC
{

class Core;
class EventManager;
class LuaArguments;
class LuaManager
{
    Core *m_core;

    lua_State *m_pVM;
    EventManager *m_eventManager;
public:
    static Core *m_corePointer;
    bool OpenFile(std::string &f_path);
    EventManager* GetEventManager();
protected:
    LuaManager(Core *f_core);
    ~LuaManager();
    void CallFunction(int f_func,LuaArguments &f_args);
    void RemoveReference(int f_ref);
    friend Core;
    friend EventManager;
};

}
