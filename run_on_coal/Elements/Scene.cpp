#include "stdafx.h"

#include "Elements/Scene.h"
#include "Elements/Camera.h"
#include "Elements/Light.h"

ROC::Scene::Scene()
{
    m_elementType = ET_Scene;
    m_elementTypeName.assign("Scene");

    m_mainCamera = nullptr;
    m_mainLight = nullptr;
}
ROC::Scene::~Scene()
{
}
