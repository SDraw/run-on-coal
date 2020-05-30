#include "stdafx.h"

#include "Lua/LuaDefs/LuaEventsDef.h"

#include "Module/LuaModule.h"
#include "Module/EventHandler.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArgument.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->AddEvent(l_event);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaEventsDef::AddEventHandler(lua_State *f_vm)
{
    // bool addEventHandler(str eventName, func callback)
    std::string l_event;
    LuaFunction l_func;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->AddEventHandler(l_event, l_func);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaEventsDef::RemoveEvent(lua_State *f_vm)
{
    // bool removeEvent(str eventName)
    std::string l_event;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->RemoveEvent(l_event);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaEventsDef::RemoveEventHandler(lua_State *f_vm)
{
    // bool removeEventHandler(str eventName, func callback)
    std::string l_event;
    LuaFunction l_func;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEventHandler()->RemoveEventHandler(l_event, l_func);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaEventsDef::CallEvent(lua_State *f_vm)
{
    // bool callEvent(str eventName, var value1, ...)
    std::string l_event;
    std::vector<LuaArgument> l_arguments;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        argStream.ReadArguments(l_arguments);
        LuaModule::GetModule()->GetEventHandler()->CallEvent(l_event, l_arguments);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
