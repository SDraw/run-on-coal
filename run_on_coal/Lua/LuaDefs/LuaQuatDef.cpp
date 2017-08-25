#include "stdafx.h"

#include "Lua/LuaDefs/LuaQuatDef.h"

#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

#include "Utils/CustomData.h"

void ROC::LuaQuatDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Quat", Create);
    LuaUtils::AddClassMethod(f_vm, "__gc", Destroy);

    LuaUtils::AddClassMethod(f_vm, "getXYZW", GetXYZW);
    LuaUtils::AddClassMethod(f_vm, "setXYZW", SetXYZW);
    LuaUtils::AddClassMethod(f_vm, "setEuler", SetEuler);

    LuaUtils::AddClassMethod(f_vm, "conjugate", Conjugate);
    LuaUtils::AddClassMethod(f_vm, "inverse", Inverse);
    LuaUtils::AddClassMethod(f_vm, "normalize", Normalize);
    LuaUtils::AddClassMethod(f_vm, "length", Length);
    LuaUtils::AddClassMethod(f_vm, "rotate", Rotate);

    LuaUtils::AddClassMethod(f_vm, "lerp", Lerp);
    LuaUtils::AddClassMethod(f_vm, "slerp", Slerp);
    LuaUtils::AddClassMethod(f_vm, "dot", Dot);
    LuaUtils::AddClassMethod(f_vm, "add", Add);
    LuaUtils::AddClassMethod(f_vm, "multiply", Multiply);

    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaQuatDef::Create(lua_State *f_vm)
{
    // userdata Quat(float x, float y, float z)
    // userdata Quat(float x, float y, float z, float w)
    glm::vec3 l_vec;
    float l_quatW = std::nanf("0");
    ArgReader argStream(f_vm);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_vec[i]);
    argStream.ReadNextNumber(l_quatW);
    if(!argStream.HasErrors())
    {
        Quat l_quat;
        l_quat = std::isnan(l_quatW) ? Quat(l_vec) : Quat(l_quatW, l_vec.x, l_vec.y, l_vec.z);
        argStream.PushQuat(l_quat);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaQuatDef::Destroy(lua_State *f_vm)
{
    // GC only
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        delete l_quat;
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::GetXYZW(lua_State *f_vm)
{
    // float float float float Quat:getXYZW()
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        float *l_quatPtr = reinterpret_cast<float*>(l_quat);
        for(int i = 0; i < 4; i++) argStream.PushNumber(l_quatPtr[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaQuatDef::SetXYZW(lua_State *f_vm)
{
    // bool Quat:setXYZW(float x, float y, float z, float w)
    Quat *l_quat;
    Quat l_val;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
    if(!argStream.HasErrors())
    {
        std::memcpy(l_quat, &l_val, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaQuatDef::SetEuler(lua_State *f_vm)
{
    // bool Quat:setEuler(float x, float y, float z)
    Quat *l_quat;
    glm::vec3 l_euler;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_euler[i]);
    if(!argStream.HasErrors())
    {
        Quat l_quatConv(l_euler);
        std::memcpy(l_quat, &l_quatConv, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Add(lua_State *f_vm)
{
    // bool Quat:add(userdata quat)
    Quat *l_quatA, *l_quatB;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quatA);
    argStream.ReadQuat(l_quatB);
    if(!argStream.HasErrors())
    {
        Quat l_quatC = *l_quatA + *l_quatB;
        std::memcpy(l_quatA, &l_quatC, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Multiply(lua_State *f_vm)
{
    // bool Quat:multiply(userdata quat)
    Quat *l_quatA, *l_quatB;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quatA);
    argStream.ReadQuat(l_quatB);
    if(!argStream.HasErrors())
    {
        Quat l_quatC = *l_quatA * *l_quatB;
        std::memcpy(l_quatA, &l_quatC, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Inverse(lua_State *f_vm)
{
    // bool Quat:inverse()
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        Quat l_quatInv = glm::inverse(*l_quat);
        std::memcpy(l_quat, &l_quatInv, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Conjugate(lua_State *f_vm)
{
    // bool Quat:conjugate()
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        Quat l_quatConj = glm::conjugate(*l_quat);
        std::memcpy(l_quat, &l_quatConj, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Normalize(lua_State *f_vm)
{
    // bool Quat:normalize()
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        Quat l_quatNorm = glm::normalize(*l_quat);
        std::memcpy(l_quat, &l_quatNorm, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Length(lua_State *f_vm)
{
    // float Quat:length()
    Quat *l_quat;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    if(!argStream.HasErrors())
    {
        float l_length = glm::length(*l_quat);
        argStream.PushNumber(l_length);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Dot(lua_State *f_vm)
{
    // float Quat:dot()
    Quat *l_quatA, *l_quatB;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quatA);
    argStream.ReadQuat(l_quatB);
    if(!argStream.HasErrors())
    {
        float l_dot = glm::dot(*l_quatA, *l_quatB);
        argStream.PushNumber(l_dot);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Rotate(lua_State *f_vm)
{
    // bool Quat:rotate(float angle, float axisX, float axisY, float axisZ)
    Quat *l_quat;
    float l_angle;
    glm::vec3 l_axis;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quat);
    argStream.ReadNumber(l_angle);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_axis[i]);
    if(!argStream.HasErrors())
    {
        Quat l_quatRot = glm::rotate(*l_quat, l_angle, l_axis);
        std::memcpy(l_quat, &l_quatRot, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaQuatDef::Lerp(lua_State *f_vm)
{
    // bool Quat:lerp(userdata quat, float lerp)
    Quat *l_quatA, *l_quatB;
    float l_blend;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quatA);
    argStream.ReadQuat(l_quatB);
    argStream.ReadNumber(l_blend);
    if(!argStream.HasErrors())
    {
        Quat l_quatC = glm::lerp(*l_quatA, *l_quatB, l_blend);
        std::memcpy(l_quatA, &l_quatC, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaQuatDef::Slerp(lua_State *f_vm)
{
    // bool Quat:slerp(userdata quat, float slerp)
    Quat *l_quatA, *l_quatB;
    float l_blend;
    ArgReader argStream(f_vm);
    argStream.ReadQuat(l_quatA);
    argStream.ReadQuat(l_quatB);
    argStream.ReadNumber(l_blend);
    if(!argStream.HasErrors())
    {
        Quat l_quatC = glm::slerp(*l_quatA, *l_quatB, l_blend);
        std::memcpy(l_quatA, &l_quatC, sizeof(Quat));
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
