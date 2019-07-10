#pragma once
#include "Interfaces/IScene.h"
#include "Elements/Element.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Camera.h"
#include "Elements/Shader/Shader.h"

namespace ROC
{

class Camera;

struct RenderModel
{
    Model *m_model;
    float m_distance;
    bool m_visible = true;
    RenderModel(Model *f_model, float f_dist = 0.f)
    {
        m_model = f_model;
        m_distance = f_dist;
    }
};

class Scene final : public Element, public virtual IScene
{
    Camera *m_camera;
    RenderTarget *m_renderTarget;
    std::vector<Light*> m_lights;
    Shader *m_shader;
    std::vector<RenderModel*> m_renderModels;

    bool m_sortByGeometry;
    bool m_active;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    void UpdateRenderModels();

    static bool RenderModelComparator_Geometry(const RenderModel *f_modelA, const RenderModel *f_modelB);
    static bool RenderModelComparator_Distance(const RenderModel *f_modelA, const RenderModel *f_modelB);

    // Interfaces reroute
    bool SetCamera(ICamera *f_cam);
    bool SetRenderTarget(IRenderTarget *f_rt);
    bool SetShader(IShader *f_shader);
    bool AddLight(ILight *f_light);
    bool RemoveLight(ILight *f_light);
    bool HasLight(ILight *f_light) const;
    bool AddModel(IModel *f_model);
    bool RemoveModel(IModel *f_model);
    bool HasModel(IModel *f_model) const;
public:
    bool SetCamera(Camera *f_cam);
    bool RemoveCamera();
    Camera* GetCamera() const;
    inline bool HasCamera() const { return (m_camera != nullptr); }

    bool SetRenderTarget(RenderTarget *f_rt);
    bool RemoveRenderTarget();
    RenderTarget* GetRenderTarget() const;
    inline bool HasRenderTarget() const { return (m_renderTarget != nullptr); }

    bool SetShader(Shader *f_shader);
    bool RemoveShader();
    Shader* GetShader() const;
    inline bool HasShader() const { return (m_shader != nullptr); }

    bool AddLight(Light *f_light);
    bool RemoveLight(Light *f_light);
    bool HasLight(Light *f_light) const;
    size_t GetLightsCount() const;

    bool AddModel(Model *f_model);
    bool RemoveModel(Model *f_model);
    bool HasModel(Model *f_model) const;

    inline bool IsValidForRender() const { return ((m_camera != nullptr) && (m_shader != nullptr)); }
    bool IsActive() const;
protected:
    Scene();
    ~Scene();

    const std::vector<RenderModel*>& GetRenderModels() const { return m_renderModels; }

    void Enable();
    void Disable();

    void OnChildLinkDestroyed(Element *f_child);

    friend class ElementManager;
    friend class RenderManager;
};

}
