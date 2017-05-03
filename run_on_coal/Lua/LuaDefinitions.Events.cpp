#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_eventNamesTable
{
    "onAppStart", "onAppStop",
    "onOGLPreRender", "onOGLRender",
    "onWindowResize", "onWindowFocus",
    "onKeyPress", "onMouseKeyPress", "onMouseScroll", "onCursorMove", "onCursorEnter",
    "onJoypadConnect", "onJoypadButton", "onJoypadAxis",
    "onTextInput",
    "onNetworkStateChange", "onNetworkDataRecieve"
};

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
    int l_funcRef;
    void *l_funcPointer;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_funcRef, l_funcPointer);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->AddEventHandler(l_event, l_funcRef, l_funcPointer);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
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
    void *l_funcPointer;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_funcPointer);
    if(!argStream.HasErrors() && !l_event.empty())
    {
        bool l_result = LuaManager::GetCore()->GetLuaManager()->GetEventManager()->RemoveEventHandler(l_event,l_funcPointer);
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
    // TODO: Add parsing of arguments until top of Lua VM is reached
    if(!argStream.HasErrors() && !l_event.empty())
    {
        LuaManager::GetCore()->GetLuaManager()->GetEventManager()->CallEvent(l_event, &l_arguments);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
