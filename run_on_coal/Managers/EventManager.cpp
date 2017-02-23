#include "stdafx.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"

ROC::EventManager::EventManager(LuaManager *f_luaManager)
{
    m_luaManager = f_luaManager;
    m_activeEvent = EventType::None;
    m_locked = false;
}
ROC::EventManager::~EventManager()
{
    for(auto &iter : m_eventsVector)
    {
        for(auto iter1 : iter) delete iter1;
        iter.clear();
    }
}

bool ROC::EventManager::AddEvent(EventType f_event, int f_ref, void *f_pointer)
{
    bool l_result = false;
    if(f_event != EventType::None)
    {
        auto &l_event = m_eventsVector[f_event];
        for(auto iter : l_event)
        {
            if(iter->m_luaFunc == f_pointer)
            {
                if(iter->m_deleted)
                {
                    iter->m_deleted = false;
                    l_result = true;
                }
                break;
            }
        }
        if(!l_result)
        {
            Event *l_eventObj = new Event();
            l_eventObj->m_luaFunc = f_pointer;
            l_eventObj->m_luaRef = f_ref;
            if(f_event == m_activeEvent) m_iter = l_event.insert(m_iter, l_eventObj) + 1;
            else l_event.push_back(l_eventObj);
            l_result = true;
        }
    }
    return l_result;
}
bool ROC::EventManager::SetEventMute(EventType f_event, void *f_pointer, bool f_mute)
{
    bool l_result = false;
    if(f_event != EventType::None)
    {
        auto &l_event = m_eventsVector[f_event];
        for(auto iter = l_event.begin(), iterEnd = l_event.end(); iter != iterEnd; ++iter)
        {
            Event *l_eventObj = (*iter);
            if(l_eventObj->m_luaFunc == f_pointer)
            {
                l_eventObj->m_muted = f_mute;
                l_result = true;
                break;
            }
        }
    }
    return l_result;
}
bool ROC::EventManager::RemoveEvent(EventType f_event, void *f_pointer)
{
    bool l_result = false;
    if(f_event != EventType::None)
    {
        auto &l_event = m_eventsVector[f_event];
        for(auto iter = l_event.begin(), iterEnd = l_event.end(); iter != iterEnd; ++iter)
        {
            Event *l_eventObj = (*iter);
            if(l_eventObj->m_luaFunc == f_pointer)
            {
                l_eventObj->m_deleted = true;
                l_result = true;
                break;
            }
        }
    }
    return l_result;
}

void ROC::EventManager::CallEvent(EventType f_event, LuaArguments *f_args)
{
    if(f_event != EventType::None)
    {
        if(!m_locked)
        {
            m_locked = true;
            m_activeEvent = f_event;
            auto &l_event = m_eventsVector[f_event];
            auto l_iter = l_event.begin();
            for(m_iter = l_event.begin(); m_iter != l_event.end(); ++m_iter)
            {
                Event *l_eventObj = (*m_iter);
                if(!l_eventObj->m_muted && !l_eventObj->m_deleted) m_luaManager->CallFunction(l_eventObj->m_luaRef, f_args);
                if(l_eventObj->m_deleted)
                {
                    m_luaManager->RemoveReference(l_eventObj->m_luaRef);
                    delete l_eventObj;
                    m_iter = l_event.erase(m_iter);
                    if(m_iter == l_event.end()) break;
                }
            }
            m_locked = false;
        }
    }
}
