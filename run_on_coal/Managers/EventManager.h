#pragma once

namespace ROC
{

class LuaManager;
class LuaArguments;
class EventManager
{
    LuaManager *m_luaManager;

    struct Event
    {
        void *m_luaFunc = nullptr;
        int m_luaRef = -1;
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
    bool m_locked;
public:
    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, int f_ref, void *f_pointer);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, void *f_pointer);

    void CallEvent(const std::string &f_event, LuaArguments *f_args);
protected:
    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    friend LuaManager;
};

}
