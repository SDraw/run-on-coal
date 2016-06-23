#include "stdafx.h"
#include "Scene/Scene.h"

ROC::Scene::Scene()
{
    m_mainCamera = NULL;
    m_mainLight = NULL;
}

ROC::Scene::~Scene()
{
}

void ROC::Scene::SetCamera(Camera *f_cam)
{
    m_mainCamera = f_cam;
}
void ROC::Scene::SetLight(Light *f_light)
{
    m_mainLight = f_light;
}

ROC::Camera* ROC::Scene::GetCamera()
{
    return m_mainCamera;
}
ROC::Light* ROC::Scene::GetLight()
{
    return m_mainLight;
}
