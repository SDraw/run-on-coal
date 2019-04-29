#pragma once
#include "Lua/LuaFunction.h"

class LuaModule;

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
        EHE_OnServerStart = 0U,
        EHE_OnServerStop,
        EHE_OnServerPulse,
        EHE_onNetworkClientConnect,
        EHE_onNetworkClientDisconnect,
        EHE_onNetworkDataRecieve
    };

    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, const LuaFunction &f_func);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, LuaFunction &f_func);

    void CallEvent(const std::string &f_event, const CustomArguments &f_args); // Only custom events
protected:
    explicit EventHandler(LuaModule *f_module);
    ~EventHandler();

    void CallEvent(EventHandlerEvent f_event, const CustomArguments &f_args); // Only default events

    friend class LuaModule;
};
