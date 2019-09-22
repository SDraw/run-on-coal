#include "stdafx.h"

#include "Elements/Scene/SceneLayer.h"
#include "Elements/Model/Model.h"

#include "Elements/Camera.h"
#include "Elements/Geometry/Geometry.h"

bool ROC::SceneLayer::RenderModelComparator_Geometry(const RenderModel *f_modelA, const RenderModel *f_modelB)
{
    // Not an elegant way to compare by pointers, but groups are made anyway
    return (f_modelA->m_model->GetGeometry() < f_modelB->m_model->GetGeometry());
}
bool ROC::SceneLayer::RenderModelComparator_Distance(const RenderModel *f_modelA, const RenderModel *f_modelB)
{
    return (f_modelA->m_distance < f_modelB->m_distance);
}

ROC::SceneLayer::SceneLayer(size_t f_hash, Shader *f_shader, unsigned char f_priority)
{
    m_hash = f_hash;
    m_shader = f_shader;
    m_priority = f_priority;
}
ROC::SceneLayer::~SceneLayer()
{
    for(auto l_renderModel : m_renderModels) delete l_renderModel;
    m_renderModels.clear();
}

void ROC::SceneLayer::AddModel(Model *f_model)
{
    RenderModel *l_renderModel = new RenderModel(f_model);
    m_renderModels.push_back(l_renderModel);
}
bool ROC::SceneLayer::RemoveModel(Model *f_model)
{
    bool l_result = false;
    for(auto l_searchIter = m_renderModels.begin(), l_end = m_renderModels.end(); l_searchIter != l_end; ++l_searchIter)
    {
        RenderModel *l_renderModel = (*l_searchIter);
        if(l_renderModel->m_model == f_model)
        {
            delete l_renderModel;
            m_renderModels.erase(l_searchIter);

            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::SceneLayer::HasModel(Model *f_model) const
{
    bool l_result = false;
    for(auto l_searchIter = m_renderModels.begin(), l_end = m_renderModels.end(); l_searchIter != l_end; ++l_searchIter)
    {
        RenderModel *l_renderModel = (*l_searchIter);
        if(l_renderModel->m_model == f_model)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::SceneLayer::HasModels() const
{
    return !m_renderModels.empty();
}

void ROC::SceneLayer::Update(Camera *f_camera)
{
    size_t l_size = m_renderModels.size();
    if(l_size > 1U)
    {
        std::sort(m_renderModels.begin(), m_renderModels.end(), RenderModelComparator_Geometry);

        const glm::vec3 &l_cameraPos = f_camera->GetPosition();
        glm::vec3 l_distModelPos(0.f);
        btTransform l_transform = btTransform::getIdentity();
        Geometry *l_lastGeometry = m_renderModels[0U]->m_model->GetGeometry();
        for(size_t i = 0U, j = 0U; i < l_size; i++)
        {
            RenderModel *l_renderModel = m_renderModels[i];
            Model *l_model = l_renderModel->m_model;
            l_transform.setFromOpenGLMatrix(glm::value_ptr(l_model->GetFullMatrix()));
            std::memcpy(&l_distModelPos, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
            l_renderModel->m_distance = glm::distance(l_cameraPos, l_distModelPos);
            l_renderModel->m_visible = f_camera->IsInFrustum(l_distModelPos, l_model->GetBoundSphereRadius());

            Geometry *l_stepGeometry = l_model->GetGeometry();
            if(l_stepGeometry != l_lastGeometry)
            {
                std::sort(m_renderModels.begin() + j, m_renderModels.begin() + i, RenderModelComparator_Distance);
                l_lastGeometry = l_stepGeometry;
                j = i;
                continue;
            }

            if(i == (l_size - 1U)) std::sort(m_renderModels.begin() + j, m_renderModels.end(), RenderModelComparator_Distance);
        }
    }
}
