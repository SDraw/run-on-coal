#include "stdafx.h"

#include "Elements/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"

namespace ROC
{

extern const glm::vec3 g_EmptyVec3;
extern const glm::vec4 g_EmptyVec4;
extern const glm::mat4 g_EmptyMat4;

}

bool ROC::Scene::DistantModelComparator_Geometry(const DistantModel *f_modelA, const DistantModel *f_modelB)
{
    // Not an elegant way to compare by pointers, but groups are made anyway
    return (f_modelA->m_model->GetGeometry() < f_modelB->m_model->GetGeometry());
}
bool ROC::Scene::DistantModelComparator_Distance(const DistantModel *f_modelA, const DistantModel *f_modelB)
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
    for(auto iter : m_distModelVector) delete iter;
    m_distModelVector.clear();
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

void ROC::Scene::SetRenderTarget(RenderTarget *f_rt)
{
    if(m_renderTarget && m_active) m_renderTarget->Disable();
    m_renderTarget = f_rt;
    if(m_renderTarget && m_active) m_renderTarget->Enable();
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

void ROC::Scene::AddModel(Model *f_model)
{
    DistantModel *l_distModel = new DistantModel(f_model, 0.f);
    m_distModelVector.push_back(l_distModel);
    m_sortByGeometry = true;
}
void ROC::Scene::RemoveModel(Model *f_model)
{
    for(auto iter = m_distModelVector.begin(), end = m_distModelVector.end(); iter != end; ++iter)
    {
        if((*iter)->m_model == f_model)
        {
            m_distModelVector.erase(iter);
            break;
        }
    }
}
bool ROC::Scene::HasModel(Model *f_model) const
{
    bool l_result = false;
    for(auto iter = m_distModelVector.begin(), end = m_distModelVector.end(); iter != end; ++iter)
    {
        if((*iter)->m_model == f_model)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}

void ROC::Scene::UpdateDistantModels()
{
    if(m_camera)
    {
        // Sort models by geometry and distance to camera
        size_t l_size = m_distModelVector.size();
        if(l_size > 1U)
        {
            if(m_sortByGeometry)
            {
                std::sort(m_distModelVector.begin(), m_distModelVector.end(), DistantModelComparator_Geometry);
                m_sortByGeometry = false;
            }

            const glm::vec3 &l_cameraPos = m_camera->GetPosition();
            glm::vec3 l_distModelPos(0.f);
            btTransform l_transform = btTransform::getIdentity();
            Geometry *l_lastGeometry = m_distModelVector[0U]->m_model->GetGeometry();
            for(size_t i = 0U, j = 0U; i < l_size; i++)
            {
                DistantModel *l_distModel = m_distModelVector[i];
                Model *l_model = l_distModel->m_model;
                l_transform.setFromOpenGLMatrix(glm::value_ptr(l_model->GetFullMatrix()));
                std::memcpy(&l_distModelPos, l_transform.getOrigin().m_floats, sizeof(glm::vec3));
                l_distModel->m_distance = glm::distance(l_cameraPos, l_distModelPos);
                l_distModel->m_visible = m_camera->IsInFrustum(l_distModelPos, l_model->GetBoundSphereRadius());

                Geometry *l_stepGeometry = l_model->GetGeometry();
                if(l_stepGeometry != l_lastGeometry)
                {
                    std::sort(m_distModelVector.begin() + j, m_distModelVector.begin() + i, DistantModelComparator_Distance);
                    l_lastGeometry = l_stepGeometry;
                    j = i;
                    continue;
                }

                if(i == (l_size - 1U)) std::sort(m_distModelVector.begin() + j, m_distModelVector.end(), DistantModelComparator_Distance);
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
