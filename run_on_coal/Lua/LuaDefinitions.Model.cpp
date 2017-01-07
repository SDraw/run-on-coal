#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Model.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_modelCollisionTable
{
    "sphere", "box", "cylinder", "capsule", "cone"
};
const std::vector<std::string> g_modelTypesTable
{
    "none", "static", "animated"
};

int modelCreate(lua_State *f_vm)
{
    Geometry *l_geometry;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_geometry), ElementType::GeometryElement);
    if(!argStream.HasErrors())
    {
        Model *l_model = LuaManager::m_corePointer->GetElementManager()->CreateModel(l_geometry);
        l_model ? argStream.PushPointer(l_model) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelDestroy(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyModel(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetGeometry(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Geometry *l_geometry = l_model->GetGeometry();
        l_geometry ? argStream.PushPointer(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetType(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    !argStream.HasErrors() ? argStream.PushText(g_modelTypesTable[l_model->GetType()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetPosition(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_model->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetPosition(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_model->GetPosition(l_pos, l_global);
        argStream.PushNumber(l_pos.x);
        argStream.PushNumber(l_pos.y);
        argStream.PushNumber(l_pos.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetRotation(lua_State *f_vm)
{
    Model *l_model;
    glm::vec4 l_rot(0.f, 0.f, 0.f, std::nanf("0"));
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_rot[i]);
    argStream.ReadNextNumber(l_rot.w);
    if(!argStream.HasErrors())
    {
        glm::quat l_qRot = std::isnan(l_rot.w) ? glm::quat(l_rot) : glm::quat(l_rot.w, l_rot.x, l_rot.y, l_rot.z);
        l_model->SetRotation(l_qRot);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetRotation(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    bool l_quatReq = false;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    argStream.ReadNextBoolean(l_quatReq);
    if(!argStream.HasErrors())
    {
        glm::quat l_qRot;
        l_model->GetRotation(l_qRot, l_global);
        if(!l_quatReq)
        {
            glm::vec3 l_vRot = glm::eulerAngles(l_qRot);
            argStream.PushNumber(l_vRot.x);
            argStream.PushNumber(l_vRot.y);
            argStream.PushNumber(l_vRot.z);
        }
        else
        {
            argStream.PushNumber(l_qRot.x);
            argStream.PushNumber(l_qRot.y);
            argStream.PushNumber(l_qRot.z);
            argStream.PushNumber(l_qRot.w);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetScale(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_scale;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_scale[i]);
    if(!argStream.HasErrors())
    {
        l_model->SetScale(l_scale);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetScale(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_scale;
        l_model->GetScale(l_scale, l_global);
        argStream.PushNumber(l_scale.x);
        argStream.PushNumber(l_scale.y);
        argStream.PushNumber(l_scale.z);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelDraw(lua_State *f_vm)
{
    Model *l_model;
    bool l_texturize = true;
    bool l_frustum = false;
    float l_radius = 1.0;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_texturize);
    argStream.ReadNextBoolean(l_frustum);
    argStream.ReadNextNumber(l_radius);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetRenderManager()->Render(l_model, l_texturize, l_frustum, l_radius);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelAttach(lua_State *f_vm)
{
    Model *l_model, *l_parent;
    int l_bone = -1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_parent), ElementType::ModelElement);
    argStream.ReadNextInteger(l_bone);
    if(!argStream.HasErrors())
    {
        if(l_bone < -1) l_bone = -1;
        bool l_result = LuaManager::m_corePointer->GetInheritManager()->AttachModelToModel(l_model, l_parent, l_bone);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelDettach(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetInheritManager()->DettachModel(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetParent(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Model *l_parent = l_model->GetParent();
        l_parent ? argStream.PushPointer(l_parent) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int modelSetAnimation(lua_State *f_vm)
{
    Model *l_model;
    Animation *l_anim;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_anim), ElementType::AnimationElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetInheritManager()->SetModelAnimation(l_model, l_anim);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Animation *l_anim = l_model->GetAnimation();
        l_anim ? argStream.PushPointer(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelPlayAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PlayAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelPauseAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PauseAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelResetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->ResetAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetAnimationSpeed(lua_State *f_vm)
{
    Model *l_model = NULL;
    float l_speed;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNumber(l_speed);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetAnimationSpeed(l_speed);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetAnimationSpeed(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_speed = l_model->GetAnimationSpeed();
        (l_speed != -1.f) ? argStream.PushNumber(l_speed) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetAnimationProgress(lua_State *f_vm)
{
    Model *l_model;
    float l_progress;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNumber(l_progress);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetAnimationProgress(l_progress);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetAnimationProgress(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_progress = l_model->GetAnimationProgress();
        (l_progress != -1.f) ? argStream.PushNumber(l_progress) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int modelSetCollision(lua_State *f_vm)
{
    Model *l_model;
    std::string l_textType;
    float l_mass = 1.0;
    glm::vec3 l_size(1.f);
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadText(l_textType);
    argStream.ReadNextNumber(l_mass);
    for(int i = 0; i < 3; i++) argStream.ReadNextNumber(l_size[i]);
    if(!argStream.HasErrors() && !l_textType.empty() && (l_mass >= 0.f))
    {
        int l_type = Utils::ReadEnumVector(g_modelCollisionTable, l_textType);
        if(l_type != -1)
        {
            bool l_result = LuaManager::m_corePointer->GetPhysicsManager()->SetModelCollision(l_model, MODEL_RIGIDITY_TYPE_SPHERE + static_cast<unsigned char>(l_type), l_mass, l_size);
            argStream.PushBoolean(l_result);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelRemoveCollision(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetPhysicsManager()->RemoveModelCollision(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetMass(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_mass = l_model->GetMass();
        (l_mass != -1.f) ? argStream.PushNumber(l_mass) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetFriction(lua_State *f_vm)
{
    Model *l_model;
    float l_friction;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNumber(l_friction);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetFriction(l_friction);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetFriction(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_friction = l_model->GetFriction();
        (l_friction != -1.f) ? argStream.PushNumber(l_friction) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetVelocity(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_velocity;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_velocity[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetVelocity(l_velocity);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetVelocity(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        if(l_model->GetVelocity(l_velocity))
        {
            argStream.PushNumber(l_velocity.x);
            argStream.PushNumber(l_velocity.y);
            argStream.PushNumber(l_velocity.z);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetAngularVelocity(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_velocity;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_velocity[i]);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetAngularVelocity(l_velocity);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetAngularVelocity(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        l_model->GetAngularVelocity(l_velocity);
        if(l_model->GetAngularVelocity(l_velocity))
        {
            argStream.PushNumber(l_velocity.x);
            argStream.PushNumber(l_velocity.y);
            argStream.PushNumber(l_velocity.z);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
