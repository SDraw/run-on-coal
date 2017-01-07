#include "stdafx.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"
#include "Elements/Scene.h"

ROC::Scene::Scene()
{
    m_mainCamera = NULL;
    m_mainLight = NULL;
}
ROC::Scene::~Scene()
{
}
