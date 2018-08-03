#include "stdafx.h"

#include "Managers/InheritanceManager.h"
#include "Core/Core.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/Drawable.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Light.h"
#include "Elements/Model/AnimationController.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader/Shader.h"

#include "Managers/ElementManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"

ROC::InheritanceManager::InheritanceManager(Core *f_core)
{
    m_core = f_core;
}
ROC::InheritanceManager::~InheritanceManager()
{
    m_inheritMap.clear();
}

void ROC::InheritanceManager::AddInheritance(Element *f_child, Element *f_parent)
{
    m_inheritMap.insert(std::make_pair(f_child, f_parent));
}
void ROC::InheritanceManager::RemoveInheritance(Element *f_child, Element *f_parent)
{
    auto iter = m_inheritMap.equal_range(f_child);
    for(auto iter1 = iter.first; iter1 != iter.second; iter1++)
    {
        if(iter1->second == f_parent)
        {
            InheritanceBreakProcessing(f_child, f_parent);
            m_inheritMap.erase(iter1);
            break;
        }
    }
}
void ROC::InheritanceManager::RemoveChildRelations(Element *f_child)
{
    auto iter = m_inheritMap.find(f_child);
    while(iter != m_inheritMap.end())
    {
        InheritanceBreakProcessing(iter->first, iter->second);
        m_inheritMap.erase(iter);
        iter = m_inheritMap.find(f_child);
    }
}
void ROC::InheritanceManager::RemoveParentRelations(Element *f_parent)
{
    for(auto iter = m_inheritMap.begin(); iter != m_inheritMap.end();)
    {
        if(iter->second == f_parent)
        {
            InheritanceBreakProcessing(iter->first, iter->second);
            m_inheritMap.erase(iter++);
        }
        else ++iter;
    }
}

void ROC::InheritanceManager::InheritanceBreakProcessing(Element *f_child, Element *f_parent)
{
    switch(f_child->GetElementType())
    {
        case Element::ET_Model:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Model:
                    reinterpret_cast<Model*>(f_child)->SetParent(nullptr);
                    break;
                case Element::ET_Geometry:
                    reinterpret_cast<Model*>(f_child)->SetGeometry(nullptr);
                    break;
                case Element::ET_Animation:
                    reinterpret_cast<Model*>(f_child)->GetAnimationController()->SetAnimation(nullptr);
                    break;
                case Element::ET_Collision:
                    reinterpret_cast<Model*>(f_child)->SetCollision(nullptr);
                    break;
            }
        } break;
        case Element::ET_Camera:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Scene:
                    reinterpret_cast<Scene*>(f_parent)->SetCamera(nullptr);
                    break;
            }
        } break;
        case Element::ET_Light:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Scene:
                    reinterpret_cast<Scene*>(f_parent)->SetLight(nullptr);
                    break;
            }
        } break;
        case Element::ET_Texture:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Shader:
                    reinterpret_cast<Shader*>(f_parent)->Detach(reinterpret_cast<Drawable*>(f_child));
                    break;
            }
        } break;
        case Element::ET_RenderTarget:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Scene:
                    reinterpret_cast<Scene*>(f_parent)->SetRenderTarget(nullptr);
                    break;
                case Element::ET_Shader:
                    reinterpret_cast<Shader*>(f_parent)->Detach(reinterpret_cast<Drawable*>(f_child));
                    break;
            }
        } break;
        case Element::ET_Shader:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ET_Scene:
                    reinterpret_cast<Scene*>(f_parent)->SetShader(nullptr);
                    break;
            }
        } break;
    }
}

