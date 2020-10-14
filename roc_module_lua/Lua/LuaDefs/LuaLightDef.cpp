#include "stdafx.h"

#include "Lua/LuaDefs/LuaLightDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Core/Core.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_lightTypes
{
    "directional", "point", "spotlight"
};

void LuaLightDef::Init(lua_State *f_vm)
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

int LuaLightDef::Create(lua_State *f_vm)
{
    // element Light(string type)
    std::string l_typeString;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_typeString);
    if(!l_argStream.HasErrors() && !l_typeString.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_typeString, g_lightTypes);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            ROC::ILight *l_light = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateILight(static_cast<unsigned char>(l_type));
            l_light ? l_argStream.PushElement(l_light) : l_argStream.PushBoolean(false);
        }
        else l_argStream.PushBoolean(false);
    }
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetPosition(lua_State *f_vm)
{
    // bool Light:setPosition(float x, float y, float z)
    ROC::ILight *l_light;
    glm::vec3 l_pos;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    if(!l_argStream.HasErrors())
    {
        l_light->SetPosition(l_pos);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetPosition(lua_State *f_vm)
{
    // float float float Light:getPosition()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_pos = l_light->GetPosition();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_pos[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetDirection(lua_State *f_vm)
{
    // bool Light:setDirection(float x, float y, float z)
    ROC::ILight *l_light;
    glm::vec3 l_dir;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_dir[i]);
    if(!l_argStream.HasErrors())
    {
        l_light->SetDirection(l_dir);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetDirection(lua_State *f_vm)
{
    // float float float Light:getDirection()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_dir = l_light->GetDirection();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_dir[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetColor(lua_State *f_vm)
{
    // bool Light:setColor(float colorR, float colorG, float colorB, float intensity)
    ROC::ILight *l_light;
    glm::vec4 l_color;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_color[i]);
    if(!l_argStream.HasErrors())
    {
        l_light->SetColor(l_color);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetColor(lua_State *f_vm)
{
    // float float float float Light:getColor()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        const glm::vec4 &l_color = l_light->GetColor();
        for(int i = 0; i < 4; i++) l_argStream.PushNumber(l_color[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetCutoff(lua_State *f_vm)
{
    // bool Light:setFalloff(float v1, float v2)
    ROC::ILight *l_light;
    glm::vec2 l_cutoff;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_cutoff[i]);
    if(!l_argStream.HasErrors())
    {
        l_light->SetCutoff(l_cutoff);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetCutoff(lua_State *f_vm)
{
    // float float float Light:getFalloff()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        const glm::vec2 &l_cutoff = l_light->GetCutoff();
        for(int i = 0; i < 2; i++) l_argStream.PushNumber(l_cutoff[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetFalloff(lua_State *f_vm)
{
    // bool Light:setFalloff(float param1, float param2, float param3)
    ROC::ILight *l_light;
    glm::vec3 l_falloff;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_falloff[i]);
    if(!l_argStream.HasErrors())
    {
        l_light->SetFalloff(l_falloff);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetFalloff(lua_State *f_vm)
{
    // float float float Light:getFalloff()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3 &l_falloff = l_light->GetFalloff();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_falloff[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaLightDef::SetType(lua_State *f_vm)
{
    // bool Light:setType()
    ROC::ILight *l_light;
    std::string l_typeString;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    l_argStream.ReadText(l_typeString);
    if(!l_argStream.HasErrors() && !l_typeString.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_typeString, g_lightTypes);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            l_light->SetType(static_cast<unsigned char>(l_type));
            l_argStream.PushBoolean(true);
        }
        else l_argStream.PushBoolean(false);
    }
    return l_argStream.GetReturnValue();
}

int LuaLightDef::GetType(lua_State *f_vm)
{
    // str Light:getType()
    ROC::ILight *l_light;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_light);
    !l_argStream.HasErrors() ? l_argStream.PushText(g_lightTypes[l_light->GetType()]) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
