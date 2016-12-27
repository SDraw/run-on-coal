#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/PhysicsManager.h"
#include "Model/Model.h"
#include "Model/Skeleton.h"
#include "Lua/LuaArguments.h"

ROC::PreRenderManager::PreRenderManager(Core *f_core)
{
    m_core = f_core;
    m_argument = new LuaArguments();
}
ROC::PreRenderManager::~PreRenderManager()
{
    m_nodeList.insert(m_nodeList.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeList.empty())
    {
        TreeNode *l_node = m_nodeList.back();
        m_nodeList.pop_back();
        m_nodeList.insert(m_nodeList.end(), l_node->m_children.rbegin(), l_node->m_children.rend());
        delete l_node;
    }
    delete m_argument;
}

void ROC::PreRenderManager::AddModel(Model *f_model)
{
    if(m_modelToNodeMap.find(f_model) != m_modelToNodeMap.end()) return;
    TreeNode *l_node = new TreeNode();
    l_node->m_model = f_model;
    m_modelTreeSet.insert(l_node);
    m_modelToNodeMap.insert(std::pair<Model*, TreeNode*>(f_model, l_node));
}

void ROC::PreRenderManager::AddLink(Model *f_model, Model *f_parent)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter == m_modelToNodeMap.end()) return;
    auto l_parentIter = m_modelToNodeMap.find(f_parent);
    if(l_parentIter == m_modelToNodeMap.end()) return;
    TreeNode *l_modelNode = l_modelIter->second;
    TreeNode *l_parentNode = l_parentIter->second;
    if(l_parentNode->m_children.find(l_modelNode) != l_parentNode->m_children.end()) return;

    if(!l_modelNode->m_parent) m_modelTreeSet.erase(l_modelNode);
    l_modelNode->m_parent = l_parentNode;
    l_parentNode->m_children.insert(l_modelNode);
}
void ROC::PreRenderManager::RemoveLink(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter == m_modelToNodeMap.end()) return;
    TreeNode *l_node = l_modelIter->second;

    if(!l_node->m_parent) return;
    l_node->m_parent->m_children.erase(l_node);

    m_modelTreeSet.insert(l_node);
    l_node->m_parent = NULL;
}

void ROC::PreRenderManager::RemoveModel(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter == m_modelToNodeMap.end()) return;
    TreeNode *l_node = l_modelIter->second;

    if(l_node->m_parent) l_node->m_parent->m_children.erase(l_node);
    for(auto iter : l_node->m_children)
    {
        m_modelTreeSet.insert(iter);
        iter->m_parent = NULL;
    }
    m_modelTreeSet.erase(l_node);
    delete l_node;
}

void ROC::PreRenderManager::DoPulse_S1()
{
    m_core->GetLuaManager()->GetEventManager()->CallEvent(EventType::PreRender, m_argument);
    bool l_physicsState = m_core->GetPhysicsManager()->GetPhysicsEnabled();

    m_nodeList.insert(m_nodeList.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeList.empty())
    {
        TreeNode *l_current = m_nodeList.back();
        m_nodeList.pop_back();
        m_nodeList.insert(m_nodeList.end(), l_current->m_children.rbegin(), l_current->m_children.rend());
        if(l_current->m_model->HasCollision() && l_physicsState) continue;

        l_current->m_model->UpdateMatrix();
        if(l_current->m_model->HasSkeleton())
        {
            l_current->m_model->UpdateAnimation();
            l_current->m_model->GetSkeleton()->UpdateCollision_S1(l_current->m_model->GetMatrixRef(), l_physicsState);
        }
    }
}
void ROC::PreRenderManager::DoPulse_S2()
{
    bool l_physicsState = m_core->GetPhysicsManager()->GetPhysicsEnabled();

    m_nodeList.insert(m_nodeList.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeList.empty())
    {
        TreeNode *l_current = m_nodeList.back();
        m_nodeList.pop_back();
        m_nodeList.insert(m_nodeList.end(), l_current->m_children.rbegin(), l_current->m_children.rend());
        l_current->m_model->HasCollision() ? l_current->m_model->UpdateCollision() : l_current->m_model->UpdateMatrix();
        if(l_current->m_model->HasSkeleton()) l_current->m_model->GetSkeleton()->UpdateCollision_S2(l_current->m_model->GetMatrixRef(), l_physicsState);
    }
}