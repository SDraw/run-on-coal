#include "stdafx.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"

ROC::EventManager::EventManager(LuaManager *f_luaManager)
{
    m_luaManager = f_luaManager;

    m_lastIteration = 0;
    m_currentEvent = EventType::PreRender;
}
ROC::EventManager::~EventManager()
{
}

bool ROC::EventManager::AddEvent(unsigned char f_event,int f_ref,void *f_pointer)
{
    if(f_event >= EventType::Last) return false;
    auto &l_event = m_eventVector[f_event];
    for(auto iter : l_event)
    {
        if(iter.first == f_pointer) return false;
    }
    l_event.push_back(std::pair<void*,int>(f_pointer,f_ref));
    return true;
}
bool ROC::EventManager::RemoveEvent(unsigned char f_event,void *f_pointer)
{
    if(f_event >= EventType::Last) return false;
    auto &eventMap = m_eventVector[f_event];
    for(auto it = eventMap.begin(); it != eventMap.end(); it++)
    {
        if(it->first == f_pointer)
        {
            m_luaManager->RemoveReference(it->second);
            it = eventMap.erase(it);
            if(f_event == m_currentEvent) m_lastIteration--;
            break;
        }
    }
    return true;
}

void ROC::EventManager::CallEvent(unsigned char f_event,LuaArguments *f_args)
{
    if(f_event >= EventType::Last) return;
    m_currentEvent = f_event;
    m_lastIteration = 0;
    auto &l_event = m_eventVector[f_event];
    for(; m_lastIteration < l_event.size(); m_lastIteration++)
    {
        m_luaManager->CallFunction(l_event[m_lastIteration].second,f_args);
    }
}
