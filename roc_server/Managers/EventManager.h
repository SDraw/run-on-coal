#pragma once

namespace ROC
{

enum EventType : unsigned char
{
    ServerStart = 0U, //onServerStart
    ServerStop, //onServerStop
    ServerPulse, //onServerPulse
    NetworkClientConnect, //onNetworkClientConnect
    NetworkClientDisconnect, //onNetworkClientConnect
    NetworkDataRecieve, //onNetworkDataRecieve
    Last // None
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
    std::vector<Event*> m_eventsVector[EventType::Last];
    std::vector<Event*>::iterator m_iter;
    unsigned char m_activeEvent;
    bool m_locked;
public:
    bool AddEvent(unsigned char f_event, int f_ref, void *f_pointer);
    bool SetEventMute(unsigned char f_event, void *f_pointer, bool f_mute);
    bool RemoveEvent(unsigned char f_event, void *f_pointer);
    void CallEvent(unsigned char f_event, LuaArguments *f_args);
protected:
    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    friend LuaManager;
};

}
