#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Scene/Light.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Light.h"

namespace ROC
{
namespace Lua 
{

int lightCreate(lua_State *f_vm)
{
    Light *l_light = LuaManager::m_corePointer->GetElementManager()->CreateLight();
    l_light ? lua_pushlightuserdata(f_vm,l_light) : lua_pushboolean(f_vm,0);
    return 1;
}
int lightDestroy(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyLight(l_light);
    lua_pushboolean(f_vm,result);
    return 1;
}
int lightSetParams(lua_State *f_vm)
{
    Light *l_light;
    lua_Number l_params[4];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    for(int i=0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec4 l_vparams(l_params[0],l_params[1],l_params[2],l_params[3]);
    l_light->SetParams(l_vparams);
    lua_pushboolean(f_vm,1);
    return 1;
}
int lightGetParams(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec4 l_params;
    l_light->GetParams(l_params);
    lua_pushnumber(f_vm,l_params.x);
    lua_pushnumber(f_vm,l_params.y);
    lua_pushnumber(f_vm,l_params.z);
    lua_pushnumber(f_vm,l_params.w);
    return 4;
}
int lightSetColor(lua_State *f_vm)
{
    Light *l_light;
    lua_Number f_inten[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    for(int i=0; i < 3; i++) argStream.ReadNumber(f_inten[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 f_color(f_inten[0],f_inten[1],f_inten[2]);
    l_light->SetColor(f_color);
    lua_pushboolean(f_vm,1);
    return 1;
}
int lightGetColor(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 f_color;
    l_light->GetColor(f_color);
    lua_pushnumber(f_vm,f_color.x);
    lua_pushnumber(f_vm,f_color.y);
    lua_pushnumber(f_vm,f_color.z);
    return 3;
}
int lightSetDirection(lua_State *f_vm)
{
    Light *l_light;
    lua_Number l_dir[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    for(int i=0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vDir(l_dir[0],l_dir[1],l_dir[2]);
    l_light->SetDirection(l_vDir);
    lua_pushboolean(f_vm,1);
    return 1;
}
int lightGetDirection(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_light,ElementType::LightElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vDir;
    l_light->GetDirection(l_vDir);
    lua_pushnumber(f_vm,l_vDir.x);
    lua_pushnumber(f_vm,l_vDir.y);
    lua_pushnumber(f_vm,l_vDir.z);
    return 3;
}

}
}
