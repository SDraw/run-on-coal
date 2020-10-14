#include "stdafx.h"

#include "Lua/LuaDefs/LuaQuatDef.h"

#include "Core/Core.h"
#include "Lua/LuaArgReader.h"
#include "Utils/LuaUtils.h"

void LuaQuatDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Quat", Create);
    LuaUtils::AddClassMethod(f_vm, "__gc", Destroy);

    LuaUtils::AddClassMethod(f_vm, "getXYZW", GetXYZW);
    LuaUtils::AddClassMethod(f_vm, "setXYZW", SetXYZW);
    LuaUtils::AddClassMethod(f_vm, "setEuler", SetEuler);

    LuaUtils::AddClassMethod(f_vm, "conjugate", Conjugate);
    LuaUtils::AddClassMethod(f_vm, "__unm", Inverse);
    LuaUtils::AddClassMethod(f_vm, "inverse", Inverse);
    LuaUtils::AddClassMethod(f_vm, "normalize", Normalize);
    LuaUtils::AddClassMethod(f_vm, "__len", Length);
    LuaUtils::AddClassMethod(f_vm, "length", Length);
    LuaUtils::AddClassMethod(f_vm, "rotate", Rotate);
    LuaUtils::AddClassMethod(f_vm, "rotateVector", RotateVector);

    LuaUtils::AddClassMethod(f_vm, "lerp", Lerp);
    LuaUtils::AddClassMethod(f_vm, "slerp", Slerp);
    LuaUtils::AddClassMethod(f_vm, "dot", Dot);
    LuaUtils::AddClassMethod(f_vm, "__add", Add);
    LuaUtils::AddClassMethod(f_vm, "__sub", Subtract);
    LuaUtils::AddClassMethod(f_vm, "__mul", Multiply);
    LuaUtils::AddClassMethod(f_vm, "__eq", Equal);

    LuaUtils::AddClassFinish(f_vm);
}

int LuaQuatDef::Create(lua_State *f_vm)
{
    // userdata Quat(float x, float y, float z)
    // userdata Quat(float x, float y, float z, float w)
    glm::vec3 l_vec;
    float l_quatW = std::nanf("0");
    LuaArgReader l_argStream(f_vm);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_vec[i]);
    l_argStream.ReadNextNumber(l_quatW);
    if(!l_argStream.HasErrors())
    {
        Quat l_quat = (std::isnan(l_quatW) ? Quat(l_vec) : Quat(l_quatW, l_vec.x, l_vec.y, l_vec.z));
        l_argStream.PushQuat(l_quat);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Destroy(lua_State *f_vm)
{
    // GC only
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        delete l_quat;
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::GetXYZW(lua_State *f_vm)
{
    // float float float float Quat:getXYZW()
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        float *l_quatPtr = reinterpret_cast<float*>(l_quat);
        for(int i = 0; i < 4; i++) l_argStream.PushNumber(l_quatPtr[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::SetXYZW(lua_State *f_vm)
{
    // bool Quat:setXYZW(float x, float y, float z, float w)
    Quat *l_quat;
    Quat l_val;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_val[i]);
    if(!l_argStream.HasErrors())
    {
        std::memcpy(l_quat, &l_val, sizeof(Quat));
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::SetEuler(lua_State *f_vm)
{
    // bool Quat:setEuler(float x, float y, float z)
    Quat *l_quat;
    glm::vec3 l_euler;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_euler[i]);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatConv(l_euler);
        std::memcpy(l_quat, &l_quatConv, sizeof(Quat));
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Add(lua_State *f_vm)
{
    // userdata Quat:add(userdata quat)
    // userdata Quat()+Quat()
    Quat *l_quatA, *l_quatB;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatC = (*l_quatA) + (*l_quatB);
        l_argStream.PushQuat(l_quatC);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Subtract(lua_State *f_vm)
{
    // userdata Quat:subtract(userdata quat)
    // userdata Quat()-Quat()
    Quat *l_quatA, *l_quatB;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatC = (*l_quatA) - (*l_quatB);
        l_argStream.PushQuat(l_quatC);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Multiply(lua_State *f_vm)
{
    // userdata Quat:multiply(userdata quat)
    // userdata Quat()*Quat()
    Quat *l_quatA, *l_quatB;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatC = (*l_quatA) * (*l_quatB);
        l_argStream.PushQuat(l_quatC);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Inverse(lua_State *f_vm)
{
    // userdata Quat:inverse()
    // userdata -Quat()
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatInv = glm::inverse(*l_quat);
        std::memcpy(l_quat, &l_quatInv, sizeof(Quat));
        l_argStream.PushQuat(l_quatInv);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Conjugate(lua_State *f_vm)
{
    // userdata Quat:conjugate()
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatConj = glm::conjugate(*l_quat);
        l_argStream.PushQuat(l_quatConj);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Normalize(lua_State *f_vm)
{
    // userdata Quat:normalize()
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatNorm = glm::normalize(*l_quat);
        l_argStream.PushQuat(l_quatNorm);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Length(lua_State *f_vm)
{
    // float Quat:length()
    // float #Quat()
    Quat *l_quat;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    if(!l_argStream.HasErrors())
    {
        float l_length = glm::length(*l_quat);
        l_argStream.PushNumber(l_length);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Dot(lua_State *f_vm)
{
    // float Quat:dot(userdata quat)
    Quat *l_quatA, *l_quatB;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    if(!l_argStream.HasErrors())
    {
        float l_dot = glm::dot(*l_quatA, *l_quatB);
        l_argStream.PushNumber(l_dot);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Rotate(lua_State *f_vm)
{
    // userdata Quat:rotate(float angle, float axisX, float axisY, float axisZ)
    Quat *l_quat;
    float l_angle;
    glm::vec3 l_axis;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    l_argStream.ReadNumber(l_angle);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_axis[i]);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatRot = glm::rotate(*l_quat, l_angle, l_axis);
        l_argStream.PushQuat(l_quatRot);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::RotateVector(lua_State *f_vm)
{
    // float float float Quat:rotateVector(float x, float y, float z)
    Quat *l_quat;
    glm::vec3 l_vec;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quat);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_vec[i]);
    if(!l_argStream.HasErrors())
    {
        glm::vec3 l_result = (*l_quat) * l_vec;
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_result[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Lerp(lua_State *f_vm)
{
    // userdata Quat:lerp(userdata quat, float lerp)
    Quat *l_quatA, *l_quatB;
    float l_blend;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    l_argStream.ReadNumber(l_blend);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatC = glm::lerp(*l_quatA, *l_quatB, l_blend);
        l_argStream.PushQuat(l_quatC);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Slerp(lua_State *f_vm)
{
    // userdata Quat:slerp(userdata quat, float slerp)
    Quat *l_quatA, *l_quatB;
    float l_blend;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    l_argStream.ReadNumber(l_blend);
    if(!l_argStream.HasErrors())
    {
        Quat l_quatC = glm::slerp(*l_quatA, *l_quatB, l_blend);
        l_argStream.PushQuat(l_quatC);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaQuatDef::Equal(lua_State *f_vm)
{
    // bool Quat:equal(userdata quat)
    // bool (Quat() == Quat())
    Quat *l_quatA, *l_quatB;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadQuat(l_quatA);
    l_argStream.ReadQuat(l_quatB);
    if(!l_argStream.HasErrors())
    {
        bool l_result = ((*l_quatA) == (*l_quatB));
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
