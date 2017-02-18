#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Elements/Light.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Light.h"

namespace ROC
{
namespace Lua
{

int lightCreate(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    Light *l_light = LuaManager::GetCore()->GetElementManager()->CreateLight();
    l_light ? argStream.PushPointer(l_light) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightDestroy(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetElementManager()->DestroyLight(l_light);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightSetParams(lua_State *f_vm)
{
    Light *l_light;
    glm::vec4 l_params;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetParams(l_params);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightGetParams(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        glm::vec4 l_params;
        l_light->GetParams(l_params);
        argStream.PushNumber(l_params.x);
        argStream.PushNumber(l_params.y);
        argStream.PushNumber(l_params.z);
        argStream.PushNumber(l_params.w);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightSetColor(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_color;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetColor(l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightGetColor(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_color;
        l_light->GetColor(l_color);
        argStream.PushNumber(l_color.x);
        argStream.PushNumber(l_color.y);
        argStream.PushNumber(l_color.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightSetDirection(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetDirection(l_dir);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int lightGetDirection(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_light), ElementType::LightElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_dir;
        l_light->GetDirection(l_dir);
        argStream.PushNumber(l_dir.x);
        argStream.PushNumber(l_dir.y);
        argStream.PushNumber(l_dir.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
