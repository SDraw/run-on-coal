#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager.h"
#include "Model/Animation.h"
#include "Model/Model.h"
#include "Model/Skeleton.h"
#include "Scene/RenderTarget.h"
#include "Scene/Scene.h"
#include "Scene/Shader.h"
#include "Scene/Texture.h"

ROC::InheritanceManager::InheritanceManager(Core *f_core)
{
    m_core = f_core;
}
ROC::InheritanceManager::~InheritanceManager()
{
    m_inheritMap.clear();
}

void ROC::InheritanceManager::RemoveInheritance(void *f_child,void *f_parent)
{
    auto iter = m_inheritMap.equal_range(f_child);
    for(auto iter1 = iter.first; iter1 != iter.second; iter1++)
    {
        if(iter1->second == f_parent) 
        {
            MemoryManager *l_memoryManager = m_core->GetMemoryManager();
            int l_cType = l_memoryManager->GetMemoryPointerType(f_child);
            int l_pType = l_memoryManager->GetMemoryPointerType(f_parent);
            if(l_cType != -1 && l_pType != -1) InheritanceBreakProcessing(f_child,f_parent,l_cType,l_pType);
            m_inheritMap.erase(iter1);
            break;
        }
    }
}
void ROC::InheritanceManager::RemoveChildRelation(void *f_child)
{
    auto iter = m_inheritMap.find(f_child);
    while(iter != m_inheritMap.end())
    {
        MemoryManager *l_memoryManager = m_core->GetMemoryManager();
        int l_cType = l_memoryManager->GetMemoryPointerType(f_child);
        int l_pType = l_memoryManager->GetMemoryPointerType(iter->second);
        if(l_cType != -1 && l_pType != -1) InheritanceBreakProcessing(iter->first,iter->second,l_cType,l_pType);
        m_inheritMap.erase(iter);
        iter = m_inheritMap.find(f_child);
    }
}
void ROC::InheritanceManager::RemoveParentRelation(void *f_parent)
{
    for(auto iter = m_inheritMap.begin(); iter != m_inheritMap.end();)
    {
        if(iter->second == f_parent)
        {
            MemoryManager *l_memoryManager = m_core->GetMemoryManager();
            int l_childType = l_memoryManager->GetMemoryPointerType(iter->first);
            int l_parentType = l_memoryManager->GetMemoryPointerType(iter->second);
            if(l_childType != -1 && l_parentType != -1) InheritanceBreakProcessing(iter->first,iter->second,l_childType,l_parentType);
            m_inheritMap.erase(iter++);
        }
        else ++iter;
    }
}

void ROC::InheritanceManager::InheritanceBreakProcessing(void *f_child, void *f_parent, unsigned char f_childType, unsigned char f_parentType)
{
    switch(f_childType)
    {
        case ElementType::ModelElement:
        {
            switch(f_parentType)
            {
                case ElementType::ModelElement:
                    static_cast<Model*>(f_child)->SetParent(NULL);
                    break;
                case ElementType::GeometryElement:
                    static_cast<Model*>(f_child)->SetGeometry(NULL);
                    break;
                case ElementType::AnimationElement:
                    static_cast<Model*>(f_child)->SetAnimation(NULL);
                    break;
            }
        } break;
        case ElementType::CameraElement:
        {
            switch(f_parentType)
            {
                case ElementType::SceneElement:
                    static_cast<Scene*>(f_parent)->SetCamera(NULL);
                    break;
            }
        } break;
        case ElementType::LightElement:
        {
            switch(f_parentType)
            {
                case ElementType::SceneElement:
                    static_cast<Scene*>(f_parent)->SetLight(NULL);
                    break;
            }
        } break;
        case ElementType::TextureElement:
        {
            switch(f_parentType)
            {
                case ElementType::ShaderElement:
                    m_core->GetRenderManager()->DettachFromShader(static_cast<Shader*>(f_parent),static_cast<Texture*>(f_child));
                    break;
            }
        } break;
        case ElementType::RenderTargetElement:
        {
            switch(f_parentType)
            {
                case ElementType::ShaderElement:
                    m_core->GetRenderManager()->DettachFromShader(static_cast<Shader*>(f_parent),static_cast<RenderTarget*>(f_child));
                    break;
            }
        } break;
    }
}

