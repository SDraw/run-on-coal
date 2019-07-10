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

bool ROC::Scene::SetCamera(Camera *f_cam)
{
    bool l_result = false;

    if(m_camera != f_cam)
    {
        if(m_camera) Element::RemoveChild(m_camera);

        m_camera = f_cam;
        Element::AddChild(m_camera);
        m_camera->AddParent(this);

        if(m_active && m_shader)
        {
            if(m_camera)
            {
                m_camera->Update();
                UpdateRenderModels();
            }
            m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
            m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
            m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
            m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_EmptyVec3);
            m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_EmptyVec3);
        }

        l_result = true;
    }
    return l_result;
}
bool ROC::Scene::RemoveCamera()
{
    bool l_result = false;
    if(m_camera)
    {
        Element::RemoveChild(m_camera);
        m_camera = nullptr;
        l_result = true;
    }
    return l_result;
}
ROC::Camera* ROC::Scene::GetCamera() const
{
    return m_camera;
}

bool ROC::Scene::SetRenderTarget(RenderTarget *f_rt)
{
    bool l_result = false;
    if(m_renderTarget != f_rt)
    {
        if(m_renderTarget)
        {
            if(m_active) m_renderTarget->Disable();
            Element::RemoveChild(m_renderTarget);
        }

        m_renderTarget = f_rt;
        Element::AddChild(m_renderTarget);
        m_renderTarget->AddParent(this);

        if(m_active) m_renderTarget->Enable();

        l_result = true;
    }
    return l_result;
}
bool ROC::Scene::RemoveRenderTarget()
{
    bool l_result = false;
    if(m_renderTarget)
    {
        if(m_active) m_renderTarget->Disable();

        Element::RemoveChild(m_renderTarget);
        m_renderTarget = nullptr;

        l_result = true;
    }
    return l_result;
}
ROC::RenderTarget* ROC::Scene::GetRenderTarget() const
{
    return m_renderTarget;
}

bool ROC::Scene::SetShader(Shader *f_shader)
{
    bool l_result = false;

    if(m_shader != f_shader)
    {
        if(m_shader && m_active) m_shader->Disable();
        Element::RemoveChild(m_shader);

        m_shader = f_shader;
        Element::AddChild(m_shader);
        m_shader->AddParent(this);

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

        l_result = true;
    }
    return l_result;
}
bool ROC::Scene::RemoveShader()
{
    bool l_result = false;
    if(m_shader)
    {
        if(m_active) m_shader->Disable();

        Element::RemoveChild(m_shader);
        m_shader = nullptr;

        l_result = true;
    }
    return l_result;
}
ROC::Shader* ROC::Scene::GetShader() const
{
    return m_shader;
}


bool ROC::Scene::AddLight(Light *f_light)
{
    bool l_result = false;
    if(!HasLight(f_light))
    {
        m_lights.push_back(f_light);
        Element::AddChild(f_light);
        f_light->AddParent(this);

        if(m_active && m_shader) m_shader->SetLightsData(m_lights);

        l_result = true;
    }
    return l_result;
}
bool ROC::Scene::HasLight(Light *f_light) const
{
    bool l_result = (std::find(m_lights.begin(), m_lights.end(), f_light) != m_lights.end());
    return l_result;
}
bool ROC::Scene::RemoveLight(Light *f_light)
{
    bool l_result = false;
    if(HasLight(f_light))
    {
        auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), f_light);
        if(l_searchIter != m_lights.end()) m_lights.erase(l_searchIter);

        Element::RemoveChild(f_light);

        if(m_active && m_shader) m_shader->SetLightsData(m_lights);

        l_result = true;
    }
    return l_result;
}
size_t ROC::Scene::GetLightsCount() const
{
    return m_lights.size();
}

bool ROC::Scene::AddModel(Model *f_model)
{
    bool l_result = false;
    if(!HasModel(f_model))
    {
        RenderModel *l_renderModel = new RenderModel(f_model, 0.f);
        m_renderModels.push_back(l_renderModel);

        Element::AddChild(f_model);
        f_model->AddParent(this);

        m_sortByGeometry = true;
        l_result = true;
    }
    return l_result;
}
bool ROC::Scene::RemoveModel(Model *f_model)
{
    bool l_result = false;

    if(HasModel(f_model))
    {
        for(auto l_renderModelIter = m_renderModels.begin(), l_end = m_renderModels.end(); l_renderModelIter != l_end; ++l_renderModelIter)
        {
            if((*l_renderModelIter)->m_model == f_model)
            {
                m_renderModels.erase(l_renderModelIter);
                break;
            }
        }

        Element::RemoveChild(f_model);

        l_result = true;
    }
    return l_result;
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

void ROC::Scene::UpdateRenderModels()
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
            UpdateRenderModels();
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

void ROC::Scene::OnChildLinkDestroyed(Element *f_child)
{
    switch(f_child->GetElementType())
    {
        case ET_Camera:
            m_camera = nullptr;
            break;
        case ET_RenderTarget:
        {
            m_renderTarget = nullptr;
            if(m_active) RenderTarget::Fallback();
        } break;
        case ET_Light:
        {
            auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), f_child);
            if(l_searchIter != m_lights.end()) m_lights.erase(l_searchIter);
        } break;
        case ET_Model:
        {
            for(auto l_searchIter = m_renderModels.begin(), l_end = m_renderModels.end(); l_searchIter != l_end; ++l_searchIter)
            {
                if((*l_searchIter)->m_model == f_child)
                {
                    m_renderModels.erase(l_searchIter);
                    break;
                }
            }
        } break;
    }

    Element::OnChildLinkDestroyed(f_child);
}

// Interfaces reroute
bool ROC::Scene::SetCamera(ICamera *f_cam)
{
    return SetCamera(dynamic_cast<Camera*>(f_cam));
}
bool ROC::Scene::SetRenderTarget(IRenderTarget *f_rt)
{
    return SetRenderTarget(dynamic_cast<RenderTarget*>(f_rt));
}
bool ROC::Scene::SetShader(IShader *f_shader)
{
    return SetShader(dynamic_cast<Shader*>(f_shader));
}
bool ROC::Scene::AddLight(ILight *f_light)
{
    return AddLight(dynamic_cast<Light*>(f_light));
}
bool ROC::Scene::RemoveLight(ILight *f_light)
{
    return RemoveLight(dynamic_cast<Light*>(f_light));
}
bool ROC::Scene::HasLight(ILight *f_light) const
{
    return HasLight(dynamic_cast<Light*>(f_light));
}
bool ROC::Scene::AddModel(IModel *f_model)
{
    return AddModel(dynamic_cast<Model*>(f_model));
}
bool ROC::Scene::RemoveModel(IModel *f_model)
{
    return RemoveModel(dynamic_cast<Model*>(f_model));
}
bool ROC::Scene::HasModel(IModel *f_model) const
{
    return HasModel(dynamic_cast<Model*>(f_model));
}
