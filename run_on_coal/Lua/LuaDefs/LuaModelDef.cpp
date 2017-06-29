#include "stdafx.h"

#include "Lua/LuaDefs/LuaModelDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

#define ROC_MODEL_ANIMPROPERTY_SPEED 0
#define ROC_MODEL_ANIMPROPERTY_PROGRESS 1
#define ROC_MODEL_ANIMPROPERTY_BLENDFACTOR 2

namespace ROC
{

const std::vector<std::string> g_AnimationPropertiesTable
{
    "speed", "progress", "blendFactor"
};

}

void ROC::LuaModelDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Model", ModelCreate);
    Utils::Lua::lua_registerClassMethod(f_vm, "getGeometry", ModelGetGeometry);
    Utils::Lua::lua_registerClassMethod(f_vm, "setPosition", ModelSetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "getPosition", ModelGetPosition);
    Utils::Lua::lua_registerClassMethod(f_vm, "setRotation", ModelSetRotation);
    Utils::Lua::lua_registerClassMethod(f_vm, "getRotation", ModelGetRotation);
    Utils::Lua::lua_registerClassMethod(f_vm, "setScale", ModelSetScale);
    Utils::Lua::lua_registerClassMethod(f_vm, "getScale", ModelGetScale);
    Utils::Lua::lua_registerClassMethod(f_vm, "draw", ModelDraw);
    Utils::Lua::lua_registerClassMethod(f_vm, "attach", ModelAttach);
    Utils::Lua::lua_registerClassMethod(f_vm, "detach", ModelDetach);
    Utils::Lua::lua_registerClassMethod(f_vm, "getParent", ModelGetParent);
    Utils::Lua::lua_registerClassMethod(f_vm, "setAnimation", ModelSetAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "getAnimation", ModelGetAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "removeAnimation", ModelRemoveAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "playAnimation", ModelPlayAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "pauseAnimation", ModelPauseAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "resetAnimation", ModelResetAnimation);
    Utils::Lua::lua_registerClassMethod(f_vm, "setAnimationProperty", ModelSetAnimationProperty);
    Utils::Lua::lua_registerClassMethod(f_vm, "getAnimationProperty", ModelGetAnimationProperty);
    Utils::Lua::lua_registerClassMethod(f_vm, "getCollision", ModelGetCollision);
    Utils::Lua::lua_registerClassMethod(f_vm, "setCollidable", ModelSetCollidable);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaModelDef::ModelCreate(lua_State *f_vm)
{
    Geometry *l_geometry = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_geometry);
    if(!argStream.HasErrors())
    {
        Model *l_model = LuaManager::GetCore()->GetElementManager()->CreateModel(l_geometry);
        l_model ? argStream.PushElement(l_model) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetGeometry(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Geometry *l_geometry = l_model->GetGeometry();
        l_geometry ? argStream.PushElement(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelSetPosition(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_pos;
    bool l_preserveMotion = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    argStream.ReadNextBoolean(l_preserveMotion);
    if(!argStream.HasErrors())
    {
        l_model->SetPosition(l_pos, l_preserveMotion);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetPosition(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
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
int ROC::LuaModelDef::ModelSetRotation(lua_State *f_vm)
{
    Model *l_model;
    glm::vec4 l_rot(0.f, 0.f, 0.f, std::nanf("0"));
    bool l_preserveMotion = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_rot[i]);
    argStream.ReadNextNumber(l_rot.w);
    argStream.ReadNextBoolean(l_preserveMotion);
    if(!argStream.HasErrors())
    {
        glm::quat l_qRot = std::isnan(l_rot.w) ? glm::quat(l_rot) : glm::quat(l_rot.w, l_rot.x, l_rot.y, l_rot.z);
        l_model->SetRotation(l_qRot, l_preserveMotion);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetRotation(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    bool l_quatReq = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadNextBoolean(l_global);
    argStream.ReadNextBoolean(l_quatReq);
    if(!argStream.HasErrors())
    {
        glm::quat l_qRot;
        l_model->GetRotation(l_qRot, l_global);
        if(l_quatReq)
        {
            argStream.PushNumber(l_qRot.x);
            argStream.PushNumber(l_qRot.y);
            argStream.PushNumber(l_qRot.z);
            argStream.PushNumber(l_qRot.w);
        }
        else
        {
            glm::vec3 l_euler = glm::eulerAngles(l_qRot);
            argStream.PushNumber(l_euler.x);
            argStream.PushNumber(l_euler.y);
            argStream.PushNumber(l_euler.z);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelSetScale(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_scale;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_scale[i]);
    if(!argStream.HasErrors())
    {
        l_model->SetScale(l_scale);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetScale(lua_State *f_vm)
{
    Model *l_model;
    bool l_global = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
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
int ROC::LuaModelDef::ModelDraw(lua_State *f_vm)
{
    Model *l_model;
    bool l_texturize = true;
    bool l_frustum = true;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadNextBoolean(l_texturize);
    argStream.ReadNextBoolean(l_frustum);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->Render(l_model, l_frustum, l_texturize);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelAttach(lua_State *f_vm)
{
    Model *l_model, *l_parent;
    int l_bone = -1;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_parent);
    argStream.ReadNextInteger(l_bone);
    if(!argStream.HasErrors())
    {
        if(l_bone < -1) l_bone = -1;
        bool l_result = LuaManager::GetCore()->GetInheritManager()->AttachModelToModel(l_model, l_parent, l_bone);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelDetach(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->DetachModel(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetParent(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Model *l_parent = l_model->GetParent();
        l_parent ? argStream.PushElement(l_parent) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::ModelSetAnimation(lua_State *f_vm)
{
    Model *l_model;
    Animation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_anim);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->SetModelAnimation(l_model, l_anim);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Animation *l_anim = l_model->GetAnimation();
        l_anim ? argStream.PushElement(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelRemoveAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->RemoveModelAnimation(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelPlayAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PlayAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelPauseAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PauseAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelResetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->ResetAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelSetAnimationProperty(lua_State *f_vm)
{
    Model *l_model;
    std::string l_property;
    float l_value;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_property);
    argStream.ReadNumber(l_value);
    if(!argStream.HasErrors() && !l_property.empty())
    {
        bool l_result = false;
        switch(Utils::Enum::ReadEnumVector(g_AnimationPropertiesTable, l_property))
        {
            case ROC_MODEL_ANIMPROPERTY_SPEED:
                l_result = l_model->SetAnimationSpeed(l_value);
                break;
            case ROC_MODEL_ANIMPROPERTY_PROGRESS:
                l_result = l_model->SetAnimationProgress(l_value);
                break;
            case ROC_MODEL_ANIMPROPERTY_BLENDFACTOR:
                l_model->SetAnimationBlendFactor(l_value);
                break;
        }
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::ModelGetAnimationProperty(lua_State *f_vm)
{
    Model *l_model;
    std::string l_property;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_property);
    if(!argStream.HasErrors() && !l_property.empty())
    {
        float l_value = -1.f;
        switch(Utils::Enum::ReadEnumVector(g_AnimationPropertiesTable, l_property))
        {
            case ROC_MODEL_ANIMPROPERTY_SPEED:
                l_value = l_model->GetAnimationSpeed();
                break;
            case ROC_MODEL_ANIMPROPERTY_PROGRESS:
                l_value = l_model->GetAnimationProgress();
                break;
            case ROC_MODEL_ANIMPROPERTY_BLENDFACTOR:
                l_value = l_model->GetAnimationBlendFactor();
                break;
        }
        (l_value != -1.f) ? argStream.PushNumber(l_value) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::ModelGetCollision(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Collision *l_col = l_model->GetCollision();
        l_col ? argStream.PushElement(l_col) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::ModelSetCollidable(lua_State *f_vm)
{
    Model *l_model1, *l_model2;
    bool l_state;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model1);
    argStream.ReadElement(l_model2);
    argStream.ReadBoolean(l_state);
    if(!argStream.HasErrors())
    {
        bool l_result = PhysicsManager::SetModelsCollidable(l_model1, l_model2, l_state);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
