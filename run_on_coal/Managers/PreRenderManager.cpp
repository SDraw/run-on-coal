#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/PhysicsManager.h"
#include "Elements/Model/Model.h"
#include "Elements/Model/Skeleton.h"
#include "Lua/LuaArguments.h"

ROC::PreRenderManager::PreRenderManager(Core *f_core)
{
    m_core = f_core;
    m_argument = new LuaArguments();
    m_modelToNodeMapEnd = m_modelToNodeMap.end();
}
ROC::PreRenderManager::~PreRenderManager()
{
    m_nodeStack.insert(m_nodeStack.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_node = m_nodeStack.back();
        m_nodeStack.pop_back();
        m_nodeStack.insert(m_nodeStack.end(), l_node->m_children.rbegin(), l_node->m_children.rend());
        delete l_node;
    }
    delete m_argument;
}

void ROC::PreRenderManager::AddModel(Model *f_model)
{
    TreeNode *l_node = new TreeNode();
    l_node->m_model = f_model;
    m_modelTreeSet.insert(l_node);
    m_modelToNodeMap.insert(std::make_pair(f_model, l_node));
    m_modelToNodeMapEnd = m_modelToNodeMap.end();
}

void ROC::PreRenderManager::AddLink(Model *f_model, Model *f_parent)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        auto l_parentIter = m_modelToNodeMap.find(f_parent);
        if(l_parentIter != m_modelToNodeMapEnd)
        {
            TreeNode *l_modelNode = l_modelIter->second;
            TreeNode *l_parentNode = l_parentIter->second;
            if(l_parentNode->m_children.find(l_modelNode) == l_parentNode->m_children.end())
            {
                if(!l_modelNode->m_parent) m_modelTreeSet.erase(l_modelNode);
                l_modelNode->m_parent = l_parentNode;
                l_parentNode->m_children.insert(l_modelNode);
            }
        }
    }
}
void ROC::PreRenderManager::RemoveLink(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        TreeNode *l_node = l_modelIter->second;
        if(l_node->m_parent)
        {
            l_node->m_parent->m_children.erase(l_node);
            m_modelTreeSet.insert(l_node);
            l_node->m_parent = NULL;
        }
    }
}

void ROC::PreRenderManager::RemoveModel(Model *f_model)
{
    auto l_modelIter = m_modelToNodeMap.find(f_model);
    if(l_modelIter != m_modelToNodeMapEnd)
    {
        TreeNode *l_node = l_modelIter->second;
        if(l_node->m_parent) l_node->m_parent->m_children.erase(l_node);
        for(auto iter : l_node->m_children)
        {
            m_modelTreeSet.insert(iter);
            iter->m_parent = NULL;
        }
        m_modelToNodeMapEnd = m_modelToNodeMap.end();
        m_modelTreeSet.erase(l_node);
        delete l_node;
    }
}

void ROC::PreRenderManager::DoPulse_S1()
{
    m_core->GetLuaManager()->GetEventManager()->CallEvent("onOGLPreRender", m_argument);
    bool l_physicsState = m_core->GetPhysicsManager()->GetPhysicsEnabled();

    m_nodeStack.insert(m_nodeStack.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_current = m_nodeStack.back();
        Model *l_model = l_current->m_model;
        m_nodeStack.pop_back();
        m_nodeStack.insert(m_nodeStack.end(), l_current->m_children.rbegin(), l_current->m_children.rend());

        if(!l_model->HasCollision()) l_model->UpdateMatrix();
        if(l_model->HasSkeleton())
        {
            if(l_model->HasAnimation()) l_model->UpdateAnimation();
            l_model->GetSkeleton()->UpdateCollision_S1(l_model->GetMatrixRef(), l_physicsState);
        }
    }
}
void ROC::PreRenderManager::DoPulse_S2()
{
    bool l_physicsState = m_core->GetPhysicsManager()->GetPhysicsEnabled();

    m_nodeStack.insert(m_nodeStack.end(), m_modelTreeSet.rbegin(), m_modelTreeSet.rend());
    while(!m_nodeStack.empty())
    {
        TreeNode *l_current = m_nodeStack.back();
        Model *l_model = l_current->m_model;
        m_nodeStack.pop_back();
        m_nodeStack.insert(m_nodeStack.end(), l_current->m_children.rbegin(), l_current->m_children.rend());

        l_model->HasCollision() ? l_model->UpdateCollision() : l_model->UpdateMatrix();
        if(l_model->HasSkeleton()) l_model->GetSkeleton()->UpdateCollision_S2(l_model->GetMatrixRef(), l_physicsState);
    }
}
