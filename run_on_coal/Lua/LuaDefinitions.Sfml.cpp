#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Sfml.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

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
    int l_type = Utils::ReadEnumString(l_state,"hidden,normal,disabled");
    if(l_type==-1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetSfmlManager()->SetCursorMode(l_type);
    lua_pushboolean(f_vm,1);
    return 1;
}
int getCursorPosition(lua_State *f_vm)
{
    glm::dvec2 l_pos;
    LuaManager::m_corePointer->GetSfmlManager()->GetCursorPosition(l_pos);
    lua_pushnumber(f_vm,l_pos.x);
    lua_pushnumber(f_vm,l_pos.y);
    return 2;
}
int setCursorPosition(lua_State *f_vm)
{
    lua_Number l_val[2];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i = 0; i<2; i++) argStream.ReadNumber(l_val[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::dvec2 l_pos(l_val[0],l_val[1]);
    LuaManager::m_corePointer->GetSfmlManager()->SetCursorPosition(l_pos);
    lua_pushboolean(f_vm,1);
    return 1;
}

int getWindowPosition(lua_State *f_vm)
{
    glm::ivec2 l_pos;
    LuaManager::m_corePointer->GetSfmlManager()->GetWindowPosition(l_pos);
    lua_pushinteger(f_vm,l_pos.x);
    lua_pushinteger(f_vm,l_pos.y);
    return 2;
}
int getWindowSize(lua_State *f_vm)
{
    glm::ivec2 l_size;
    LuaManager::m_corePointer->GetSfmlManager()->GetWindowSize(l_size);
    lua_pushinteger(f_vm,l_size.x);
    lua_pushinteger(f_vm,l_size.y);
    return 2;
}
int closeApp(lua_State *f_vm)
{
    LuaManager::m_corePointer->GetSfmlManager()->CloseWindow();
    lua_pushboolean(f_vm,1);
    return 1;
}

int getTime(lua_State *f_vm)
{
    float l_time = LuaManager::m_corePointer->GetSfmlManager()->GetTime();
    lua_pushnumber(f_vm,static_cast<lua_Number>(l_time));
    return 1;
}

}
}
