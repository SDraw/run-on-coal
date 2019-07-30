#include "stdafx.h"

#include "Lua/LuaDefs/LuaModelDef.h"

#include "Module/LuaModule.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaCollidableDef.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_AnimationProperties
{
    "speed", "progress", "blendTime"
};

void LuaModelDef::Init(lua_State *f_vm)
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
    LuaUtils::AddClassMethod(f_vm, "attachTo", AttachTo);
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
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaCollidableDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaModelDef::Create(lua_State *f_vm)
{
    // element Model()
    // element Model(element geometry)
    ROC::IGeometry *l_geometry = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_geometry);
    if(!argStream.HasErrors())
    {
        ROC::IModel *l_model = LuaModule::GetModule()->GetEngineCore()->GetElementManager()->CreateModel(l_geometry);
        l_model ? argStream.PushElement(l_model) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaModelDef::GetGeometry(lua_State *f_vm)
{
    // element Model:getGeometry()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        ROC::IGeometry *l_geometry = l_model->GetGeometry();
        l_geometry ? argStream.PushElement(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaModelDef::SetPosition(lua_State *f_vm)
{
    // bool Model:setPosition(float x, float y, float z)
    ROC::IModel *l_model;
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
int LuaModelDef::GetPosition(lua_State *f_vm)
{
    // float float float Model:getPosition()
    ROC::IModel *l_model;
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

int LuaModelDef::SetRotation(lua_State *f_vm)
{
    // bool Model:setRotation(float x, float y, float z, float w)
    ROC::IModel *l_model;
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
int LuaModelDef::GetRotation(lua_State *f_vm)
{
    // float float float float Model:getRotation()
    ROC::IModel *l_model;
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

int LuaModelDef::SetScale(lua_State *f_vm)
{
    // bool Model:setScale(float x, float y, float z)
    ROC::IModel *l_model;
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
int LuaModelDef::GetScale(lua_State *f_vm)
{
    // float float float Model:getScale()
    ROC::IModel *l_model;
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

int LuaModelDef::GetMatrix(lua_State *f_vm)
{
    ROC::IModel *l_model;
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

int LuaModelDef::AttachTo(lua_State *f_vm)
{
    // bool Model:attachTo(element parentModel [, int boneIndex = -1 ])
    ROC::IModel *l_model, *l_parent;
    int l_bone = -1;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_parent);
    argStream.ReadNextInteger(l_bone);
    if(!argStream.HasErrors())
    {
        l_bone = std::max(-1, std::min(l_bone,std::numeric_limits<int>::max()));
        bool l_result = l_model->AttachTo(l_parent, l_bone);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::Detach(lua_State *f_vm)
{
    // bool Model:detach()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->Dettach();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::GetParent(lua_State *f_vm)
{
    // element Model:getParent()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        ROC::IModel *l_parent = l_model->GetParentModel();
        l_parent ? argStream.PushElement(l_parent) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaModelDef::SetAnimation(lua_State *f_vm)
{
    // bool Model:setAnimation(element animation)
    ROC::IModel *l_model;
    ROC::IAnimation *l_anim;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_anim);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetAnimation(l_anim);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::GetAnimation(lua_State *f_vm)
{
    // element Model:getAnimation()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        ROC::IAnimation *l_anim = l_model->GetAnimation();
        l_anim ? argStream.PushElement(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::RemoveAnimation(lua_State *f_vm)
{
    // bool Model:removeAnimation()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->RemoveAnimation();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::PlayAnimation(lua_State *f_vm)
{
    // bool Model:playAnimation()
    ROC::IModel *l_model;
    bool l_loop = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadNextBoolean(l_loop);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->PlayAnimation(l_loop);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::PauseAnimation(lua_State *f_vm)
{
    // bool Model:pauseAnimation()
    ROC::IModel *l_model;
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
int LuaModelDef::ResetAnimation(lua_State *f_vm)
{
    // bool Model:resetAnimation()
    ROC::IModel *l_model;
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
int LuaModelDef::SetAnimationProperty(lua_State *f_vm)
{
    // bool Model:setAnimationProperty(str property, float value)
    ROC::IModel *l_model;
    std::string l_propertyString;
    float l_value;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_propertyString);
    argStream.ReadNumber(l_value);
    if(!argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        int l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_AnimationProperties);
        if(l_enum != -1)
        {
            ROC::IModel::ModelAnimationProperty l_property = static_cast<ROC::IModel::ModelAnimationProperty>(l_enum);
            l_result = l_model->SetAnimationProperty(l_property, l_value);

        }
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::GetAnimationProperty(lua_State *f_vm)
{
    // float Model:getAnimationProperty(str property)
    ROC::IModel *l_model;
    std::string l_propertyString;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_propertyString);
    if(!argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        int l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_AnimationProperties);
        if(l_enum != -1)
        {
            float l_value = 0.f;
            ROC::IModel::ModelAnimationProperty l_property = static_cast<ROC::IModel::ModelAnimationProperty>(l_enum);
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

int LuaModelDef::GetCollision(lua_State *f_vm)
{
    // element Model:getCollision()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        ROC::ICollision *l_col = l_model->GetCollsion();
        l_col ? argStream.PushElement(l_col) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::SetCollision(lua_State *f_vm)
{
    // bool Model:setCollision(element col)
    ROC::IModel *l_model;
    ROC::ICollision *l_col;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadElement(l_col);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->SetCollision(l_col);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaModelDef::RemoveCollision(lua_State *f_vm)
{
    // bool Model:removeCollision()
    ROC::IModel *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        bool l_result = l_model->RemoveCollision();
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
