#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.OpenGL.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

int oglClear(lua_State *f_vm)
{
    std::string l_param;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_param);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    if(l_param.length() == 0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_result = true;
    switch(Utils::ReadEnumString(l_param,"color,depth"))
    {
    case 0:
        LuaManager::m_corePointer->GetRenderManager()->ClearRenderArea(GL_COLOR_BUFFER_BIT);
        break;
    case 1:
        LuaManager::m_corePointer->GetRenderManager()->ClearRenderArea(GL_DEPTH_BUFFER_BIT);
        break;
    default:
        l_result = false;
    }
    lua_pushboolean(f_vm,l_result);
    return 1;
}
int oglClearColor(lua_State *f_vm)
{
    lua_Number l_color[4];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec4 l_vColor(l_color[0],l_color[1],l_color[2],l_color[3]);
    LuaManager::m_corePointer->GetRenderManager()->SetClearColour(l_vColor);
    lua_pushboolean(f_vm,1);
    return 1;
}
int oglViewport(lua_State *f_vm)
{
    LUA_INTEGER l_params[4];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_params[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::ivec4 l_area(l_params[0],l_params[1],l_params[2],l_params[3]);
    LuaManager::m_corePointer->GetRenderManager()->SetViewport(l_area);
    lua_pushboolean(f_vm,1);
    return 1;
}
int oglPolygonMode(lua_State *f_vm)
{
    std::string l_mode;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_mode);
    if(argStream.HasErrors() || !l_mode.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_mode,"point,line,fill");
    if(l_type == -1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetRenderManager()->SetPolygonMode(static_cast<unsigned int>(l_type));
    lua_pushboolean(f_vm,1);
    return 1;
}

}
}
