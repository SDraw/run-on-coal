#include "stdafx.h"

#include "Elements/Scene/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"

#include "Elements/Model/Model.h"

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;

    m_camera = nullptr;
    m_renderTarget = nullptr;
    m_shader = nullptr;

    m_active = false;
}

ROC::Scene::~Scene()
{
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

        if(m_active) m_camera->Update();

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
    if(m_shader) RemoveShader();

    m_shader = f_shader;
    Element::AddChild(f_shader);
    f_shader->AddParent(this);
    return (m_shader != nullptr);
}

bool ROC::Scene::RemoveShader()
{
    if(m_shader)
    {
        Element::RemoveChild(m_shader);
        m_shader = nullptr;
    }
    return (!m_shader);
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
    auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), f_light);
    if(l_searchIter != m_lights.end())
    {
        m_lights.erase(l_searchIter);
        Element::RemoveChild(f_light);
        l_result = true;
    }
    return l_result;
}

size_t ROC::Scene::GetLightsCount() const
{
    return m_lights.size();
}

const std::vector<ROC::Light*>& ROC::Scene::GetLights() const
{
    return m_lights;
}

bool ROC::Scene::AddModel(Model *f_model)
{
    bool l_result = false;
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), f_model);
    if(l_searchIter == m_models.end())
    {
        m_models.push_back(f_model);
        Element::AddChild(f_model);
        f_model->AddParent(this);
        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::RemoveModel(Model *f_model)
{
    bool l_result = false;
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), f_model);
    if(l_searchIter != m_models.end())
    {
        m_models.erase(l_searchIter);
        Element::RemoveChild(f_model);
        l_result = true;
    }
    return l_result;
}

bool ROC::Scene::HasModel(Model *f_model) const
{
    auto l_searchIter = std::find(m_models.begin(), m_models.end(), f_model);
    return (l_searchIter != m_models.end());
}

const std::vector<ROC::Model*>& ROC::Scene::GetModels() const
{
    return m_models;
}

bool ROC::Scene::IsRenderValid() const
{
    return ((m_camera != nullptr) && (m_shader != nullptr));
}

void ROC::Scene::Enable()
{
    if(!m_active)
    {
        if(m_camera)
        {
            m_camera->Update();

            // Set visibilty for models
            glm::vec3 l_pos;
            btTransform l_transform = btTransform::getIdentity();
            for(auto l_model : m_models)
            {
                l_transform.setFromOpenGLMatrix(glm::value_ptr(l_model->GetFullMatrix()));
                const btVector3 &l_btPos = l_transform.getOrigin();
                l_pos.x = l_btPos.x();
                l_pos.y = l_btPos.y();
                l_pos.z = l_btPos.z();

                bool l_visible = m_camera->IsInFrustum(l_pos, l_model->GetBoundSphereRadius());
                l_model->SetVisibility(l_visible);

                float l_dist = glm::distance(m_camera->GetPosition(), l_pos);
                l_model->SetVisibilityDistance(l_dist);
            }

            // Sort by geometry and distance to camera
            std::sort(m_models.begin(), m_models.end(), SceneModelSorting);
        }
        if(m_renderTarget) m_renderTarget->Enable();
        else RenderTarget::Disable();

        m_active = true;
    }
}

void ROC::Scene::Disable()
{
    if(m_active)
    {
        if(m_renderTarget) m_renderTarget->Disable();
        m_active = false;
    }
}

bool ROC::Scene::IsActive() const
{
    return m_active;
}

bool ROC::Scene::SceneModelSorting(const Model *f_modelA, const Model *f_modelB)
{
    if(f_modelA->GetGeometry() != f_modelB->GetGeometry())
    {
        return (f_modelA->GetGeometry() < f_modelB->GetGeometry());
    }
    return (f_modelA->GetVisibilityDistance() < f_modelB->GetVisibilityDistance());
}

void ROC::Scene::OnChildRemoved(Element *f_child)
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
            auto l_searchIter = std::find(m_models.begin(), m_models.end(), f_child);
            if(l_searchIter != m_models.end()) m_models.erase(l_searchIter);
        } break;
        case ET_Shader:
        {
            m_shader = nullptr;
        } break;
    }

    Element::OnChildRemoved(f_child);
}

// Interfaces reroute
bool ROC::Scene::SetICamera(ICamera *f_cam)
{
    return SetCamera(dynamic_cast<Camera*>(f_cam));
}

ROC::ICamera* ROC::Scene::GetICamera() const
{
    return m_camera;
}

bool ROC::Scene::SetIRenderTarget(IRenderTarget *f_rt)
{
    return SetRenderTarget(dynamic_cast<RenderTarget*>(f_rt));
}

ROC::IRenderTarget* ROC::Scene::GetIRenderTarget() const
{
    return m_renderTarget;
}

bool ROC::Scene::SetIShader(IShader *f_shader)
{
    return SetShader(dynamic_cast<Shader*>(f_shader));
}

bool ROC::Scene::RemoveIShader()
{
    return RemoveShader();
}

ROC::IShader* ROC::Scene::GetIShader() const
{
    return GetShader();
}
bool ROC::Scene::AddILight(ILight *f_light)
{
    return AddLight(dynamic_cast<Light*>(f_light));
}

bool ROC::Scene::RemoveILight(ILight *f_light)
{
    return RemoveLight(dynamic_cast<Light*>(f_light));
}

bool ROC::Scene::HasILight(ILight *f_light) const
{
    return HasLight(dynamic_cast<Light*>(f_light));
}

bool ROC::Scene::AddIModel(IModel *f_model)
{
    return AddModel(dynamic_cast<Model*>(f_model));
}

bool ROC::Scene::RemoveIModel(IModel *f_model)
{
    return RemoveModel(dynamic_cast<Model*>(f_model));
}

bool ROC::Scene::HasIModel(IModel *f_model) const
{
    return HasModel(dynamic_cast<Model*>(f_model));
}
