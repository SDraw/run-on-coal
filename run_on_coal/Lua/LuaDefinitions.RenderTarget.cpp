#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Scene/RenderTarget.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.RenderTarget.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{
int rtCreate(lua_State *f_vm)
{
    LUA_INTEGER l_par[3];
    std::string l_type;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    for(int i = 0; i < 3; i++) argStream.ReadInteger(l_par[i]);
    argStream.ReadText(l_type);
    if(argStream.HasErrors() || l_par[0] < 1 || l_par[1] < 1 || l_par[2] < 0 || !l_type.length())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_etype = Utils::ReadEnumString(l_type, "depth,rgb,rgba,rgbf");
    if(l_etype == -1)
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    glm::ivec2 l_size(l_par[0], l_par[1]);
    RenderTarget *l_rt = LuaManager::m_corePointer->GetElementManager()->CreateRenderTarget(static_cast<unsigned int>(l_par[2]), l_size, RENDERTARGET_TYPE_DEPTH + l_etype);
    l_rt ? lua_pushlightuserdata(f_vm, l_rt) : lua_pushboolean(f_vm, 0);
    return 1;
}
int rtDestroy(lua_State *f_vm)
{
    RenderTarget *l_rt;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_rt, ElementType::RenderTargetElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyRenderTarget(l_rt);
    lua_pushboolean(f_vm, result);
    return 1;
}
int rtDraw(lua_State *f_vm)
{
    RenderTarget *l_rt;
    lua_Number l_val[5];
    l_val[4] = 0.0;
    lua_Number l_color[4] = { 1.0, 1.0, 1.0, 1.0 };
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_rt, ElementType::RenderTargetElement);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
    argStream.ReadNextNumber(l_val[4]);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    glm::vec2 l_pos(l_val[0], l_val[1] + l_val[3]);
    glm::vec2 l_size(l_val[2], -l_val[3]);
    glm::vec4 l_vColor(l_color[0], l_color[1], l_color[2], l_color[3]);
    LuaManager::m_corePointer->GetRenderManager()->Render(l_rt, l_pos, l_size, static_cast<float>(l_val[4]), l_vColor);
    lua_pushboolean(f_vm, 0);
    return 1;
}
}
}