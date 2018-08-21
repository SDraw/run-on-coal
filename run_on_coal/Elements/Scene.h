#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Camera;
class Light;
class RenderTarget;
class Shader;

class Scene final : public Element
{
    Camera *m_camera;
    Light *m_light;
    RenderTarget *m_renderTarget;
    Shader *m_shader;

    glm::vec3 m_skyGradientDown;
    glm::vec3 m_skyGradientUp;

    bool m_active;

    Scene(const Scene &that);
    Scene& operator=(const Scene &that);
public:
    inline Camera* GetCamera() const { return m_camera; }
    inline Light* GetLight() const { return m_light; }
    inline RenderTarget* GetRenderTarget() const { return m_renderTarget; }
    inline Shader* GetShader() const { return m_shader; }

    inline bool HasCamera() const { return (m_camera != nullptr); }
    inline bool HasLight() const { return (m_light != nullptr); }
    inline bool HasRenderTarget() const { return (m_renderTarget != nullptr); }
    inline bool HasShader() const { return (m_shader != nullptr); }

    void SetSkyGradient(const glm::vec3 &f_colorDown, const glm::vec3 &f_colorUp);
    void GetSkyGradient(glm::vec3 &f_colorDown, glm::vec3 &f_colorUp) const;

    inline bool IsValidForRender() const { return ((m_camera != nullptr) && (m_shader != nullptr)); }
    inline bool IsActive() const { return m_active; }
protected:
    Scene();
    ~Scene();

    void SetCamera(Camera *f_cam);
    void SetLight(Light *f_light);
    void SetRenderTarget(RenderTarget *f_rt);
    void SetShader(Shader *f_shader);

    void Enable();
    void Disable();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
};

}
