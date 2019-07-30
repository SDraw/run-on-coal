#pragma once

class EventHandler;
class LuaVM;
class TaskHandler;

class LuaModule : public ROC::IModule
{
    static LuaModule *ms_instance;

    ROC::ICore *m_core;
    EventHandler *m_eventHandler;
    LuaVM *m_luaVM;
    TaskHandler *m_taskHandler;

    explicit LuaModule(ROC::ICore *f_core);
    ~LuaModule();
public:
    static LuaModule* Init(ROC::ICore *f_core);
    inline static LuaModule* GetModule() { return ms_instance; }
    static void Terminate();

    inline ROC::ICore* GetEngineCore() const { return m_core; }
    inline LuaVM* GetLuaVM() const { return m_luaVM; }
    inline EventHandler* GetEventHandler() const { return m_eventHandler; }
    inline TaskHandler* GetTaskHandler() const { return m_taskHandler; }
protected:
    void RecieveGlobalEvent(unsigned char f_event, const CustomArguments &f_args);
    void DoPulse();
};

