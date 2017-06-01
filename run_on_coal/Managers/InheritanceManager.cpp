#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"

#include "Elements/Animation/Animation.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/Drawable.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Elements/Scene.h"
#include "Elements/Shader.h"

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
        case Element::ElementType::ModelElement:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ElementType::ModelElement:
                    dynamic_cast<Model*>(f_child)->SetParent(NULL);
                    break;
                case Element::ElementType::GeometryElement:
                    dynamic_cast<Model*>(f_child)->SetGeometry(NULL);
                    break;
                case Element::ElementType::AnimationElement:
                    dynamic_cast<Model*>(f_child)->SetAnimation(NULL);
                    break;
            }
        } break;
        case Element::ElementType::CollisionElement:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ElementType::ModelElement:
                {
                    dynamic_cast<Collision*>(f_child)->SetParentModel(NULL);
                    dynamic_cast<Model*>(f_parent)->SetCollision(NULL);
                } break;
            }
        } break;
        case Element::ElementType::CameraElement:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ElementType::SceneElement:
                    dynamic_cast<Scene*>(f_parent)->SetCamera(NULL);
                    break;
            }
        } break;
        case Element::ElementType::LightElement:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ElementType::SceneElement:
                    dynamic_cast<Scene*>(f_parent)->SetLight(NULL);
                    break;
            }
        } break;
        case Element::ElementType::TextureElement: case Element::ElementType::RenderTargetElement: case Element::ElementType::MovieElement:
        {
            switch(f_parent->GetElementType())
            {
                case Element::ElementType::ShaderElement:
                    m_core->GetRenderManager()->DetachFromShader(dynamic_cast<Shader*>(f_parent), dynamic_cast<Drawable*>(f_child));
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
        Animation *l_modelAnim = f_model->GetAnimation();
        if(l_modelAnim == f_anim) l_result = true;
        if(!l_result)
        {
            if(f_model->GetSkeleton()->GetBonesCount() == f_anim->GetBonesCount())
            {
                if(l_modelAnim) RemoveInheritance(f_model, l_modelAnim);
                AddInheritance(f_model, f_anim);
                f_model->SetAnimation(f_anim);
                l_result = true;
            }
        }
    }
    return l_result;
}
bool ROC::InheritanceManager::RemoveModelAnimation(Model *f_model)
{
    bool l_result = false;
    if(f_model->HasAnimation())
    {
        RemoveInheritance(f_model, f_model->GetAnimation());
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

bool ROC::InheritanceManager::AttachCollisionToModel(Collision *f_col, Model *f_model)
{
    bool l_result = false;
    if(!f_col->GetParentModel() && !f_model->HasCollision() && !f_model->GetParent())
    {
        AddInheritance(f_col, f_model);
        f_col->SetParentModel(f_model);
        f_model->SetCollision(f_col);
        l_result = true;
    }
    return l_result;
}
bool ROC::InheritanceManager::DetachCollision(Collision *f_col)
{
    bool l_result = false;
    if(f_col->GetParentModel())
    {
        RemoveInheritance(f_col,f_col->GetParentModel());
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

bool ROC::InheritanceManager::AttachDrawableToShader(Shader *f_shader, Drawable *f_drawable, const std::string &f_uniform)
{
    bool l_result = false;
    if(!f_shader->HasAttached(f_drawable))
    {
        l_result = m_core->GetRenderManager()->AttachToShader(f_shader, f_drawable, f_uniform);
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
