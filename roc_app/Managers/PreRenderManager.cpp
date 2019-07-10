#include "stdafx.h"

#include "Managers/PreRenderManager.h"
#include "Core/Core.h"
#include "Elements/Model/Model.h"

#include "Managers/ModuleManager.h"
#include "Interfaces/IModule.h"

ROC::PreRenderManager::PreRenderManager(Core *f_core)
{
    m_core = f_core;
    m_modelsEnd = m_models.end();
}
ROC::PreRenderManager::~PreRenderManager()
{
    m_models.clear();
}

void ROC::PreRenderManager::AddModel(Model *f_model)
{
    m_models.push_back(f_model);
    m_modelsEnd = m_models.end();
}
void ROC::PreRenderManager::RemoveModel(Model *f_model)
{
    for(auto l_searchIter = m_models.begin(); l_searchIter != m_modelsEnd; ++l_searchIter)
    {
        if(*l_searchIter == f_model)
        {
            m_models.erase(l_searchIter);
            m_modelsEnd = m_models.end();
            break;
        }
    }
}

void ROC::PreRenderManager::DoPulse_S1()
{
    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnPreRender, m_arguments);

    for(auto l_model : m_models)
    {
        if(!l_model->HasParentModel()) m_modelsStack.push(l_model);
    }

    while(!m_modelsStack.empty())
    {
        Model *l_model = m_modelsStack.top();
        m_modelsStack.pop();

        if(!l_model->HasCollision()) l_model->Update(Model::MUS_Matrix);
        l_model->Update(Model::MUS_Animation);
        l_model->Update(Model::MUS_SkeletonCollisionStatic);

        for(auto l_childElement : l_model->GetChildren())
        {
            if(l_childElement->GetElementType() == Element::ET_Model) m_modelsStack.push(reinterpret_cast<Model*>(l_childElement));
        }
    }
}
void ROC::PreRenderManager::DoPulse_S2()
{
    for(auto l_model : m_models)
    {
        if(!l_model->HasParentModel()) m_modelsStack.push(l_model);
    }

    while(!m_modelsStack.empty())
    {
        Model *l_model = m_modelsStack.top();
        m_modelsStack.pop();

        l_model->Update(l_model->HasCollision() ? Model::MUS_Collision : Model::MUS_Matrix);
        l_model->Update(Model::MUS_SkeletonCollisionDynamic);

        for(auto l_childElement : l_model->GetChildren())
        {
            if(l_childElement->GetElementType() == Element::ET_Model) m_modelsStack.push(reinterpret_cast<Model*>(l_childElement));
        }
    }
}
