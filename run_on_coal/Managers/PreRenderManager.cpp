#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/PhysicsManager.h"
#include "Model/Model.h"
#include "Lua/LuaArguments.h"

ROC::LuaArguments ROC::PreRenderManager::m_argument = ROC::LuaArguments();

ROC::PreRenderManager::PreRenderManager(Core *f_core)
{
    m_core = f_core;
}
ROC::PreRenderManager::~PreRenderManager()
{
    m_staticModelSet.clear();
    m_animatedModelSet.clear();
}

void ROC::PreRenderManager::AddModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        if(m_animatedModelSet.find(f_model) == m_animatedModelSet.end()) m_animatedModelSet.insert(f_model);
    }
    else
    {
        if(m_staticModelSet.find(f_model) == m_staticModelSet.end()) m_staticModelSet.insert(f_model);
    }
}
void ROC::PreRenderManager::RemoveModel(Model *f_model)
{
    if(f_model->HasSkeleton())
    {
        auto iter = m_animatedModelSet.find(f_model);
        if(iter != m_animatedModelSet.end()) m_animatedModelSet.erase(iter);
    }
    else
    {
        auto iter = m_staticModelSet.find(f_model);
        if(iter != m_staticModelSet.end()) m_staticModelSet.erase(iter);
    }
}

void ROC::PreRenderManager::DoPulse_S1()
{
    EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
    if(l_eventManager->IsEventExists(EventType::PreRender)) l_eventManager->CallEvent(EventType::PreRender,m_argument);
    for(auto iter : m_animatedModelSet) iter->UpdateAnimation();
    for(auto iter : m_animatedModelSet) iter->UpdateMatrix();
    if(m_core->GetPhysicsManager()->GetPhysicsEnabled())
    {
        for(auto iter : m_animatedModelSet) iter->UpdateSkeletonChains();
    }
}
void ROC::PreRenderManager::DoPulse_S2()
{
    if(m_core->GetPhysicsManager()->GetPhysicsEnabled())
    {
        for(auto iter : m_animatedModelSet) iter->UpdateSkeletonRigidBones();
    }
    for(auto iter : m_animatedModelSet) iter->UpdateMatrix();
    for(auto iter : m_staticModelSet) iter->UpdateMatrix();
}
