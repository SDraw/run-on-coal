#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager.h"
#include "Model/Model.h"
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

int modelCreate(lua_State *f_vm)
{
    Geometry *l_geometry;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_geometry), ElementType::GeometryElement);
    if(!argStream.HasErrors())
    {
        Model *l_model = LuaManager::m_corePointer->GetElementManager()->CreateModel(l_geometry);
        l_model ? lua_pushlightuserdata(f_vm, l_model) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelDestroy(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyModel(l_model);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetGeometry(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Geometry *l_geometry = l_model->GetGeometry();
        l_geometry ? lua_pushlightuserdata(f_vm, l_geometry) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetType(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        int l_type = l_model->GetType();
        (l_type > MODEL_TYPE_NONE) ? ((l_type == MODEL_TYPE_STATIC) ? lua_pushstring(f_vm, "static") : lua_pushstring(f_vm, "animated")) : lua_pushstring(f_vm, "none");
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetPosition(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_pos;
        l_model->GetPosition(l_pos, l_global);
        lua_pushnumber(f_vm, l_pos.x);
        lua_pushnumber(f_vm, l_pos.y);
        lua_pushnumber(f_vm, l_pos.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
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
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetRotation(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    bool l_quatReq = false;
    int l_returnVal = 1;
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
            lua_pushnumber(f_vm, l_vRot.x);
            lua_pushnumber(f_vm, l_vRot.y);
            lua_pushnumber(f_vm, l_vRot.z);
            l_returnVal = 3;
        }
        else
        {
            lua_pushnumber(f_vm, l_qRot.x);
            lua_pushnumber(f_vm, l_qRot.y);
            lua_pushnumber(f_vm, l_qRot.z);
            lua_pushnumber(f_vm, l_qRot.w);
            l_returnVal = 4;
        }
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
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
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetScale(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_scale;
        l_model->GetScale(l_scale, l_global);
        lua_pushnumber(f_vm, l_scale.x);
        lua_pushnumber(f_vm, l_scale.y);
        lua_pushnumber(f_vm, l_scale.z);
        l_returnVal = 3;
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
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
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        bool l_result = LuaManager::m_corePointer->GetCore()->GetInheritManager()->AttachModelToModel(l_model, l_parent, l_bone);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelDettach(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetInheritManager()->DettachModel(l_model);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetParent(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Model *l_parent = l_model->GetParent();
        l_parent ? lua_pushlightuserdata(f_vm, l_parent) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        Animation *l_anim = l_model->GetAnimation();
        l_anim ? lua_pushlightuserdata(f_vm, l_anim) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelPlayAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PlayAnimation();
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelPauseAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PauseAnimation();
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelResetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->ResetAnimation();
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetAnimationSpeed(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_speed = l_model->GetAnimationSpeed();
        (l_speed != -1.f) ? lua_pushnumber(f_vm, l_speed) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetAnimationProgress(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_progress = l_model->GetAnimationProgress();
        (l_progress != -1.f) ? lua_pushnumber(f_vm, l_progress) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int modelSetCollision(lua_State *f_vm)
{
    Model *l_model;
    std::string l_textType;
    float l_mass = 1.0;
    glm::vec3 l_size;
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
            lua_pushboolean(f_vm, l_result);
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelRemoveCollision(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetPhysicsManager()->RemoveModelCollision(l_model);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetMass(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_mass = l_model->GetMass();
        (l_mass != -1.f) ? lua_pushnumber(f_vm, l_mass) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetFriction(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        float l_friction = l_model->GetFriction();
        (l_friction != -1.f) ? lua_pushnumber(f_vm, l_friction) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetVelocity(lua_State *f_vm)
{
    Model *l_model;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        bool l_result = l_model->GetVelocity(l_velocity);
        if(l_result)
        {
            lua_pushnumber(f_vm, l_velocity.x);
            lua_pushnumber(f_vm, l_velocity.y);
            lua_pushnumber(f_vm, l_velocity.z);
            l_returnVal = 3;
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
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
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int modelGetAngularVelocity(lua_State *f_vm)
{
    Model *l_model;
    int l_returnVal = 1;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_model), ElementType::ModelElement);
    if(!argStream.HasErrors())
    {
        glm::vec3 l_velocity;
        bool l_result = l_model->GetAngularVelocity(l_velocity);
        if(l_result)
        {
            lua_pushnumber(f_vm, l_velocity.x);
            lua_pushnumber(f_vm, l_velocity.y);
            lua_pushnumber(f_vm, l_velocity.z);
            l_returnVal = 3;
        }
        else lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return l_returnVal;
}

}
}