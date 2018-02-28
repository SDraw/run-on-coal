#include "stdafx.h"

#include "Lua/LuaDefs/LuaEventsDef.h"

#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaFunction.h"

void ROC::LuaEventsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "addEvent", Add);
    lua_register(f_vm, "addEventHandler", AddHandler);
    lua_register(f_vm, "removeEvent", Remove);
    lua_register(f_vm, "removeEventHandler", RemoveHandler);
    lua_register(f_vm, "callEvent", Call);
}

int ROC::LuaEventsDef::Add(lua_State *f_vm)
{
    // bool addEvent(str eventName)
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

int ROC::LuaEventsDef::AddHandler(lua_State *f_vm)
{
    // bool addEventHandler(str eventName, func callback)
    std::string l_event;
    LuaFunction l_func;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->AddEventHandler(l_event, l_func);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaEventsDef::Remove(lua_State *f_vm)
{
    // bool removeEvent(str eventName)
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
int ROC::LuaEventsDef::RemoveHandler(lua_State *f_vm)
{
    // bool removeEventHandler(str eventName, func callback)
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
int ROC::LuaEventsDef::Call(lua_State *f_vm)
{
    // bool callEvent(str eventName, var value1, ...)
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
