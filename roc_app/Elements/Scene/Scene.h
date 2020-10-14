#pragma once
#include "Interfaces/IScene.h"
#include "Elements/Element.h"

namespace ROC
{

class Camera;
class Light;
class Model;
class RenderTarget;
class Shader;

class Scene final : public Element, public virtual IScene
{
    Camera *m_camera;
    RenderTarget *m_renderTarget;
    Shader *m_shader;
    std::vector<Light*> m_lights;
    std::vector<Model*> m_models;

    bool m_active;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    static bool SceneModelSorting(const Model *f_modelA, const Model *f_modelB);

    // ROC::Element
    void OnChildRemoved(Element *f_child) override;

    // ROC::IScene
    bool SetICamera(ICamera *f_cam);
    ICamera* GetICamera() const;
    bool SetIRenderTarget(IRenderTarget *f_rt);
    IRenderTarget* GetIRenderTarget() const;
    bool SetIShader(IShader *f_shader);
    bool RemoveIShader();
    IShader* GetIShader() const;
    bool AddILight(ILight *f_light);
    bool RemoveILight(ILight *f_light);
    bool HasILight(ILight *f_light) const;
    bool AddIModel(IModel *f_model);
    bool RemoveIModel(IModel *f_model);
    bool HasIModel(IModel *f_model) const;
public:
    bool SetCamera(Camera *f_cam);
    bool RemoveCamera();
    Camera* GetCamera() const;

    bool SetRenderTarget(RenderTarget *f_rt);
    bool RemoveRenderTarget();
    RenderTarget* GetRenderTarget() const;

    bool AddLight(Light *f_light);
    bool RemoveLight(Light *f_light);
    bool HasLight(Light *f_light) const;
    size_t GetLightsCount() const;

    bool SetShader(Shader *f_shader);
    bool RemoveShader();
    Shader* GetShader() const;

    bool AddModel(Model *f_model);
    bool RemoveModel(Model *f_model);
    bool HasModel(Model *f_model) const;

    bool IsActive() const;
    bool IsRenderValid() const;
protected:
    Scene();
    ~Scene();

    const std::vector<Light*>& GetLights() const;
    const std::vector<Model*>& GetModels() const;

    void Enable();
    void Disable();

    friend class ElementManager;
    friend class RenderManager;
};

}
