#include "stdafx.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"
#include "Utils/Utils.h"

#define ROC_EVENT_MISSING 0U
#define ROC_EVENT_DELETED 1U
#define ROC_EVENT_EXISTS 2U

const std::vector<std::string> g_DefaultEventsNames
{
    "onServerStart", "onServerStop", "onServerPulse",
    "onNetworkClientConnect", "onNetworkClientDisconnect", "onNetworkDataRecieve"
};

ROC::EventManager::EventManager(LuaManager *f_luaManager)
{
    m_luaManager = f_luaManager;

    for(auto &iter : g_DefaultEventsNames) m_eventMap.insert(std::make_pair(iter, new EventHeap()));
    m_eventMapEnd = m_eventMap.end();

    m_locked = false;
}
ROC::EventManager::~EventManager()
{
    for(auto iter : m_eventMap) delete iter.second;
    m_eventMap.clear();
}

bool ROC::EventManager::AddEvent(const std::string &f_event)
{
    bool l_result = false;
    auto iter = m_eventMap.find(f_event);
    EventHeap *l_eventHeap = NULL;
    if(iter == m_eventMapEnd)
    {
        l_eventHeap = new EventHeap();
        m_eventMap.insert(std::make_pair(f_event, l_eventHeap));
        m_eventMapEnd = m_eventMap.end();
        l_result = true;
    }
    else
    {
        l_eventHeap = iter->second;
        if(l_eventHeap->m_deleted)
        {
            l_eventHeap->m_deleted = false;
            l_result = true;
        }
    }
    return l_result;
}
bool ROC::EventManager::AddEventHandler(const std::string &f_event, int f_ref, void *f_pointer)
{
    bool l_result = false;
    auto iter = m_eventMap.find(f_event);
    if(iter != m_eventMapEnd)
    {
        EventHeap *l_heap = iter->second;
        if(!l_heap->m_deleted)
        {
            auto &l_eventVector = l_heap->m_eventVector;
            unsigned char l_check = ROC_EVENT_MISSING;
            for(auto &l_event : l_eventVector)
            {
                if(l_event.m_luaFunc == f_pointer)
                {
                    if(!l_event.m_deleted) l_check = ROC_EVENT_EXISTS;
                    else
                    {
                        l_event.m_deleted = false;
                        l_check = ROC_EVENT_DELETED;
                    }
                }
            }
            if(l_check == ROC_EVENT_MISSING)
            {
                Event l_eventObj;
                l_eventObj.m_luaFunc = f_pointer;
                l_eventObj.m_luaRef = f_ref;

                if(l_heap->m_active) l_heap->m_eventVectorIter = l_eventVector.insert(l_heap->m_eventVectorIter, l_eventObj) + 1;
                else l_eventVector.push_back(l_eventObj);
            }
            l_result = (l_check != ROC_EVENT_EXISTS);
        }
    }
    return l_result;
}

bool ROC::EventManager::RemoveEvent(const std::string &f_event)
{
    bool l_result = false;
    if(Utils::ReadEnumVector(g_DefaultEventsNames, f_event) == -1)
    {
        auto iter = m_eventMap.find(f_event);
        if(iter != m_eventMapEnd)
        {
            EventHeap *l_heap = iter->second;
            if(l_heap->m_active)
            {
                if(!l_heap->m_deleted)
                {
                    l_heap->m_deleted = true;
                    l_result = true;
                }
            }
            else
            {
                delete l_heap;
                m_eventMap.erase(iter);
                m_eventMapEnd = m_eventMap.end();
                l_result = true;
            }
        }
    }
    return l_result;
}
bool ROC::EventManager::RemoveEventHandler(const std::string &f_event, void *f_pointer)
{
    bool l_result = false;
    auto iter = m_eventMap.find(f_event);
    if(iter != m_eventMapEnd)
    {
        EventHeap *l_heap = iter->second;
        if(!l_heap->m_deleted)
        {
            auto &l_eventVector = l_heap->m_eventVector;
            for(auto &l_event : l_eventVector)
            {
                if(l_event.m_luaFunc == f_pointer && !l_event.m_deleted)
                {
                    l_event.m_deleted = true;
                    l_result = true;
                    break;
                }
            }
        }
    }
    return l_result;
}

void ROC::EventManager::CallEvent(const std::string &f_event, LuaArguments *f_args)
{
    auto iter = m_eventMap.find(f_event);
    if(iter != m_eventMapEnd)
    {
        EventHeap *l_heap = iter->second;
        if(!l_heap->m_active)
        {
            l_heap->m_active = true;
            if(!l_heap->m_deleted)
            {
                auto &l_eventVector = l_heap->m_eventVector;
                auto &l_heapIter = l_heap->m_eventVectorIter;
                for(l_heapIter = l_eventVector.begin(); l_heapIter != l_eventVector.end(); ++l_heapIter)
                {
                    if(!l_heapIter->m_deleted) m_luaManager->CallFunction(l_heapIter->m_luaRef, f_args);
                    if(l_heapIter->m_deleted)
                    {
                        m_luaManager->RemoveReference(l_heapIter->m_luaRef);
                        l_heapIter = l_eventVector.erase(l_heapIter);
                        if(l_heapIter == l_eventVector.end()) break;
                    }
                }
            }
            l_heap->m_active = false;
            if(l_heap->m_deleted)
            {
                delete l_heap;
                m_eventMap.erase(iter);
                m_eventMapEnd = m_eventMap.end();
            }
        }
    }
}
