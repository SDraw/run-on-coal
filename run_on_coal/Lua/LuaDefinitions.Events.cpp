#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{
const std::vector<std::string> g_eventNamesTable
{
    "onOGLPreRender", "onOGLRender",
    "onWindowResize", "onWindowFocus",
    "onKeyPress", "onMouseKeyPress", "onMouseScroll", "onCursorMove", "onCursorEnter",
    "onJoypadConnect", "onJoypadButton", "onJoypadAxis",
    "onTextInput"
};

int addEvent(lua_State *f_vm)
{
    std::string l_event;
    int l_func;
    void *l_point;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.DecreaseArguments(2);
    argStream.ReadText(l_event);
    argStream.ReadFunction(l_func, &l_point);
    if(argStream.HasErrors() || l_event.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_enumValue = Utils::ReadEnumVector(g_eventNamesTable, l_event);
    if(l_enumValue == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetLuaManager()->GetEventManager()->AddEvent(l_enumValue, l_func, l_point);
    lua_pushboolean(f_vm, result);
    return 1;
}
int setEventMute(lua_State *f_vm)
{
    std::string l_event;
    void *l_point;
    bool l_mute;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_event);
    argStream.ReadFunctionPointer(&l_point);
    argStream.ReadBoolean(l_mute);
    if(argStream.HasErrors() || l_event.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_enumValue = Utils::ReadEnumVector(g_eventNamesTable, l_event);
    if(l_enumValue == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetLuaManager()->GetEventManager()->SetEventMute(l_enumValue, l_point, l_mute);
    lua_pushboolean(f_vm, result);
    return 1;
}
int removeEvent(lua_State *f_vm)
{
    std::string l_event;
    void *l_point;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_event);
    argStream.ReadFunctionPointer(&l_point);
    if(argStream.HasErrors() || l_event.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_enumValue = Utils::ReadEnumVector(g_eventNamesTable, l_event);
    if(l_enumValue == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetLuaManager()->GetEventManager()->RemoveEvent(l_enumValue, l_point);
    lua_pushboolean(f_vm, result);
    return 1;
}
}
}