bool ROC::InheritanceManager::SetModelAnimation(Model *f_model, Animation *f_anim)
{
    if(!f_model->HasSkeleton()) return false;
    Animation *l_modelAnim = f_model->GetAnimation();
    if(l_modelAnim == f_anim) return true;
    if(f_model->m_skeleton->m_bonesCount != f_anim->m_bonesValue) return false;
    if(l_modelAnim) RemoveInheritance(f_model,l_modelAnim);
    f_model->SetAnimation(f_anim);
    m_inheritMap.insert(std::pair<void*,void*>(f_model,f_anim));
    return true;
}
bool ROC::InheritanceManager::AttachModelToModel(Model *f_model, Model *f_parent, int f_bone)
{
    if(f_model->IsRigid() || f_model == f_parent || f_model->GetParent()) return false;
    Model *l_treeParent = f_parent->GetParent();
    while(l_treeParent)
    {
        if(l_treeParent == f_model) return false;
        l_treeParent = l_treeParent->GetParent();
    }
    if(!f_parent->HasSkeleton()) f_bone = -1;
    else if(static_cast<int>(f_parent->m_skeleton->m_bonesCount) >= f_bone) f_bone = -1;
    f_model->SetParent(f_parent,f_bone);
    m_inheritMap.insert(std::pair<void*,void*>(f_model,f_parent));
    return true;
}
bool ROC::InheritanceManager::DettachModel(Model *f_model)
{
    Model *l_parent = f_model->GetParent();
    if(!l_parent) return false;
    RemoveInheritance(f_model,l_parent);
    return true;
}

bool ROC::InheritanceManager::SetSceneCamera(Scene *f_scene, Camera *f_camera)
{
    Camera *l_sceneCamera = f_scene->GetCamera();
    if(l_sceneCamera == f_camera) return true;
    if(l_sceneCamera) RemoveInheritance(l_sceneCamera,f_scene);
    RemoveChildRelation(f_camera);
    f_scene->SetCamera(f_camera);
    m_inheritMap.insert(std::pair<void*,void*>(f_camera,f_scene));
    return true;
}
bool ROC::InheritanceManager::SetSceneLight(Scene *f_scene, Light *f_light)
{
    Light *l_sceneLight = f_scene->GetLight();
    if(l_sceneLight == f_light) return true;
    if(l_sceneLight) RemoveInheritance(l_sceneLight,f_scene);
    RemoveChildRelation(f_light);
    f_scene->SetLight(f_light);
    m_inheritMap.insert(std::pair<void*,void*>(f_light,f_scene));
    return true;
}

bool ROC::InheritanceManager::AttachTextureToShader(Shader *f_shader, Texture *f_texture, int f_uniform)
{
    auto iter = m_inheritMap.equal_range(f_texture);
    for(auto iter1 = iter.first; iter1 != iter.second; iter1++)
    {
        if(iter1->second == f_shader) return false;
    }
    if(!m_core->GetRenderManager()->AttachToShader(f_shader,f_texture,f_uniform)) return false;
    m_inheritMap.insert(std::pair<void*,void*>(f_texture,f_shader));
    return true;
}
bool ROC::InheritanceManager::AttachRenderTargetToShader(Shader *f_shader, RenderTarget *f_target, int f_uniform)
{
    auto iter = m_inheritMap.equal_range(f_target);
    for(auto iter1 = iter.first; iter1 != iter.second; iter1++)
    {
        if(iter1->second == f_shader) return false;
    }
    if(!m_core->GetRenderManager()->AttachToShader(f_shader,f_target,f_uniform)) return false;
    m_inheritMap.insert(std::pair<void*,void*>(f_target,f_shader));
    return true;
}
