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

int modelCreate(lua_State *f_vm)
{
    Geometry *l_geometry;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_geometry,ElementType::GeometryElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Model *l_model = LuaManager::m_corePointer->GetElementManager()->CreateModel(l_geometry);
    l_model ? lua_pushlightuserdata(f_vm,l_model) : lua_pushboolean(f_vm,0);
    return 1;
}
int modelDestroy(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyModel(l_model);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetGeometry(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Geometry *l_geometry = l_model->GetGeometry();
    l_geometry ? lua_pushlightuserdata(f_vm,l_geometry) : lua_pushboolean(f_vm,0);
    return 1;
}
int modelGetType(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = l_model->GetType();
    (l_type>0) ? ((l_type==1) ? lua_pushstring(f_vm,"static") : lua_pushstring(f_vm,"animated")) : lua_pushstring(f_vm,"none");
    return 1;
}
int modelSetPosition(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_pos[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_pos[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vPos(l_pos[0],l_pos[1],l_pos[2]);
    l_model->SetPosition(l_vPos);
    lua_pushboolean(f_vm,1);
    return 1;
}
int modelGetPosition(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vPos;
    l_model->GetPosition(l_vPos,l_global);
    lua_pushnumber(f_vm,l_vPos.x);
    lua_pushnumber(f_vm,l_vPos.y);
    lua_pushnumber(f_vm,l_vPos.z);
    return 3;
}
int modelSetRotation(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_rot[4];
    l_rot[3] = std::nan("0");
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_rot[i]);
    argStream.ReadNextNumber(l_rot[3]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    if(std::isnan(l_rot[3]))
    {
        glm::quat l_qRot(glm::vec3(l_rot[0],l_rot[1],l_rot[2]));
        l_model->SetRotation(l_qRot);
    }
    else
    {
        glm::quat l_qRot(static_cast<float>(l_rot[3]),static_cast<float>(l_rot[0]),static_cast<float>(l_rot[1]),static_cast<float>(l_rot[2]));
        l_model->SetRotation(l_qRot);
    }
    lua_pushboolean(f_vm,1);
    return 1;
}
int modelGetRotation(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    bool l_quatReq = false;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    argStream.ReadNextBoolean(l_quatReq);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_retVal;
    glm::quat l_qRot;
    l_model->GetRotation(l_qRot,l_global);
    if(!l_quatReq)
    {
        glm::vec3 l_vRot = glm::eulerAngles(l_qRot);
        lua_pushnumber(f_vm,l_vRot.x);
        lua_pushnumber(f_vm,l_vRot.y);
        lua_pushnumber(f_vm,l_vRot.z);
        l_retVal = 3;
    }
    else
    {
        lua_pushnumber(f_vm,l_qRot.x);
        lua_pushnumber(f_vm,l_qRot.y);
        lua_pushnumber(f_vm,l_qRot.z);
        lua_pushnumber(f_vm,l_qRot.w);
        l_retVal = 4;
    }
    return l_retVal;
}
int modelSetScale(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_scl[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_scl[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vScl(l_scl[0],l_scl[1],l_scl[2]);
    l_model->SetScale(l_vScl);
    lua_pushboolean(f_vm,1);
    return 1;
}
int modelGetScale(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNextBoolean(l_global);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vScl;
    l_model->GetScale(l_vScl,l_global);
    lua_pushnumber(f_vm,l_vScl.x);
    lua_pushnumber(f_vm,l_vScl.y);
    lua_pushnumber(f_vm,l_vScl.z);
    return 3;
}
int modelSetMatrixUpdate(lua_State *f_vm)
{
    Model *l_model;
    bool l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadBoolean(l_val);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    l_val ? LuaManager::m_corePointer->GetPreRenderManager()->AddModel(l_model) : LuaManager::m_corePointer->GetPreRenderManager()->RemoveModel(l_model);
    lua_pushboolean(f_vm,1);
    return 1;
}
int modelDraw(lua_State *f_vm)
{
    Model *l_model;
    bool l_texturize = true;
    bool l_frustum = false;
    lua_Number l_radius = 1.0;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNextBoolean(l_texturize);
    argStream.ReadNextBoolean(l_frustum);
    argStream.ReadNextNumber(l_radius);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    LuaManager::m_corePointer->GetRenderManager()->Render(l_model,l_texturize,l_frustum,static_cast<float>(l_radius));
    lua_pushboolean(f_vm,1);
    return 1;
}
int modelAttach(lua_State *f_vm)
{
    Model *l_model,*l_parent;
    LUA_INTEGER l_bone = -1;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadUserdata((void**)&l_parent,ElementType::ModelElement);
    argStream.ReadNextInteger(l_bone);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    if(l_bone<-1) l_bone = -1;
    bool result = LuaManager::m_corePointer->GetCore()->GetInheritManager()->AttachModelToModel(l_model,l_parent,static_cast<int>(l_bone));
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelDettach(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetInheritManager()->DettachModel(l_model);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetParent(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Model *l_parent = l_model->GetParent();
    l_parent ? lua_pushlightuserdata(f_vm,l_parent) : lua_pushboolean(f_vm,0);
    return 1;
}

int modelSetAnimation(lua_State *f_vm)
{
    Model *l_model;
    Animation *l_anim;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadUserdata((void**)&l_anim,ElementType::AnimationElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetInheritManager()->SetModelAnimation(l_model,l_anim);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Animation *l_anim = l_model->GetAnimation();
    l_anim ? lua_pushlightuserdata(f_vm,l_anim) : lua_pushboolean(f_vm,0);
    return 1;
}
int modelPlayAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_model->PlayAnimation();
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelPauseAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_model->PauseAnimation();
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelResetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_model->ResetAnimation();
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelSetAnimationSpeed(lua_State *f_vm)
{
    Model *l_model = NULL;
    lua_Number l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNumber(l_val);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_result = l_model->SetAnimationSpeed(static_cast<float>(l_val));
    lua_pushboolean(f_vm,l_result);
    return 1;
}
int modelGetAnimationSpeed(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_Number l_val = l_model->GetAnimationSpeed();
    lua_pushnumber(f_vm,l_val);
    return 1;
}
int modelSetAnimationProgress(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_val;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNumber(l_val);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool l_result = l_model->SetAnimationProgress(static_cast<float>(l_val));
    lua_pushboolean(f_vm,l_result);
    return 1;
}
int modelGetAnimationProgress(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_Number l_val = l_model->GetAnimationProgress();
    lua_pushnumber(f_vm,l_val);
    return 1;
}

int modelSetRigidity(lua_State *f_vm)
{
    Model *l_model;
    std::string l_textType;
    lua_Number l_mass = 1.0;
    lua_Number l_sizes[3] = {1.0,1.0,1.0};
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadText(l_textType);
    argStream.ReadNextNumber(l_mass);
    for(int i = 0; i<3; i++) argStream.ReadNextNumber(l_sizes[i]);
    if(argStream.HasErrors()||!l_textType.length()||l_mass<0.0)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    int l_type = Utils::ReadEnumString(l_textType,"sphere,box,cylinder,capsule,cone");
    if(l_type==-1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vSize(l_sizes[0],l_sizes[1],l_sizes[2]);
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->SetModelRigidity(l_model,MODEL_RIGIDITY_TYPE_SPHERE+static_cast<unsigned char>(l_type),static_cast<float>(l_mass),l_vSize);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelRemoveRigidity(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetPhysicsManager()->RemoveModelRigidity(l_model);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetMass(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_mass = l_model->GetMass();
    (l_mass==-1.f) ? lua_pushboolean(f_vm,0) : lua_pushnumber(f_vm,l_mass);
    return 1;
}
int modelSetFriction(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_friction;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    argStream.ReadNumber(l_friction);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = l_model->SetFriction(static_cast<float>(l_friction));
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetFriction(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    float l_friction = l_model->GetFriction();
    (l_friction==-1.f) ? lua_pushboolean(f_vm,0) : lua_pushnumber(f_vm,l_friction);
    return 1;
}
int modelSetVelocity(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_velocity[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_velocity[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vVelocity(l_velocity[0],l_velocity[1],l_velocity[2]);
    bool result = l_model->SetVelocity(l_vVelocity);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetVelocity(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_velocity;
    bool result = l_model->GetVelocity(l_velocity);
    if(!result)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_pushnumber(f_vm,l_velocity.x);
    lua_pushnumber(f_vm,l_velocity.y);
    lua_pushnumber(f_vm,l_velocity.z);
    return 3;
}
int modelSetAngularVelocity(lua_State *f_vm)
{
    Model *l_model;
    lua_Number l_velocity[3];
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    for(int i = 0; i<3; i++) argStream.ReadNumber(l_velocity[i]);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_vVelocity(l_velocity[0],l_velocity[1],l_velocity[2]);
    bool result = l_model->SetAngularVelocity(l_vVelocity);
    lua_pushboolean(f_vm,result);
    return 1;
}
int modelGetAngularVelocity(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_model,ElementType::ModelElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec3 l_velocity;
    bool result = l_model->GetAngularVelocity(l_velocity);
    if(!result)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    lua_pushnumber(f_vm,l_velocity.x);
    lua_pushnumber(f_vm,l_velocity.y);
    lua_pushnumber(f_vm,l_velocity.z);
    return 3;
}

}
}
