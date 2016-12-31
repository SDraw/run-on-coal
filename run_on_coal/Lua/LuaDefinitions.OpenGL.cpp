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

const std::vector<std::string> g_buffersTable
{
    "color", "depth"
};
const std::vector<std::string> g_drawingTable
{
    "point", "line", "fill"
};

int oglClear(lua_State *f_vm)
{
    std::string l_param;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_param);
    if(!argStream.HasErrors() && !l_param.empty())
    {
        int l_type = Utils::ReadEnumVector(g_buffersTable, l_param);
        if(l_type != -1)
        {
            GLbitfield l_buffer = (l_type == 0) ? GL_COLOR_BUFFER_BIT : GL_DEPTH_BUFFER_BIT;
            LuaManager::m_corePointer->GetRenderManager()->ClearRenderArea(l_buffer);
            lua_pushboolean(f_vm, 1);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int oglClearColor(lua_State *f_vm)
{
    glm::vec4 l_color;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetRenderManager()->SetClearColour(l_color);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int oglViewport(lua_State *f_vm)
{
    glm::ivec4 l_area;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_area[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetRenderManager()->SetViewport(l_area);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int oglPolygonMode(lua_State *f_vm)
{
    std::string l_mode;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_mode);
    if(!argStream.HasErrors() && !l_mode.empty())
    {
        int l_type = Utils::ReadEnumVector(g_drawingTable, l_mode);
        if(l_type != -1)
        {
            LuaManager::m_corePointer->GetRenderManager()->SetPolygonMode(l_type);
            lua_pushboolean(f_vm, 1);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}