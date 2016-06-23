#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/GlfwManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Glfw.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua 
{

int setCursor(lua_State *f_vm)
{
    Cursor *l_cursor = NULL;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadNextUserdata((void**)&l_cursor,ElementType::CursorElement);
    LuaManager::m_corePointer->GetGlfwManager()->SetCursor(l_cursor);
    lua_pushboolean(f_vm,1);
    return 1;
}
int setCursorMode(lua_State *f_vm)
{
    std::string l_state;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_state);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    if(!l_state.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_state,"normal,hidden,disabled");
    if(l_type == -1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetGlfwManager()->SetCursorMode(GLFW_CURSOR_NORMAL+l_type);
    lua_pushboolean(f_vm,1);
    return 1;
}
int getCursorPosition(lua_State *f_vm)
{
    glm::dvec2 l_pos;
    LuaManager::m_corePointer->GetGlfwManager()->GetCursorPosition(l_pos);
    lua_pushnumber(f_vm,l_pos.x);
    lua_pushnumber(f_vm,l_pos.y);
    return 2;
}
int setCursorPosition(lua_State *f_vm)
{
    lua_Number l_val[2];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i=0; i < 2; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::dvec2 l_pos(l_val[0],l_val[1]);
    LuaManager::m_corePointer->GetGlfwManager()->SetCursorPosition(l_pos);
    lua_pushboolean(f_vm,1);
    return 1;
}

int getWindowPosition(lua_State *f_vm)
{
    glm::ivec2 l_pos;
    LuaManager::m_corePointer->GetGlfwManager()->GetWindowPosition(l_pos);
    lua_pushinteger(f_vm,l_pos.x);
    lua_pushinteger(f_vm,l_pos.y);
    return 2;
}
int getWindowSize(lua_State *f_vm)
{
    glm::ivec2 l_size;
    LuaManager::m_corePointer->GetGlfwManager()->GetWindowSize(l_size);
    lua_pushinteger(f_vm,l_size.x);
    lua_pushinteger(f_vm,l_size.y);
    return 2;
}
int closeApp(lua_State *f_vm)
{
    LuaManager::m_corePointer->GetGlfwManager()->CloseWindow();
    lua_pushboolean(f_vm,1);
    return 1;
}

int getTime(lua_State *f_vm)
{
    double l_time = LuaManager::m_corePointer->GetGlfwManager()->GetTime();
    lua_pushnumber(f_vm,l_time);
    return 1;
}

}
}