void ROC::InheritanceManager::SetModelGeometry(Model *f_model, Geometry *f_geometry)
{
    AddInheritance(f_model, f_geometry);
}
bool ROC::InheritanceManager::SetModelAnimation(Model *f_model, Animation *f_anim)
{
    bool l_result = false;
    if(f_model->HasSkeleton())
    {
        Animation *l_modelAnim = f_model->GetAnimationController()->GetAnimation();
        if(l_modelAnim == f_anim) l_result = true;
        else
        {
            if(f_model->GetSkeleton()->GetBonesCount() == f_anim->GetBonesCount())
            {
                if(l_modelAnim) RemoveInheritance(f_model, l_modelAnim);
                AddInheritance(f_model, f_anim);
                f_model->GetAnimationController()->SetAnimation(f_anim);
                l_result = true;
            }
        }
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveModelAnimation(Model *f_model)
{
    bool l_result = false;
    Animation *l_anim = f_model->GetAnimationController()->GetAnimation();
    if(l_anim)
    {
        RemoveInheritance(f_model, l_anim);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::AttachModelToModel(Model *f_model, Model *f_parent, int f_bone)
{
    bool l_result = false;
    if(!f_model->HasCollision() && (f_model != f_parent) && !f_model->GetParent())
    {
        bool l_treeCheck = true;
        Model *l_treeParent = f_parent->GetParent();
        while(l_treeParent)
        {
            if(l_treeParent == f_model)
            {
                l_treeCheck = false;
                break;
            }
            l_treeParent = l_treeParent->GetParent();
        }
        if(l_treeCheck)
        {
            if(!f_parent->HasSkeleton()) f_bone = -1;
            else if(f_bone >= static_cast<int>(f_parent->GetSkeleton()->GetBonesCount())) f_bone = -1;

            AddInheritance(f_model, f_parent);
            m_core->GetPreRenderManager()->AddLink(f_model, f_parent);
            f_model->SetParent(f_parent, f_bone);
            l_result = true;
        }
    }
    return l_result;
}
bool ROC::InheritanceManager::DetachModel(Model *f_model)
{
    bool l_result = false;
    Model *l_parent = f_model->GetParent();
    if(l_parent)
    {
        RemoveInheritance(f_model, l_parent);
        m_core->GetPreRenderManager()->RemoveLink(f_model);
        l_result = true;
    }
    return l_result;
}

bool ROC::InheritanceManager::SetModelCollision(Model *f_model, Collision *f_col)
{
    bool l_result = false;
    if(!f_model->HasCollision() && !f_model->GetParent())
    {
        AddInheritance(f_model, f_col);
        f_model->SetCollision(f_col);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveModelCollision(Model *f_model)
{
    bool l_result = false;
    Collision *l_col = f_model->GetCollsion();
    if(l_col)
    {
        RemoveInheritance(f_model, l_col);
        l_result = true;
    }
    return l_result;
}

bool ROC::InheritanceManager::SetSceneCamera(Scene *f_scene, Camera *f_camera)
{
    bool l_result = false;
    Camera *l_sceneCamera = f_scene->GetCamera();
    if(l_sceneCamera != f_camera)
    {
        if(l_sceneCamera) RemoveInheritance(l_sceneCamera, f_scene);
        AddInheritance(f_camera, f_scene);
        f_scene->SetCamera(f_camera);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveSceneCamera(Scene *f_scene)
{
    bool l_result = false;
    Camera *l_camera = f_scene->GetCamera();
    if(l_camera)
    {
        RemoveInheritance(l_camera, f_scene);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::SetSceneLight(Scene *f_scene, Light *f_light)
{
    bool l_result = false;
    Light *l_sceneLight = f_scene->GetLight();
    if(l_sceneLight != f_light)
    {
        if(l_sceneLight) RemoveInheritance(l_sceneLight, f_scene);
        AddInheritance(f_light, f_scene);
        f_scene->SetLight(f_light);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveSceneLight(Scene *f_scene)
{
    bool l_result = false;
    Light *l_light = f_scene->GetLight();
    if(l_light)
    {
        RemoveInheritance(l_light, f_scene);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::SetSceneRenderTarget(Scene *f_scene, RenderTarget *f_rt)
{
    bool l_result = false;
    RenderTarget *l_rt = f_scene->GetRenderTarget();
    if(l_rt != f_rt)
    {
        if(l_rt) RemoveInheritance(l_rt, f_scene);
        AddInheritance(f_rt, f_scene);
        f_scene->SetRenderTarget(f_rt);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveSceneRenderTarget(Scene *f_scene)
{
    bool l_result = false;
    RenderTarget *l_rt = f_scene->GetRenderTarget();
    if(l_rt)
    {
        RemoveInheritance(l_rt, f_scene);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::SetSceneShader(Scene *f_scene, Shader *f_shader)
{
    bool l_result = false;
    Shader *l_shader = f_scene->GetShader();
    if(l_shader != f_shader)
    {
        if(l_shader) RemoveInheritance(l_shader, f_scene);
        AddInheritance(f_shader, f_scene);
        f_scene->SetShader(f_shader);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveSceneShader(Scene *f_scene)
{
    bool l_result = false;
    Shader *l_shader = f_scene->GetShader();
    if(l_shader)
    {
        RemoveInheritance(l_shader, f_scene);
        l_result = true;
    }
    return l_result;
}

bool ROC::InheritanceManager::AttachDrawableToShader(Shader *f_shader, Drawable *f_drawable, const std::string &f_uniform)
{
    bool l_result = false;
    if(!f_shader->HasAttached(f_drawable))
    {
        l_result = f_shader->Attach(f_drawable, f_uniform);
        if(l_result) AddInheritance(f_drawable, f_shader);
    }
    return l_result;
}
bool ROC::InheritanceManager::DetachDrawableFromShader(Shader *f_shader, Drawable *f_drawable)
{
    bool l_result = false;
    if(f_shader->HasAttached(f_drawable))
    {
        RemoveInheritance(f_drawable, f_shader);
        l_result = true;
    }
    return l_result;
}
