#pragma once
#include "Interfaces/IScene.h"
#include "Elements/Element.h"
#include "Elements/Light.h"
#include "Elements/RenderTarget.h"
#include "Elements/Camera.h"
#include "Elements/Shader/Shader.h"

namespace ROC
{

class Camera;
class Model;

struct DistantModel
{
    Model *m_model;
    float m_distance;
    bool m_visible = true;
    DistantModel(Model *f_model, float f_dist = 0.f)
    {
        m_model = f_model;
        m_distance = f_dist;
    }
};

class Scene final : public Element, public virtual IScene
{
    Camera *m_camera;
    std::vector<Light*> m_lights;
    RenderTarget *m_renderTarget;
    Shader *m_shader;

    bool m_sortByGeometry;

    bool m_active;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    void UpdateDistantModels();

    static bool DistantModelComparator_Geometry(const DistantModel *f_modelA, const DistantModel *f_modelB);
    static bool DistantModelComparator_Distance(const DistantModel *f_modelA, const DistantModel *f_modelB);
public:
    Camera* GetCamera() const;
    RenderTarget* GetRenderTarget() const;
    Shader* GetShader() const;

    inline bool HasCamera() const { return (m_camera != nullptr); }
    bool HasLight(Light *f_light) const;
    size_t GetLightsCount() const;
    inline bool HasRenderTarget() const { return (m_renderTarget != nullptr); }
    inline bool HasShader() const { return (m_shader != nullptr); }
    bool HasModel(Model *f_model) const;

    inline bool IsValidForRender() const { return ((m_camera != nullptr) && (m_shader != nullptr)); }
    bool IsActive() const;
protected:
    std::vector<DistantModel*> m_distModelVector;

    Scene();
    ~Scene();

    void SetCamera(Camera *f_cam);
    void SetRenderTarget(RenderTarget *f_rt);
    void SetShader(Shader *f_shader);

    void AddLight(Light *f_light);
    void RemoveLight(Light *f_light);

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);
    const std::vector<DistantModel*>& GetDistantModels() const { return m_distModelVector; }

    void Enable();
    void Disable();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
};

}
