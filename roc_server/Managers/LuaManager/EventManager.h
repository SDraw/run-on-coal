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
    std::unordered_map<std::string,EventHeap*> m_eventMap;
    std::unordered_map<std::string, EventHeap*>::iterator m_eventMapEnd;

    EventManager(const EventManager &that) = delete;
    EventManager& operator=(const EventManager &that) = delete;
public:
    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, LuaFunction &f_func);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, const LuaFunction &f_func);

    void CallEvent(const std::string &f_event, const LuaArguments *f_args);
protected:
    enum EventManagerEvent : size_t
    {
        EME_onServerStart = 0U,
        EME_onServerStop,
        EME_onServerPulse,
        EME_onNetworkClientConnect,
        EME_onNetworkClientDisconnect,
        EME_onNetworkDataRecieve
    };

    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    void CallEvent(EventManagerEvent f_event, const LuaArguments *f_args);

    friend class Core;
    friend class LuaManager;
    friend class NetworkManager;
};

}
