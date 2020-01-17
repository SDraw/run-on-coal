#pragma once

class EventHandler;
class LuaVM;

class LuaModule : public ROC::IModule
{
    static LuaModule *ms_instance;

    ROC::ICore *m_core;
    EventHandler *m_eventHandler;
    LuaVM *m_luaVM;

    explicit LuaModule(ROC::ICore *f_core);
    ~LuaModule();
public:
    static bool Init(ROC::ICore *f_core);
    inline static LuaModule* GetModule() { return ms_instance; }
    static void Terminate();

    inline ROC::ICore* GetEngineCore() const { return m_core; }
    inline LuaVM* GetLuaVM() const { return m_luaVM; }
    inline EventHandler* GetEventHandler() const { return m_eventHandler; }
protected:
    void RecieveGlobalEvent(unsigned char f_event, const ROC::ICustomArguments *f_args);
    void DoPulse();
};

