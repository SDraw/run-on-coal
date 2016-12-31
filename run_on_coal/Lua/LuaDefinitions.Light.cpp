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
    l_light ? lua_pushlightuserdata(f_vm, l_light) : lua_pushboolean(f_vm, 0);
    return 1;
}
int lightDestroy(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyLight(l_light);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int lightSetParams(lua_State *f_vm)
{
    Light *l_light;
    glm::vec4 l_params;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetParams(l_params);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int lightGetParams(lua_State *f_vm)
{
    Light *l_light;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {

        glm::vec4 l_params;
        l_light->GetParams(l_params);
        lua_pushnumber(f_vm, l_params.x);
        lua_pushnumber(f_vm, l_params.y);
        lua_pushnumber(f_vm, l_params.z);
        lua_pushnumber(f_vm, l_params.w);
        l_returnVal = 4;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}
int lightSetColor(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_color;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetColor(l_color);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int lightGetColor(lua_State *f_vm)
{
    Light *l_light;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 f_color;
        l_light->GetColor(f_color);
        lua_pushnumber(f_vm, f_color.x);
        lua_pushnumber(f_vm, f_color.y);
        lua_pushnumber(f_vm, f_color.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}
int lightSetDirection(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetDirection(l_dir);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int lightGetDirection(lua_State *f_vm)
{
    Light *l_light;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_dir;
        l_light->GetDirection(l_dir);
        lua_pushnumber(f_vm, l_dir.x);
        lua_pushnumber(f_vm, l_dir.y);
        lua_pushnumber(f_vm, l_dir.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

}
}