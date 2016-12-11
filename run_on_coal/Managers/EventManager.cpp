#include "stdafx.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"

ROC::EventManager::EventManager(LuaManager *f_luaManager)
{
    m_luaManager = f_luaManager;
    m_activeEvent = EventType::Last;
}
ROC::EventManager::~EventManager()
{
    for(auto &iter : m_eventsVector)
    {
        for(auto iter1 : iter) delete iter1;
        iter.clear();
    }
}

bool ROC::EventManager::AddEvent(unsigned char f_event, int f_ref, void *f_pointer)
{
    if(f_event >= EventType::Last) return false;
    auto &l_event = m_eventsVector[f_event];
    for(auto iter : l_event)
    {
        if(iter->m_luaFunc == f_pointer)
        {
            bool l_result = false;
            if(iter->m_deleted)
            {
                iter->m_deleted = false;
                l_result = true;
            }
            return l_result;
        }
    }
    Event *l_eventObj = new Event();
    l_eventObj->m_luaFunc = f_pointer;
    l_eventObj->m_luaRef = f_ref;
    if(f_event == m_activeEvent) m_iter = l_event.insert(m_iter, l_eventObj)+1;
    else l_event.push_back(l_eventObj);
    return true;
}
bool ROC::EventManager::SetEventMute(unsigned char f_event, void *f_pointer, bool f_mute)
{
    if(f_event >= EventType::Last) return false;
    bool l_result = false;
    auto &l_event = m_eventsVector[f_event];
    for(auto it = l_event.begin(); it != l_event.end(); it++)
    {
        Event *l_eventObj = (*it);
        if(l_eventObj->m_luaFunc == f_pointer)
        {
            l_eventObj->m_muted = f_mute;
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::EventManager::RemoveEvent(unsigned char f_event, void *f_pointer)
{
    if(f_event >= EventType::Last) return false;
    bool l_result = false;
    auto &l_event = m_eventsVector[f_event];
    for(auto it = l_event.begin(); it != l_event.end(); it++)
    {
        Event *l_eventObj = (*it);
        if(l_eventObj->m_luaFunc == f_pointer)
        {
            l_eventObj->m_deleted = true;
            l_result = true;
            break;
        }
    }
    return l_result;
}

void ROC::EventManager::CallEvent(unsigned char f_event, LuaArguments *f_args)
{
    if(f_event >= EventType::Last) return;
    m_activeEvent = f_event;
    auto &l_event = m_eventsVector[f_event];
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
}
