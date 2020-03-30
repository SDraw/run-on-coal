#include "stdafx.h"

#include "Elements/Scene/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene/SceneLayer.h"
#include "Elements/Shader/Shader.h"

#include "Elements/Model/Model.h"

namespace ROC
{

extern const glm::vec3 g_EmptyVec3;
extern const glm::vec4 g_EmptyVec4;
extern const glm::mat4 g_EmptyMat4;
const std::hash<std::string> g_StringHash;

}

bool ROC::Scene::SceneLayerSorter(const SceneLayer *f_layerA, const SceneLayer *f_layerB)
{
    return (f_layerA->GetPriority() < f_layerB->GetPriority());
}

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;

    m_camera = nullptr;
    m_renderTarget = nullptr;

    m_active = false;
}
ROC::Scene::~Scene()
{
    for(auto l_layer : m_layers) delete l_layer;
    m_layers.clear();
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

        if(m_active)
        {
            m_camera->Update();
            UpdateLayers();
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

bool ROC::Scene::AddShader(Shader *f_shader, const std::string &f_layer, unsigned char f_priority)
{
    bool l_result = false;

    if(!HasShader(f_shader))
    {
        const size_t l_hash = g_StringHash(f_layer);
        for(auto l_layer : m_layers)
        {
            if(l_layer->GetHash() == l_hash)
            {
                l_layer->SetShader(f_shader);
                l_layer->SetPriority(f_priority);

                l_result = true;
                break;
            }
        }

        if(!l_result)
        {
            SceneLayer *l_layer = new SceneLayer(l_hash, f_shader, f_priority);
            m_layers.push_back(l_layer);
            if(m_layers.size() > 1U) std::sort(m_layers.begin(), m_layers.end(), SceneLayerSorter);

            l_result = true;
        }
    }

    if(l_result)
    {
        Element::AddChild(f_shader);
        f_shader->AddParent(this);
    }

    return l_result;
}
bool ROC::Scene::RemoveShader(Shader *f_shader)
{
    bool l_result = false;
    for(auto l_layer : m_layers)
    {
        if(l_layer->GetShader() == f_shader)
        {
            l_layer->SetShader(nullptr);

            l_result = true;
            break;
        }
    }

    if(l_result) Element::RemoveChild(f_shader);

    return l_result;
}
bool ROC::Scene::HasShader(Shader *f_shader) const
{
    bool l_result = false;
    for(auto l_layer : m_layers)
    {
        if(l_layer->GetShader() == f_shader)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
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
    if(HasLight(f_light))
    {
        auto l_searchIter = std::find(m_lights.begin(), m_lights.end(), f_light);
        if(l_searchIter != m_lights.end()) m_lights.erase(l_searchIter);

        Element::RemoveChild(f_light);

        l_result = true;
    }
    return l_result;
}
size_t ROC::Scene::GetLightsCount() const
{
    return m_lights.size();
}

bool ROC::Scene::AddModel(Model *f_model, const std::string &f_shaderGroup)
{
    bool l_result = false;
    if(!HasModel(f_model))
    {
        size_t l_hash = g_StringHash(f_shaderGroup);
        for(auto l_layer : m_layers)
        {
            if(l_layer->GetHash() == l_hash)
            {
                l_layer->AddModel(f_model);

                l_result = true;
                break;
            }
        }
    }

    if(l_result)
    {
        Element::AddChild(f_model);
        f_model->AddParent(this);
    }

    return l_result;
}
bool ROC::Scene::RemoveModel(Model *f_model)
{
    bool l_result = false;

    for(auto l_layer : m_layers)
    {
        if(l_layer->RemoveModel(f_model))
        {
            l_result = true;
            break;
        }
    }

    if(l_result) Element::RemoveChild(f_model);

    return l_result;
}
bool ROC::Scene::HasModel(Model *f_model) const
{
    bool l_result = false;
    for(auto l_layer : m_layers)
    {
        if(l_layer->HasModel(f_model))
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::Scene::SetModelLayer(Model *f_model, const std::string &f_layer)
{
    bool l_result = false;

    const size_t l_hash = g_StringHash(f_layer);
    for(auto l_layerTo : m_layers)
    {
        if(l_layerTo->GetHash() == l_hash)
        {
            for(auto l_layerFrom : m_layers)
            {
                if(l_layerFrom->RemoveModel(f_model))
                {
                    l_layerTo->AddModel(f_model);

                    l_result = true;
                }
            }
        }

        if(l_result) break;
    }

    return l_result;
}

bool ROC::Scene::IsActive() const
{
    return m_active;
}

const ROC::SceneLayer* ROC::Scene::GetLayer(const std::string &f_layer) const
{
    SceneLayer *l_result = nullptr;
    const size_t l_hash = g_StringHash(f_layer);
    for(auto l_layer : m_layers)
    {
        if(l_layer->GetHash() == l_hash)
        {
            l_result = l_layer;
            break;
        }
    }
    return l_result;
}

void ROC::Scene::UpdateLayers()
{
    if(m_camera)
    {
        for(auto l_layer : m_layers) l_layer->Update(m_camera);
    }
}

void ROC::Scene::Enable()
{
    if(!m_active)
    {
        if(m_camera)
        {
            m_camera->Update();
            UpdateLayers();
        }
        if(m_renderTarget) m_renderTarget->Enable();

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
            for(auto l_layer : m_layers)
            {
                if(l_layer->RemoveModel(reinterpret_cast<Model*>(f_child))) break;
            }
        } break;
        case ET_Shader:
        {
            for(auto l_layer : m_layers)
            {
                if(l_layer->GetShader() == f_child)
                {
                    l_layer->SetShader(nullptr);
                    break;
                }
            }
        } break;
    }

    Element::OnChildLinkDestroyed(f_child);
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
bool ROC::Scene::AddIShader(IShader *f_shader, const std::string &f_layer, unsigned char f_priority)
{
    return AddShader(dynamic_cast<Shader*>(f_shader), f_layer, f_priority);
}
bool ROC::Scene::RemoveIShader(IShader *f_shader)
{
    return RemoveShader(dynamic_cast<Shader*>(f_shader));
}
bool ROC::Scene::HasIShader(IShader *f_shader) const
{
    return HasShader(dynamic_cast<Shader*>(f_shader));
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
bool ROC::Scene::AddIModel(IModel *f_model, const std::string &f_layer)
{
    return AddModel(dynamic_cast<Model*>(f_model), f_layer);
}
bool ROC::Scene::RemoveIModel(IModel *f_model)
{
    return RemoveModel(dynamic_cast<Model*>(f_model));
}
bool ROC::Scene::HasIModel(IModel *f_model) const
{
    return HasModel(dynamic_cast<Model*>(f_model));
}
bool ROC::Scene::SetIModelLayer(IModel *f_model, const std::string &f_layer)
{
    return SetModelLayer(dynamic_cast<Model*>(f_model), f_layer);
}
