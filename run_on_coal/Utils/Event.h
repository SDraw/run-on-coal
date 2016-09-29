#pragma once

namespace ROC
{

class Event
{
protected:
    void *m_luaFunc;
    int m_luaRef;
    bool m_muted;
    bool m_deleted;
    Event(void *f_func, int f_ref);
    ~Event();
    friend class EventManager;
};

}

