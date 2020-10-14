#include "stdafx.h"

#include "Lua/LuaDefs/LuaModelDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaCollidableDef.h"
#include "Core/Core.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_animationProperties
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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextElement(l_geometry);
    if(!l_argStream.HasErrors())
    {
        ROC::IModel *l_model = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateIModel(l_geometry);
        l_model ? l_argStream.PushElement(l_model) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetGeometry(lua_State *f_vm)
{
    // element Model:getGeometry()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        ROC::IGeometry *l_geometry = l_model->GetIGeometry();
        l_geometry ? l_argStream.PushElement(l_geometry) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetPosition(lua_State *f_vm)
{
    // bool Model:setPosition(float x, float y, float z)
    ROC::IModel *l_model;
    glm::vec3 l_pos;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    if(!l_argStream.HasErrors())
    {
        l_model->SetPosition(l_pos);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetPosition(lua_State *f_vm)
{
    // float float float Model:getPosition()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3& l_pos = l_model->GetPosition();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_pos[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetRotation(lua_State *f_vm)
{
    // bool Model:setRotation(float x, float y, float z, float w)
    ROC::IModel *l_model;
    glm::quat l_rot;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_rot[i]);
    if(!l_argStream.HasErrors())
    {
        l_model->SetRotation(l_rot);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetRotation(lua_State *f_vm)
{
    // float float float float Model:getRotation()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        const glm::quat& l_rotation = l_model->GetRotation();
        for(int i = 0; i < 4; i++) l_argStream.PushNumber(l_rotation[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetScale(lua_State *f_vm)
{
    // bool Model:setScale(float x, float y, float z)
    ROC::IModel *l_model;
    glm::vec3 l_scale;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_scale[i]);
    if(!l_argStream.HasErrors())
    {
        l_model->SetScale(l_scale);
        l_argStream.PushBoolean(true);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetScale(lua_State *f_vm)
{
    // float float float Model:getScale()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        const glm::vec3& l_scale = l_model->GetScale();
        for(int i = 0; i < 3; i++) l_argStream.PushNumber(l_scale[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetMatrix(lua_State *f_vm)
{
    // float float float float float float float float float float float float float float float float Model:getMatrix()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        const glm::mat4 &l_mat = l_model->GetFullMatrix();
        const float *l_buffer = glm::value_ptr(l_mat);
        for(size_t i = 0U; i < 16; i++) l_argStream.PushNumber(l_buffer[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::AttachTo(lua_State *f_vm)
{
    // bool Model:attachTo(element parentModel [, int boneIndex = -1 ])
    ROC::IModel *l_model, *l_parent;
    int l_bone = -1;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadElement(l_parent);
    l_argStream.ReadNextInteger(l_bone);
    if(!l_argStream.HasErrors())
    {
        l_bone = std::max(-1, std::min(l_bone, std::numeric_limits<int>::max()));
        bool l_result = l_model->AttachTo(l_parent, l_bone);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::Detach(lua_State *f_vm)
{
    // bool Model:detach()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->Dettach();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetParent(lua_State *f_vm)
{
    // element Model:getParent()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        ROC::IModel *l_parent = l_model->GetParentIModel();
        l_parent ? l_argStream.PushElement(l_parent) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetAnimation(lua_State *f_vm)
{
    // bool Model:setAnimation(element animation)
    ROC::IModel *l_model;
    ROC::IAnimation *l_anim;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadElement(l_anim);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->SetIAnimation(l_anim);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetAnimation(lua_State *f_vm)
{
    // element Model:getAnimation()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        ROC::IAnimation *l_anim = l_model->GetIAnimation();
        l_anim ? l_argStream.PushElement(l_anim) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::RemoveAnimation(lua_State *f_vm)
{
    // bool Model:removeAnimation()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->RemoveAnimation();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::PlayAnimation(lua_State *f_vm)
{
    // bool Model:playAnimation()
    ROC::IModel *l_model;
    bool l_loop = false;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadNextBoolean(l_loop);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->PlayAnimation(l_loop);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::PauseAnimation(lua_State *f_vm)
{
    // bool Model:pauseAnimation()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->PauseAnimation();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::ResetAnimation(lua_State *f_vm)
{
    // bool Model:resetAnimation()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->ResetAnimation();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetAnimationProperty(lua_State *f_vm)
{
    // bool Model:setAnimationProperty(str property, float value)
    ROC::IModel *l_model;
    std::string l_propertyString;
    float l_value;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadText(l_propertyString);
    l_argStream.ReadNumber(l_value);
    if(!l_argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        size_t l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_animationProperties);
        if(l_enum != std::numeric_limits<size_t>::max()) l_result = l_model->SetAnimationProperty(static_cast<ROC::IModel::ModelAnimationProperty>(l_enum), l_value);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetAnimationProperty(lua_State *f_vm)
{
    // float Model:getAnimationProperty(str property)
    ROC::IModel *l_model;
    std::string l_propertyString;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadText(l_propertyString);
    if(!l_argStream.HasErrors() && !l_propertyString.empty())
    {
        bool l_result = false;
        size_t l_enum = EnumUtils::ReadEnumVector(l_propertyString, g_animationProperties);
        if(l_enum != std::numeric_limits<size_t>::max())
        {
            float l_value = 0.f;
            if(l_model->GetAnimationProperty(static_cast<ROC::IModel::ModelAnimationProperty>(l_enum), l_value))
            {
                l_argStream.PushNumber(l_value);
                l_result = true;
            }
        }
        if(!l_result) l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::GetCollision(lua_State *f_vm)
{
    // element Model:getCollision()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        ROC::ICollision *l_col = l_model->GetICollsion();
        l_col ? l_argStream.PushElement(l_col) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::SetCollision(lua_State *f_vm)
{
    // bool Model:setCollision(element col)
    ROC::IModel *l_model;
    ROC::ICollision *l_col;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    l_argStream.ReadElement(l_col);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->SetICollision(l_col);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaModelDef::RemoveCollision(lua_State *f_vm)
{
    // bool Model:removeCollision()
    ROC::IModel *l_model;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_model);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_model->RemoveCollision();
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
