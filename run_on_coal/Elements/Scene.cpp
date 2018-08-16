#include "stdafx.h"

#include "Elements/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"

namespace ROC
{

extern const glm::vec3 g_DefaultPosition;
extern const glm::vec3 g_EmptyVec3;
extern const glm::vec4 g_EmptyVec4;
extern const glm::mat4 g_EmptyMat4;

}

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;
    m_elementTypeName.assign("Scene");

    m_camera = nullptr;
    m_light = nullptr;
    m_renderTarget = nullptr;
    m_shader = nullptr;

    m_skyGradientDown = g_EmptyVec3;
    m_skyGradientUp = g_EmptyVec3;

    m_active = false;
}
ROC::Scene::~Scene()
{
}

void ROC::Scene::SetSkyGradient(const glm::vec3 &f_colorDown, const glm::vec3 &f_colorUp)
{
    if(m_skyGradientDown != f_colorDown) std::memcpy(&m_skyGradientDown, &f_colorDown, sizeof(glm::vec3));
    if(m_skyGradientUp != f_colorUp) std::memcpy(&m_skyGradientUp, &f_colorUp, sizeof(glm::vec3));
}
void ROC::Scene::GetSkyGradient(glm::vec3 &f_colorDown, glm::vec3 &f_colorUp) const
{
    std::memcpy(&f_colorDown, &m_skyGradientDown, sizeof(glm::vec3));
    std::memcpy(&f_colorUp, &m_skyGradientUp, sizeof(glm::vec3));
}

void ROC::Scene::SetCamera(Camera *f_cam)
{
    m_camera = f_cam;
    if(m_active && m_shader)
    {
        if(m_camera) m_camera->Update();
        m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
        m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
        m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
        m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_DefaultPosition);
        m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_DefaultPosition);
    }
}
void ROC::Scene::SetLight(Light *f_light)
{
    m_light = f_light;
    if(m_active && m_shader)
    {
        m_shader->SetLightColor(m_light ? m_light->GetColor() : g_EmptyVec4);
        m_shader->SetLightDirection(m_light ? m_light->GetDirection() : g_DefaultPosition);
        m_shader->SetLightParam(m_light ? m_light->GetParams() : g_EmptyVec4);
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
        m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_DefaultPosition);
        m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_DefaultPosition);

        m_shader->SetLightColor(m_light ? m_light->GetColor() : g_EmptyVec4);
        m_shader->SetLightDirection(m_light ? m_light->GetDirection() : g_DefaultPosition);
        m_shader->SetLightParam(m_light ? m_light->GetParams() : g_EmptyVec4);

        m_shader->SetSkyGradientDown(m_skyGradientDown);
        m_shader->SetSkyGradientUp(m_skyGradientUp);
    }
}

void ROC::Scene::Enable()
{
    if(!m_active)
    {
        if(m_camera) m_camera->Update();
        if(m_renderTarget) m_renderTarget->Enable();
        if(m_shader)
        {
            m_shader->Enable();
            m_shader->SetProjectionMatrix(m_camera ? m_camera->GetProjectionMatrix() : g_EmptyMat4);
            m_shader->SetViewMatrix(m_camera ? m_camera->GetViewMatrix() : g_EmptyMat4);
            m_shader->SetViewProjectionMatrix(m_camera ? m_camera->GetViewProjectionMatrix() : g_EmptyMat4);
            m_shader->SetCameraPosition(m_camera ? m_camera->GetPosition() : g_DefaultPosition);
            m_shader->SetCameraDirection(m_camera ? m_camera->GetDirection() : g_DefaultPosition);

            m_shader->SetLightColor(m_light ? m_light->GetColor() : g_EmptyVec4);
            m_shader->SetLightDirection(m_light ? m_light->GetDirection() : g_DefaultPosition);
            m_shader->SetLightParam(m_light ? m_light->GetParams() : g_EmptyVec4);

            m_shader->SetSkyGradientDown(m_skyGradientDown);
            m_shader->SetSkyGradientUp(m_skyGradientUp);
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
