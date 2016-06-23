#pragma once
#include "Scene/Camera.h"
#include "Scene/Light.h"

namespace ROC
{

class Scene
{
    Camera *m_mainCamera;
    Light *m_mainLight;
public:
    Camera* GetCamera();
    Light* GetLight();
protected:
    Scene();
    ~Scene();
    void SetCamera(Camera *f_cam);
    void SetLight(Light *f_light);
    friend class ElementManager;
    friend class InheritanceManager;
};

}
