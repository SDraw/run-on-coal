#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Elements/Collision.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Collision.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_collisionTypesTable
{
    "sphere", "box", "cylinder", "capsule", "cone"
};
const std::vector<std::string> g_collisionMotionTypesTable
{
    "default", "static", "kinematic"
};

int collisionCreate(lua_State *f_vm)
{
    std::string l_typeString;
    float l_mass = 1.f;
    glm::vec3 l_size(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadText(l_typeString);
    argStream.ReadNextNumber(l_mass);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_size[i]);
    if(!argStream.HasErrors() && !l_typeString.empty())
    {
        int l_type = Utils::ReadEnumVector(g_collisionTypesTable, l_typeString);
        if(l_type != -1)
        {
            Collision *l_col = LuaManager::GetCore()->GetElementManager()->CreateCollision(l_type, l_size, l_mass);
            l_col ? argStream.PushPointer(l_col) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionSetPosition(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetPosition(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_col->GetPosition(l_pos);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionSetRotation(lua_State *f_vm)
{
    Collision *l_col;
    glm::vec4 l_val(0.f, 0.f, 0.f, std::nanf("0"));
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
    argStream.ReadNextNumber(l_val.w);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot = std::isnan(l_val.w) ? glm::quat(l_val) : glm::quat(l_val.w, l_val.x, l_val.y, l_val.z);
        l_col->SetRotation(l_rot);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionGetRotation(lua_State *f_vm)
{
    Collision *l_col;
    bool l_reqQuat = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    argStream.ReadNextBoolean(l_reqQuat);
    if(!argStream.HasErrors())
    {
        glm::quat l_rot;
        l_col->GetRotation(l_rot);
        if(l_reqQuat)
        {
            argStream.PushNumber(l_rot.x);
            argStream.PushNumber(l_rot.y);
            argStream.PushNumber(l_rot.z);
            argStream.PushNumber(l_rot.w);
        }
        else
        {
            glm::vec3 l_vRot = glm::eulerAngles(l_rot);
            argStream.PushNumber(l_vRot.x);
            argStream.PushNumber(l_vRot.y);
            argStream.PushNumber(l_vRot.z);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionSetScale(lua_State *f_vm)
{
    Collision *l_col;
    glm::vec3 l_scale;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_scale[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetPhysicsManager()->SetCollisionScale(l_col, l_scale);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionGetScale(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_scale;
        l_col->GetScale(l_scale);
        argStream.PushNumber(l_scale.x);
        argStream.PushNumber(l_scale.y);
        argStream.PushNumber(l_scale.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionGetMass(lua_State *f_vm)
{
    Collision *l_col;
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

int collisionSetVelocity(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetVelocity(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        l_col->GetVelocity(l_velocity);
        argStream.PushNumber(l_velocity.x);
        argStream.PushNumber(l_velocity.y);
        argStream.PushNumber(l_velocity.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionSetAngularVelocity(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetAngularVelocity(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_angularVelocity;
        l_col->GetAngularVelocity(l_angularVelocity);
        argStream.PushNumber(l_angularVelocity.x);
        argStream.PushNumber(l_angularVelocity.y);
        argStream.PushNumber(l_angularVelocity.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionSetFriction(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetFriction(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionSetLinearFactor(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetLinearFactor(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_linearFactor;
        l_col->GetLinearFactor(l_linearFactor);
        argStream.PushNumber(l_linearFactor.x);
        argStream.PushNumber(l_linearFactor.y);
        argStream.PushNumber(l_linearFactor.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionSetAngularFactor(lua_State *f_vm)
{
    Collision *l_col;
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
int collisionGetAngularFactor(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_angularFactor;
        l_col->GetAngularFactor(l_angularFactor);
        argStream.PushNumber(l_angularFactor.x);
        argStream.PushNumber(l_angularFactor.y);
        argStream.PushNumber(l_angularFactor.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionApplyForce(lua_State *f_vm)
{
    Collision *l_collision;
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
int collisionApplyImpulse(lua_State *f_vm)
{
    Collision *l_collision;
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
int collisionApplyTorque(lua_State *f_vm)
{
    Collision *l_collision;
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

int collisionSetMotionType(lua_State *f_vm)
{
    Collision *l_collision;
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_type.empty())
    {
        int l_idx = Utils::ReadEnumVector(g_collisionMotionTypesTable, l_type);
        if(l_idx != -1)
        {
            l_collision->SetMotionType(ROC_COLLISION_MOTION_DEFAULT + l_idx);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionGetMotionType(lua_State *f_vm)
{
    Collision *l_collision;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_collision);
    !argStream.HasErrors() ? argStream.PushText(g_collisionMotionTypesTable[l_collision->GetMotionType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int collisionAttach(lua_State *f_vm)
{
    Collision *l_col;
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->AttachCollisionToModel(l_col, l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int collisionDetach(lua_State *f_vm)
{
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->DetachCollision(l_col);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
