#include "stdafx.h"

#include "Lua/LuaDefs/LuaLightDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Elements/Light.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

namespace ROC
{

const std::vector<std::string> g_LightTypesTable
{
    "directional", "point", "spotlight"
};

}

void ROC::LuaLightDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Light", Create);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "setColor", SetColor);
    LuaUtils::AddClassMethod(f_vm, "getColor", GetColor);
    LuaUtils::AddClassMethod(f_vm, "setDirection", SetDirection);
    LuaUtils::AddClassMethod(f_vm, "getDirection", GetDirection);
    LuaUtils::AddClassMethod(f_vm, "setFalloff", SetFalloff);
    LuaUtils::AddClassMethod(f_vm, "getFalloff", GetFalloff);
    LuaUtils::AddClassMethod(f_vm, "setCutoff", SetCutoff);
    LuaUtils::AddClassMethod(f_vm, "getCutoff", GetCutoff);
    LuaUtils::AddClassMethod(f_vm, "setType", SetType);
    LuaUtils::AddClassMethod(f_vm, "getType", GetType);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaLightDef::Create(lua_State *f_vm)
{
    // element Light(string type)
    std::string l_typeString;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_typeString);
    if(!argStream.HasErrors() && !l_typeString.empty())
    {
        int l_type = EnumUtils::ReadEnumVector(l_typeString, g_LightTypesTable);
        if(l_type != -1)
        {
            Light *l_light = Core::GetCore()->GetElementManager()->CreateLight(static_cast<unsigned char>(l_type));
            l_light ? argStream.PushElement(l_light) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetPosition(lua_State *f_vm)
{
    // bool Light:setPosition(float x, float y, float z)
    Light *l_light;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::GetPosition(lua_State *f_vm)
{
    // float float float Light:getPosition()
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        const glm::vec3 &l_pos = l_light->GetPosition();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetDirection(lua_State *f_vm)
{
    // bool Light:setDirection(float x, float y, float z)
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
int ROC::LuaLightDef::GetDirection(lua_State *f_vm)
{
    // float float float Light:getDirection()
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        const glm::vec3 &l_dir = l_light->GetDirection();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_dir[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetColor(lua_State *f_vm)
{
    // bool Light:setColor(float colorR, float colorG, float colorB, float intensity)
    Light *l_light;
    glm::vec4 l_color;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetColor(l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::GetColor(lua_State *f_vm)
{
    // float float float float Light:getColor()
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        const glm::vec4 &l_color = l_light->GetColor();
        for(int i = 0; i < 4; i++) argStream.PushNumber(l_color[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetCutoff(lua_State *f_vm)
{
    // bool Light:setFalloff(float v1, float v2)
    Light *l_light;
    glm::vec2 l_cutoff;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_cutoff[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetCutoff(l_cutoff);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::GetCutoff(lua_State *f_vm)
{
    // float float float Light:getFalloff()
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        const glm::vec2 &l_cutoff = l_light->GetCutoff();
        for(int i = 0; i < 2; i++) argStream.PushNumber(l_cutoff[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetFalloff(lua_State *f_vm)
{
    // bool Light:setFalloff(float param1, float param2, float param3)
    Light *l_light;
    glm::vec3 l_falloff;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_falloff[i]);
    if(!argStream.HasErrors())
    {
        l_light->SetFalloff(l_falloff);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::GetFalloff(lua_State *f_vm)
{
    // float float float Light:getFalloff()
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    if(!argStream.HasErrors())
    {
        const glm::vec3 &l_falloff = l_light->GetFalloff();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_falloff[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaLightDef::SetType(lua_State *f_vm)
{
    // bool Light:setType()
    Light *l_light;
    std::string l_typeString;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    argStream.ReadText(l_typeString);
    if(!argStream.HasErrors() && !l_typeString.empty())
    {
        int l_type = EnumUtils::ReadEnumVector(l_typeString, g_LightTypesTable);
        if(l_type != -1)
        {
            l_light->SetType(static_cast<unsigned char>(l_type));
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    return argStream.GetReturnValue();
}
int ROC::LuaLightDef::GetType(lua_State *f_vm)
{
    Light *l_light;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_light);
    !argStream.HasErrors() ? argStream.PushText(g_LightTypesTable[l_light->GetType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
