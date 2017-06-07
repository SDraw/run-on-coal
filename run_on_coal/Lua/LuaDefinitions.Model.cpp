#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Model.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Model.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_modelAnimationPropertiesTable
{
    "speed", "progress"
};

int modelCreate(lua_State *f_vm)
{
    Geometry *l_geometry = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_geometry);
    if(!argStream.HasErrors())
    {
        Model *l_model = LuaManager::GetCore()->GetElementManager()->CreateModel(l_geometry);
        l_model ? argStream.PushPointer(l_model) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetGeometry(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Geometry *l_geometry = l_model->GetGeometry();
        l_geometry ? argStream.PushPointer(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelSetPosition(lua_State *f_vm)
{
    Model *l_model;
    glm::vec3 l_pos;
    bool l_saveMotion = false;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    argStream.ReadNextBoolean(l_saveMotion);
    if(!argStream.HasErrors())
    {
        l_model->SetPosition(l_pos,l_saveMotion);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetPosition(lua_State *f_vm)
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
int modelSetRotation(lua_State *f_vm)
{
    Model *l_model;
    glm::vec4 l_rot(0.f, 0.f, 0.f, std::nanf("0"));
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
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
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
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
int modelGetScale(lua_State *f_vm)
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
int modelDraw(lua_State *f_vm)
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
int modelAttach(lua_State *f_vm)
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
int modelDetach(lua_State *f_vm)
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
int modelGetParent(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
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
int modelGetAnimation(lua_State *f_vm)
{
    Model *l_model;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Animation *l_anim = l_model->GetAnimation();
        l_anim ? argStream.PushPointer(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelRemoveAnimation(lua_State *f_vm)
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
int modelPlayAnimation(lua_State *f_vm)
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
int modelPauseAnimation(lua_State *f_vm)
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
int modelResetAnimation(lua_State *f_vm)
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
int modelSetAnimationProperty(lua_State *f_vm)
{
    Model *l_model = NULL;
    std::string l_property;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_property);
    if(!argStream.HasErrors())
    {
        if(l_model->GetAnimation() != NULL)
        {
            switch(Utils::ReadEnumVector(g_modelAnimationPropertiesTable, l_property))
            {
                case 0: // Speed
                {
                    float l_speed;
                    argStream.ReadNumber(l_speed);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_model->SetAnimationSpeed(l_speed);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case 1: // Progress
                {
                    float l_progress;
                    argStream.ReadNumber(l_progress);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_model->SetAnimationProgress(l_progress);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                default:
                    argStream.PushBoolean(false);
            }
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int modelGetAnimationProperty(lua_State *f_vm)
{
    Model *l_model;
    std::string l_property;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    argStream.ReadText(l_property);
    if(!argStream.HasErrors())
    {
        if(l_model->GetAnimation() != NULL)
        {
            switch(Utils::ReadEnumVector(g_modelAnimationPropertiesTable, l_property))
            {
                case 0: // Speed
                {
                    float l_speed = l_model->GetAnimationSpeed();
                    argStream.PushNumber(l_speed);
                } break;
                case 1: // Progress
                {
                    float l_progress = l_model->GetAnimationProgress();
                    argStream.PushNumber(l_progress);
                } break;
                default:
                    argStream.PushBoolean(false);
            }
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int modelGetCollision(lua_State *f_vm)
{
    Model *l_model = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_model);
    if(!argStream.HasErrors())
    {
        Collision *l_col = l_model->GetCollision();
        l_col ? argStream.PushPointer(l_col) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
