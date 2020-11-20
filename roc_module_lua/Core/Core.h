#pragma once

class EventHandler;
class LuaVM;
class TaskHandler;

class Core final : public ROC::IModule
{
    static Core *ms_instance;

    ROC::ICore *m_engineCore;
    EventHandler *m_eventHandler;
    LuaVM *m_luaVM;
    TaskHandler *m_taskHandler;

    explicit Core(ROC::ICore *f_core);
    ~Core();

    // ROC::IModule
    void RecieveGlobalEvent(unsigned char f_event, const ROC::ICustomArguments *f_args);
    void DoPulse();
public:
    static bool Init(ROC::ICore *f_core);
    static Core* GetInstance();
    static void Terminate();

    ROC::ICore* GetEngineICore() const;
    LuaVM* GetLuaVM() const;
    EventHandler* GetEventHandler() const;
    TaskHandler* GetTaskHandler() const;
};
