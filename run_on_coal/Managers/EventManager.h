#pragma once
#include "Lua/LuaFunction.h"

namespace ROC
{

class LuaManager;
class LuaArguments;

class EventManager final
{
    LuaManager *m_luaManager;

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

    EventManager(const EventManager &that);
    EventManager& operator=(const EventManager &that);
public:
    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, LuaFunction &f_func);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, LuaFunction &f_func);

    void CallEvent(const std::string &f_event, const LuaArguments *f_args); // Only custom events
protected:
    enum EventManagerEvent : size_t
    {
        EME_onEngineStart = 0U,
        EME_onEngineStop,
        EME_onRender,
        EME_onVRRender,
        EME_onPreRender,
        EME_onWindowClose,
        EME_onWindowResize,
        EME_onWindowFocus,
        EME_onKeyPress,
        EME_onMouseKeyPress,
        EME_onMouseScroll,
        EME_onCursorMove,
        EME_onCursorEnter,
        EME_onJoypadStateChange,
        EME_onJoypadButton,
        EME_onJoypadAxis,
        EME_onVRControllerKeyPress,
        EME_onVRControllerKeyTouch,
        EME_onVRControllerAxis,
        EME_onTextInput,
        EME_onNetworkStateChange,
        EME_onNetworkDataRecieve,
        EME_onGeometryLoad
    };

    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    void CallEvent(EventManagerEvent f_event, const LuaArguments *f_args); // Only default events

    friend class Core;
    friend class AsyncManager;
    friend class LuaManager;
    friend class NetworkManager;
    friend class PreRenderManager;
    friend class RenderManager;
    friend class SfmlManager;
    friend class VRManager;
};

}
