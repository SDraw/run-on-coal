#include "stdafx.h"

#include "Lua/LuaDefs/LuaLightDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Light.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

void ROC::LuaLightDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Light", LightCreate);
    Utils::Lua::lua_registerClassMethod(f_vm, "setParams", LightSetParams);
    Utils::Lua::lua_registerClassMethod(f_vm, "getParams", LightGetParams);
    Utils::Lua::lua_registerClassMethod(f_vm, "setColor", LightSetColor);
    Utils::Lua::lua_registerClassMethod(f_vm, "getColor", LightGetColor);
    Utils::Lua::lua_registerClassMethod(f_vm, "setDirection", LightSetDirection);
    Utils::Lua::lua_registerClassMethod(f_vm, "getDirection", LightGetDirection);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaLightDef::LightCreate(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    Light *l_light = LuaManager::GetCore()->GetElementManager()->CreateLight();
    l_light ? argStream.PushElement(l_light) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::LightSetParams(lua_State *f_vm)
{
    Light *l_light;
    glm::vec4 l_params;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_params[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetParams(l_params);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::LightGetParams(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
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
int ROC::LuaLightDef::LightSetColor(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_color;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetColor(l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::LightGetColor(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
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
int ROC::LuaLightDef::LightSetDirection(lua_State *f_vm)
{
    Light *l_light;
    glm::vec3 l_dir;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_dir[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetDirection(l_dir);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::LightGetDirection(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
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
