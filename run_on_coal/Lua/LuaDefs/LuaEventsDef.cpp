#include "stdafx.h"

#include "Lua/LuaDefs/LuaEventsDef.h"

#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaFunction.hpp"

void ROC::LuaEventsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "addEvent", AddEvent);
    lua_register(f_vm, "addEventHandler", AddEventHandler);
    lua_register(f_vm, "removeEvent", RemoveEvent);
    lua_register(f_vm, "removeEventHandler", RemoveEventHandler);
    lua_register(f_vm, "callEvent", CallEvent);
}

int ROC::LuaEventsDef::AddEvent(lua_State *f_vm)
{
    std::string l_event;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->AddEvent(l_event);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaEventsDef::AddEventHandler(lua_State *f_vm)
{
    std::string l_event;
    LuaFunction l_func;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func,true);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->AddEventHandler(l_event, l_func);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    argStream.RemoveReference(l_func);
    return argStream.GetReturnValue();
}
int ROC::LuaEventsDef::RemoveEvent(lua_State *f_vm)
{
    std::string l_event;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->RemoveEvent(l_event);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaEventsDef::RemoveEventHandler(lua_State *f_vm)
{
    std::string l_event;
    LuaFunction l_func;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->RemoveEventHandler(l_event,l_func);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaEventsDef::CallEvent(lua_State *f_vm)
{
    std::string l_event;
    ArgReader argStream(f_vm);
    LuaArguments l_arguments;
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        argStream.ReadArguments(l_arguments);
        LuaManager::GetCore()->GetLuaManager()->GetEventManager()->CallEvent(l_event, &l_arguments);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
