#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Camera;
class Light;
class Scene : public Element
{
    Camera *m_mainCamera;
    Light *m_mainLight;
public:
    inline Camera* GetCamera() { return m_mainCamera; }
    inline Light* GetLight() { return m_mainLight; }
protected:
    Scene();
    ~Scene();

    inline void SetCamera(Camera *f_cam) { m_mainCamera = f_cam; }
    inline void SetLight(Light *f_light) { m_mainLight = f_light; }

    friend class ElementManager;
    friend class InheritanceManager;
};

}
