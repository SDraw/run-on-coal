#include "stdafx.h"

#include "Elements/Scene.h"
#include "Elements/Model/Model.h"

namespace ROC
{

extern const glm::vec3 g_EmptyVec3;
extern const glm::vec4 g_EmptyVec4;
extern const glm::mat4 g_EmptyMat4;

}

bool ROC::Scene::RenderModelComparator_Geometry(const RenderModel *f_modelA, const RenderModel *f_modelB)
{
    // Not an elegant way to compare by pointers, but groups are made anyway
    return (f_modelA->m_model->GetGeometry() < f_modelB->m_model->GetGeometry());
}
bool ROC::Scene::RenderModelComparator_Distance(const RenderModel *f_modelA, const RenderModel *f_modelB)
{
    return (f_modelA->m_distance < f_modelB->m_distance);
}

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;
    m_elementTypeName.assign("Scene");

    m_camera = nullptr;
    m_renderTarget = nullptr;
    m_shader = nullptr;

    m_sortByGeometry = false;

    m_active = false;
}
ROC::Scene::~Scene()
{
    for(auto l_renderModel : m_renderModels) delete l_renderModel;
    m_renderModels.clear();
}

void ROC::Scene::SetCamera(Camera *f_cam)
{
    m_camera = f_cam;
    if(m_active && m_shader)
    {
        if(m_camera)
        {
            m_camera->Update();
            UpdateDistantModels();
        }
        m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
        m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
        m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
        m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_EmptyVec3);
        m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_EmptyVec3);
    }
}
ROC::Camera* ROC::Scene::GetCamera() const
{
    return m_camera;
}

void ROC::Scene::SetRenderTarget(RenderTarget *f_rt)
{
    if(m_renderTarget && m_active) m_renderTarget->Disable();
    m_renderTarget = f_rt;
    if(m_renderTarget && m_active) m_renderTarget->Enable();
}
ROC::RenderTarget* ROC::Scene::GetRenderTarget() const 
{ 
    return m_renderTarget; 
}


void ROC::Scene::SetShader(Shader *f_shader)
{
    if(m_shader && m_active) m_shader->Disable();
    m_shader = f_shader;
    if(m_shader && m_active)
    {
        m_shader->Enable();
        m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
        m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
        m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
        m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_EmptyVec3);
        m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_EmptyVec3);
        m_shader->SetLightsData(m_lights);
    }
}
ROC::Shader* ROC::Scene::GetShader() const 
{ 
    return m_shader;
}


void ROC::Scene::AddLight(Light *f_light)
{
    m_lights.push_back(f_light);
    if(m_active && m_shader) m_shader->SetLightsData(m_lights);
}
bool ROC::Scene::HasLight(Light *f_light) const
{
    bool l_result = (std::find(m_lights.begin(), m_lights.end(), f_light) != m_lights.end());
    return l_result;
}
void ROC::Scene::RemoveLight(Light *f_light)
{
    auto iter = std::find(m_lights.begin(), m_lights.end(), f_light);
    if(iter != m_lights.end())
    {
        m_lights.erase(iter);
        if(m_active && m_shader) m_shader->SetLightsData(m_lights);
    }
}
size_t ROC::Scene::GetLightsCount() const 
{ 
    return m_lights.size();
}

void ROC::Scene::AddModel(Model *f_model)
{
    RenderModel *l_renderModel = new RenderModel(f_model, 0.f);
    m_renderModels.push_back(l_renderModel);
    m_sortByGeometry = true;
}
void ROC::Scene::RemoveModel(Model *f_model)
{
    for(auto l_renderModelIter = m_renderModels.begin(), l_endIter = m_renderModels.end(); l_renderModelIter != l_endIter; ++l_renderModelIter)
    {
        if((*l_renderModelIter)->m_model == f_model)
        {
            m_renderModels.erase(l_renderModelIter);
            break;
        }
    }
}
bool ROC::Scene::HasModel(Model *f_model) const
{
    bool l_result = false;
    for(auto l_renderModelIter = m_renderModels.begin(), l_endIter = m_renderModels.end(); l_renderModelIter != l_endIter; ++l_renderModelIter)
    {
        if((*l_renderModelIter)->m_model == f_model)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}

bool ROC::Scene::IsActive() const 
{ 
    return m_active;
}

void ROC::Scene::UpdateDistantModels()
{
    if(m_camera)
    {
        // Sort models by geometry and distance to camera
        size_t l_size = m_renderModels.size();
        if(l_size > 1U)
        {
            if(m_sortByGeometry)
            {
                std::sort(m_renderModels.begin(), m_renderModels.end(), RenderModelComparator_Geometry);
                m_sortByGeometry = false;
            }

            const glm::vec3 &l_cameraPos = m_camera->GetPosition();
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
                l_renderModel->m_visible = m_camera->IsInFrustum(l_distModelPos, l_model->GetBoundSphereRadius());

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
}

void ROC::Scene::Enable()
{
    if(!m_active)
    {
        if(m_camera)
        {
            m_camera->Update();
            UpdateDistantModels();
        }
        if(m_renderTarget) m_renderTarget->Enable();
        if(m_shader)
        {
            m_shader->Enable();
            m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
            m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
            m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
            m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_EmptyVec3);
            m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_EmptyVec3);
            m_shader->SetLightsData(m_lights);
        }
        m_active = true;
    }
}

void ROC::Scene::Disable()
{
    if(m_active)
    {
        if(m_renderTarget) m_renderTarget->Disable();
        if(m_shader) m_shader->Disable();
        m_active = false;
    }
}
