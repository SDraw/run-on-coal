#include "stdafx.h"

#include "Lua/LuaDefs/LuaEventsDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaArgument.h"
#include "Module/LuaModule.h"
#include "Module/EventHandler.h"
#include "Utils/LuaUtils.h"

void LuaEventsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "addEvent", AddEvent);
    lua_register(f_vm, "addEventHandler", AddEventHandler);
    lua_register(f_vm, "removeEvent", RemoveEvent);
    lua_register(f_vm, "removeEventHandler", RemoveEventHandler);
    lua_register(f_vm, "callEvent", CallEvent);
}

int LuaEventsDef::AddEvent(lua_State *f_vm)
{
    // bool addEvent(str eventName)
    std::string l_event;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_event);
    if(!l_argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->AddEvent(l_event);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaEventsDef::AddEventHandler(lua_State *f_vm)
{
    // bool addEventHandler(str eventName, func callback)
    std::string l_event;
    LuaFunction l_func;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_event);
    l_argStream.ReadFunction(l_func);
    if(!l_argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->AddEventHandler(l_event, l_func);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaEventsDef::RemoveEvent(lua_State *f_vm)
{
    // bool removeEvent(str eventName)
    std::string l_event;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_event);
    if(!l_argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->RemoveEvent(l_event);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaEventsDef::RemoveEventHandler(lua_State *f_vm)
{
    // bool removeEventHandler(str eventName, func callback)
    std::string l_event;
    LuaFunction l_func;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_event);
    l_argStream.ReadFunction(l_func);
    if(!l_argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->RemoveEventHandler(l_event, l_func);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaEventsDef::CallEvent(lua_State *f_vm)
{
    // bool callEvent(str eventName, var value1, ...)
    std::string l_event;
    std::vector<LuaArgument> l_arguments;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_event);
    if(!l_argStream.HasErrors() && !l_event.empty())
    {
        l_argStream.ReadArguments(l_arguments);
        LuaModule::GetModule()->GetEventHandler()->CallEvent(l_event, l_arguments);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
