#include "stdafx.h"

#include "Lua/LuaDefs/LuaCollisionDef.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaCollidableDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_CollisionTypes
{
    "sphere", "box", "cylinder", "capsule", "cone"
};
const std::vector<std::string> g_CollisionMotionTypes
{
    "default", "static", "kinematic"
};

void LuaCollisionDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Collision", Create);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "setRotation", SetRotation);
    LuaUtils::AddClassMethod(f_vm, "getRotation", GetRotation);
    LuaUtils::AddClassMethod(f_vm, "setScale", SetScale);
    LuaUtils::AddClassMethod(f_vm, "getScale", GetScale);
    LuaUtils::AddClassMethod(f_vm, "getMass", GetMass);
    LuaUtils::AddClassMethod(f_vm, "setVelocity", SetVelocity);
    LuaUtils::AddClassMethod(f_vm, "getVelocity", GetVelocity);
    LuaUtils::AddClassMethod(f_vm, "setAngularVelocity", SetAngularVelocity);
    LuaUtils::AddClassMethod(f_vm, "getAngularVelocity", GetAngularVelocity);
    LuaUtils::AddClassMethod(f_vm, "setFriction", SetFriction);
    LuaUtils::AddClassMethod(f_vm, "getFriction", GetFriction);
    LuaUtils::AddClassMethod(f_vm, "setRestitution", SetRestitution);
    LuaUtils::AddClassMethod(f_vm, "getRestitution", GetRestitution);
    LuaUtils::AddClassMethod(f_vm, "setLinearFactor", SetLinearFactor);
    LuaUtils::AddClassMethod(f_vm, "getLinearFactor", GetLinearFactor);
    LuaUtils::AddClassMethod(f_vm, "setAngularFactor", SetAngularFactor);
    LuaUtils::AddClassMethod(f_vm, "getAngularFactor", GetAngularFactor);
    LuaUtils::AddClassMethod(f_vm, "applyForce", ApplyForce);
    LuaUtils::AddClassMethod(f_vm, "applyImpulse", ApplyImpulse);
    LuaUtils::AddClassMethod(f_vm, "applyTorque", ApplyTorque);
    LuaUtils::AddClassMethod(f_vm, "setMotionType", SetMotionType);
    LuaUtils::AddClassMethod(f_vm, "getMotionType", GetMotionType);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaCollidableDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaCollisionDef::Create(lua_State *f_vm)
{
    // element Collision(str type [, float mass = 1.0, float sx = 1.0, float sy = 1.0, float sz = 1.0])
    std::string l_typeString;
    float l_mass = 1.f;
    glm::vec3 l_size(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadText(l_typeString);
    argStream.ReadNextNumber(l_mass);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_size[i]);
    if(!argStream.HasErrors() && !l_typeString.empty())
    {
        size_t l_type = EnumUtils::ReadEnumVector(l_typeString, g_CollisionTypes);
        if(l_type != std::numeric_limits<size_t>::max())
        {
            ROC::ICollision *l_col = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateICollision(static_cast<unsigned char>(l_type), l_size, l_mass);
            l_col ? argStream.PushElement(l_col) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetPosition(lua_State *f_vm)
{
    // bool Collision:setPosition(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetPosition(lua_State *f_vm)
{
    // float float float Collision:getPosition()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_col->GetPosition(l_pos);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetRotation(lua_State *f_vm)
{
    // bool Collision:setRotation(float x, float y, float z, float w)
    ROC::ICollision *l_col;
    glm::quat l_rot;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_rot[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetRotation(l_rot);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetRotation(lua_State *f_vm)
{
    // float float float float Collision:getRotation()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot;
        l_col->GetRotation(l_rot);
        for(int i = 0; i < 4; i++) argStream.PushNumber(l_rot[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetScale(lua_State *f_vm)
{
    // bool CollisionSetScale(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_scale;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_scale[i]);
    if(!argStream.HasErrors())
    {
        LuaModule::GetModule()->GetEngineCore()->GetIPhysicsManager()->SetCollisionScale(l_col, l_scale);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetScale(lua_State *f_vm)
{
    // float float float Collision:getScale()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_scale;
        l_col->GetScale(l_scale);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_scale[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetMass(lua_State *f_vm)
{
    // float Collision:getMass()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        float l_mass = l_col->GetMass();
        argStream.PushNumber(l_mass);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetVelocity(lua_State *f_vm)
{
    // bool Collision:setVelocity(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_velocity;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_velocity[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetVelocity(l_velocity);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetVelocity(lua_State *f_vm)
{
    // float float float Collision:getVelocity()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        l_col->GetVelocity(l_velocity);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_velocity[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetAngularVelocity(lua_State *f_vm)
{
    // bool Collision:setAngularVelocity(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_angularVelocity;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_angularVelocity[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetAngularVelocity(l_angularVelocity);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetAngularVelocity(lua_State *f_vm)
{
    // float float float Collision:getAngularVelocity()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_angularVelocity;
        l_col->GetAngularVelocity(l_angularVelocity);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_angularVelocity[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetFriction(lua_State *f_vm)
{
    // bool Collision:setFriction(float friction)
    ROC::ICollision *l_col;
    float l_friction;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    argStream.ReadNumber(l_friction);
    if(!argStream.HasErrors())
    {
        l_col->SetFriction(l_friction);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetFriction(lua_State *f_vm)
{
    // float Collision:getFriction()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        float l_friction = l_col->GetFriction();
        argStream.PushNumber(l_friction);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetRestitution(lua_State *f_vm)
{
    // bool Collision:setRestitution(float restitution)
    ROC::ICollision *l_col;
    float l_restitution;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    argStream.ReadNumber(l_restitution);
    if(!argStream.HasErrors())
    {
        l_col->SetRestitution(l_restitution);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetRestitution(lua_State *f_vm)
{
    // float Collision:getRestitution()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        float l_restitution = l_col->GetRestitution();
        argStream.PushNumber(l_restitution);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetLinearFactor(lua_State *f_vm)
{
    // bool Collision:setLinearFactor(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_linearFactor;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_linearFactor[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetLinearFactor(l_linearFactor);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetLinearFactor(lua_State *f_vm)
{
    // float float float Collision:getLinearFactor()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_linearFactor;
        l_col->GetLinearFactor(l_linearFactor);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_linearFactor[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetAngularFactor(lua_State *f_vm)
{
    // bool Collision:setAngularFactor(float x, float y, float z)
    ROC::ICollision *l_col;
    glm::vec3 l_angularFactor;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_angularFactor[i]);
    if(!argStream.HasErrors())
    {
        l_col->SetAngularFactor(l_angularFactor);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetAngularFactor(lua_State *f_vm)
{
    // float float float Collision:getAngularFactor()
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_angularFactor;
        l_col->GetAngularFactor(l_angularFactor);
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_angularFactor[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::ApplyForce(lua_State *f_vm)
{
    // bool Collision:applyForce(float forceX, float forceY, float forceZ [, float relX = 0, float relY = 0, float relZ = 0])
    ROC::ICollision *l_collision;
    glm::vec3 l_force;
    glm::vec3 l_relPos(0.f);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_force[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_relPos[i]);
    if(!argStream.HasErrors())
    {
        (glm::length(l_relPos) == 0.f) ? l_collision->ApplyCentralForce(l_force) : l_collision->ApplyForce(l_force, l_relPos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::ApplyImpulse(lua_State *f_vm)
{
    // bool Collision:applyImpulse(float impX, float impY, float impZ [, float relX = 0, float relY = 0, float relZ = 0])
    ROC::ICollision *l_collision;
    glm::vec3 l_impulse;
    glm::vec3 l_relPos(0.f);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_impulse[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_relPos[i]);
    if(!argStream.HasErrors())
    {
        (glm::length(l_relPos) == 0.f) ? l_collision->ApplyCentralImpulse(l_impulse) : l_collision->ApplyImpulse(l_impulse, l_relPos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::ApplyTorque(lua_State *f_vm)
{
    // bool Collision:applyTorque(float x, float y, float z)
    ROC::ICollision *l_collision;
    glm::vec3 l_torque;
    bool l_impulse = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_torque[i]);
    argStream.ReadNextBoolean(l_impulse);
    if(!argStream.HasErrors())
    {
        l_collision->ApplyTorque(l_torque, l_impulse);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::SetMotionType(lua_State *f_vm)
{
    // bool Collision:setMotionType(str type)
    ROC::ICollision *l_collision;
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_type.empty())
    {
        size_t l_idx = EnumUtils::ReadEnumVector(l_type, g_CollisionMotionTypes);
        if(l_idx != std::numeric_limits<size_t>::max())
        {
            l_collision->SetMotionType(static_cast<unsigned char>(l_idx));
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaCollisionDef::GetMotionType(lua_State *f_vm)
{
    // str Collision:getMotionType()
    ROC::ICollision *l_collision;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    !argStream.HasErrors() ? argStream.PushText(g_CollisionMotionTypes[l_collision->GetMotionType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
