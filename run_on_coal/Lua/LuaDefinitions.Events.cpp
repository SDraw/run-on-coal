#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaFunction.hpp"
#include "Lua/LuaDefinitions.Events.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int addEvent(lua_State *f_vm)
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

int addEventHandler(lua_State *f_vm)
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
int removeEvent(lua_State *f_vm)
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
int removeEventHandler(lua_State *f_vm)
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
int callEvent(lua_State *f_vm)
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

}
}
