#pragma once

namespace ROC
{

enum EventType
{
    None = -1,
    ServerStart, //onServerStart
    ServerStop, //onServerStop
    ServerPulse, //onServerPulse
    NetworkClientConnect, //onNetworkClientConnect
    NetworkClientDisconnect, //onNetworkClientDisconnect
    NetworkDataRecieve //onNetworkDataRecieve
};

class LuaManager;
class LuaArguments;
class EventManager
{
    LuaManager *m_luaManager;

    struct Event
    {
        void *m_luaFunc;
        int m_luaRef;
        bool m_muted = false;
        bool m_deleted = false;
    };
    std::vector<Event*> m_eventsVector[EventType::NetworkDataRecieve+1];
    std::vector<Event*>::iterator m_iter;
    EventType m_activeEvent;
    bool m_locked;
public:
    bool AddEvent(EventType f_event, int f_ref, void *f_pointer);
    bool SetEventMute(EventType f_event, void *f_pointer, bool f_mute);
    bool RemoveEvent(EventType f_event, void *f_pointer);
    void CallEvent(EventType f_event, LuaArguments *f_args);
protected:
    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    friend LuaManager;
};

}
