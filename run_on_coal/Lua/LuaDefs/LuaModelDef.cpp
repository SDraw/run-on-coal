#include "stdafx.h"

#include "Lua/LuaDefs/LuaModelDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Collision.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/AnimationController.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"
#include "Utils/MathUtils.h"

namespace ROC
{

const std::vector<std::string> g_AnimationPropertiesTable
{
    "speed", "progress", "blendTime"
};

}

void ROC::LuaModelDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Model", Create);
    LuaUtils::AddClassMethod(f_vm, "getGeometry", GetGeometry);
    LuaUtils::AddClassMethod(f_vm, "setPosition", SetPosition);
    LuaUtils::AddClassMethod(f_vm, "getPosition", GetPosition);
    LuaUtils::AddClassMethod(f_vm, "setRotation", SetRotation);
    LuaUtils::AddClassMethod(f_vm, "getRotation", GetRotation);
    LuaUtils::AddClassMethod(f_vm, "setScale", SetScale);
    LuaUtils::AddClassMethod(f_vm, "getScale", GetScale);
    LuaUtils::AddClassMethod(f_vm, "getMatrix", GetMatrix);
    LuaUtils::AddClassMethod(f_vm, "attach", Attach);
    LuaUtils::AddClassMethod(f_vm, "detach", Detach);
    LuaUtils::AddClassMethod(f_vm, "getParent", GetParent);
    LuaUtils::AddClassMethod(f_vm, "setAnimation", SetAnimation);
    LuaUtils::AddClassMethod(f_vm, "getAnimation", GetAnimation);
    LuaUtils::AddClassMethod(f_vm, "removeAnimation", RemoveAnimation);
    LuaUtils::AddClassMethod(f_vm, "playAnimation", PlayAnimation);
    LuaUtils::AddClassMethod(f_vm, "pauseAnimation", PauseAnimation);
    LuaUtils::AddClassMethod(f_vm, "resetAnimation", ResetAnimation);
    LuaUtils::AddClassMethod(f_vm, "setAnimationProperty", SetAnimationProperty);
    LuaUtils::AddClassMethod(f_vm, "getAnimationProperty", GetAnimationProperty);
    LuaUtils::AddClassMethod(f_vm, "setCollision", SetCollision);
    LuaUtils::AddClassMethod(f_vm, "getCollision", GetCollision);
    LuaUtils::AddClassMethod(f_vm, "removeCollision", RemoveCollision);
    LuaUtils::AddClassMethod(f_vm, "setCollidable", SetCollidable);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaModelDef::Create(lua_State *f_vm)
{
    // element Model()
    // element Model(element geometry)
    Geometry *l_geometry = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_geometry);
    if(!argStream.HasErrors())
    {
        Model *l_model = Core::GetCore()->GetElementManager()->CreateModel(l_geometry);
        l_model ? argStream.PushElement(l_model) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::GetGeometry(lua_State *f_vm)
{
    // element Model:getGeometry()
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

int ROC::LuaModelDef::SetPosition(lua_State *f_vm)
{
    // bool Model:setPosition(float x, float y, float z)
    Model *l_model;
    glm::vec3 l_pos;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    if(!argStream.HasErrors())
    {
        l_model->SetPosition(l_pos);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::GetPosition(lua_State *f_vm)
{
    // float float float Model:getPosition()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        const glm::vec3& l_pos = l_model->GetPosition();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_pos[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::SetRotation(lua_State *f_vm)
{
    // bool Model:setRotation(float x, float y, float z, float w)
    Model *l_model;
    glm::quat l_rot;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_rot[i]);
    if(!argStream.HasErrors())
    {
        l_model->SetRotation(l_rot);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::GetRotation(lua_State *f_vm)
{
    // float float float float Model:getRotation()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        const glm::quat& l_rotation = l_model->GetRotation();
        for(int i = 0; i < 4; i++) argStream.PushNumber(l_rotation[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::SetScale(lua_State *f_vm)
{
    // bool Model:setScale(float x, float y, float z)
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
int ROC::LuaModelDef::GetScale(lua_State *f_vm)
{
    // float float float Model:getScale()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        const glm::vec3& l_scale = l_model->GetScale();
        for(int i = 0; i < 3; i++) argStream.PushNumber(l_scale[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::GetMatrix(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_model->GetFullMatrix();
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++) argStream.PushNumber(l_mat[i][j]);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::Attach(lua_State *f_vm)
{
    // bool Model:attach(element parentModel [, int boneIndex = -1 ])
    Model *l_model, *l_parent;
    int l_bone = -1;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_parent);
    argStream.ReadNextInteger(l_bone);
    if(!argStream.HasErrors())
    {
        btClamp(l_bone, -1, std::numeric_limits<int>::max());
        bool l_result = Core::GetCore()->GetInheritManager()->AttachModelToModel(l_model, l_parent, l_bone);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::Detach(lua_State *f_vm)
{
    // bool Model:detach()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->DetachModel(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::GetParent(lua_State *f_vm)
{
    // element Model:getParent()
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

int ROC::LuaModelDef::SetAnimation(lua_State *f_vm)
{
    // bool Model:setAnimation(element animation)
    Model *l_model;
    Animation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_anim);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetModelAnimation(l_model, l_anim);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::GetAnimation(lua_State *f_vm)
{
    // element Model:getAnimation()
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
int ROC::LuaModelDef::RemoveAnimation(lua_State *f_vm)
{
    // bool Model:removeAnimation()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveModelAnimation(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::PlayAnimation(lua_State *f_vm)
{
    // bool Model:playAnimation()
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
int ROC::LuaModelDef::PauseAnimation(lua_State *f_vm)
{
    // bool Model:pauseAnimation()
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
int ROC::LuaModelDef::ResetAnimation(lua_State *f_vm)
{
    // bool Model:resetAnimation()
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
int ROC::LuaModelDef::SetAnimationProperty(lua_State *f_vm)
{
    // bool Model:setAnimationProperty(str property, float value)
    Model *l_model;
    std::string l_propertyString;
    float l_value;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_propertyString);
    argStream.ReadNumber(l_value);
    if(!argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        int l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_AnimationPropertiesTable);
        if(l_enum != -1)
        {
            Model::ModelAnimationProperty l_property = static_cast<Model::ModelAnimationProperty>(l_enum);
            l_result = l_model->SetAnimationProperty(l_property, l_value);

        }
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::GetAnimationProperty(lua_State *f_vm)
{
    // float Model:getAnimationProperty(str property)
    Model *l_model;
    std::string l_propertyString;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_propertyString);
    if(!argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        int l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_AnimationPropertiesTable);
        if(l_enum != -1)
        {
            float l_value = 0.f;
            Model::ModelAnimationProperty l_property = static_cast<Model::ModelAnimationProperty>(l_enum);
            if(l_model->GetAnimationProperty(l_property, l_value))
            {
                argStream.PushNumber(l_value);
                l_result = true;
            }
        }
        if(!l_result) argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::GetCollision(lua_State *f_vm)
{
    // element Model:getCollision()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Collision *l_col = l_model->GetCollsion();
        l_col ? argStream.PushElement(l_col) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::SetCollision(lua_State *f_vm)
{
    // bool Model:setCollision(element col)
    Model *l_model;
    Collision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->SetModelCollision(l_model, l_col);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaModelDef::RemoveCollision(lua_State *f_vm)
{
    // bool Model:removeCollision()
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->RemoveModelCollision(l_model);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaModelDef::SetCollidable(lua_State *f_vm)
{
    // bool Model:setCollidable(element model, bool state)
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
