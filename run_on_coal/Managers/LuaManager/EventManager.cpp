#include "stdafx.h"

#include "Managers/LuaManager/EventManager.h"

#include "Managers/LuaManager/LuaManager.h"
#include "Lua/LuaFunction.h"
#include "Utils/CustomArguments.h"
#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_DefaultEventsNames
{
    "onEngineStart", "onEngineStop",
    "onRender", "onVRRender", "onPreRender",
    "onWindowClose", "onWindowResize", "onWindowFocus",
    "onKeyPress", "onMouseKeyPress", "onMouseScroll", "onCursorMove", "onCursorEnter",
    "onJoypadStateChange", "onJoypadButton", "onJoypadAxis",
    "onVRControllerKeyPress", "onVRControllerKeyTouch", "onVRControllerAxis",
    "onTextInput",
    "onNetworkStateChange", "onNetworkDataRecieve"
};

}

#define ROC_EVENT_MISSING 0U
#define ROC_EVENT_DELETED 1U
#define ROC_EVENT_EXISTS 2U

ROC::EventManager::EventManager(LuaManager *f_luaManager)
{
    m_luaManager = f_luaManager;

    for(auto &iter : g_DefaultEventsNames) m_eventMap.insert(std::make_pair(iter, new EventHeap()));
    m_eventMapEnd = m_eventMap.end();
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
    EventHeap *l_eventHeap = nullptr;
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
bool ROC::EventManager::AddEventHandler(const std::string &f_event, const LuaFunction &f_func)
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
                if(l_event.m_function == f_func)
                {
                    if(!l_event.m_deleted) l_check = ROC_EVENT_EXISTS;
                    else
                    {
                        l_event.m_deleted = false;
                        l_check = ROC_EVENT_DELETED;
                    }
                    break;
                }
            }
            if(l_check == ROC_EVENT_MISSING)
            {
                Event l_eventObj;
                l_eventObj.m_function = f_func;

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
    if(EnumUtils::ReadEnumVector(f_event, g_DefaultEventsNames) == -1)
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
bool ROC::EventManager::RemoveEventHandler(const std::string &f_event, LuaFunction &f_func)
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
                if((l_event.m_function == f_func) && !l_event.m_deleted)
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

void ROC::EventManager::CallEvent(EventManagerEvent f_event, const CustomArguments &f_args)
{
    auto iter = m_eventMap.find(g_DefaultEventsNames[f_event]);
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
                    if(!l_heapIter->m_deleted) m_luaManager->CallFunction(l_heapIter->m_function, f_args);
                    if(l_heapIter->m_deleted)
                    {
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

void ROC::EventManager::CallEvent(const std::string &f_event, const CustomArguments &f_args)
{
    if(std::find(g_DefaultEventsNames.begin(), g_DefaultEventsNames.end(), f_event) == g_DefaultEventsNames.end())
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
                        if(!l_heapIter->m_deleted) m_luaManager->CallFunction(l_heapIter->m_function, f_args);
                        if(l_heapIter->m_deleted)
                        {
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
}
