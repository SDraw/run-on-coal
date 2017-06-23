#pragma once
#include "Lua/LuaFunction.hpp"

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
public:
    bool AddEvent(const std::string &f_event);
    bool AddEventHandler(const std::string &f_event, const LuaFunction &f_func);

    bool RemoveEvent(const std::string &f_event);
    bool RemoveEventHandler(const std::string &f_event, const LuaFunction &f_func);

    void CallEvent(const std::string &f_event, LuaArguments *f_args);
protected:
    explicit EventManager(LuaManager *f_luaManager);
    ~EventManager();

    friend class LuaManager;
};

}
