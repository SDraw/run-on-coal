#pragma once
#include "Lua/LuaFunction.h"

class LuaModule;
class LuaArgument;

class EventHandler
{
    LuaModule *m_luaModule;

    struct Event
    {
        LuaFunction m_function;
        bool m_deleted = false;
    };
    struct EventHeap
    {
        bool m_active = false;
        bool m_deleted = false;
        std::vector<Event> m_eventVector;
        std::vector<Event>::iterator m_eventVectorIter;
    };
    std::unordered_map<std::string, EventHeap*> m_eventMap;
    std::unordered_map<std::string, EventHeap*>::iterator m_eventMapEnd;
public:
    enum EventHandlerEvent : size_t
    {
        EHE_onEngineStart = 0U,
        EHE_onEngineStop,
        EHE_onRender,
        EHE_onVRRender,
        EHE_onPreRender,
        EHE_onWindowClose,
        EHE_onWindowResize,
        EHE_onWindowFocus,
        EHE_onKeyPress,
        EHE_onMouseKeyPress,
        EHE_onMouseScroll,
        EHE_onCursorMove,
        EHE_onCursorEnter,
        EHE_onJoypadStateChange,
        EHE_onJoypadButton,
        EHE_onJoypadAxis,
        EHE_onVRControllerKeyPress,
        EHE_onVRControllerKeyTouch,
        EHE_onVRControllerAxis,
        EHE_onTextInput,
        EHE_onNetworkStateChange,
        EHE_onNetworkDataRecieve
    };

    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, const LuaFunction &f_func);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, LuaFunction &f_func);

    void CallEvent(const std::string &f_event, const std::vector<LuaArgument> &f_args); // Only custom events
protected:
    explicit EventHandler(LuaModule *f_module);
    ~EventHandler();

    void CallEvent(EventHandlerEvent f_event, const ROC::ICustomArguments *f_args); // Only default events

    friend class LuaModule;
};
