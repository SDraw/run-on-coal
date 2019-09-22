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
class SceneLayer;

class Scene final : public Element, public virtual IScene
{
    Camera *m_camera;
    RenderTarget *m_renderTarget;
    std::vector<Light*> m_lights;
    std::vector<SceneLayer*> m_layers;

    bool m_active;

    Scene(const Scene &that) = delete;
    Scene& operator=(const Scene &that) = delete;

    void UpdateLayers();

    static bool SceneLayerSorter(const SceneLayer *f_layerA, const SceneLayer *f_layerB);

    // ROC::IScene
    bool SetICamera(ICamera *f_cam);
    ICamera* GetICamera() const;
    bool SetIRenderTarget(IRenderTarget *f_rt);
    IRenderTarget* GetIRenderTarget() const;
    bool AddIShader(IShader *f_shader, const std::string &f_layer, unsigned char f_priority);
    bool RemoveIShader(IShader *f_shader);
    bool HasIShader(IShader *f_shader) const;
    bool AddILight(ILight *f_light);
    bool RemoveILight(ILight *f_light);
    bool HasILight(ILight *f_light) const;
    bool AddIModel(IModel *f_model, const std::string &f_layer);
    bool RemoveIModel(IModel *f_model);
    bool HasIModel(IModel *f_model) const;
    bool SetIModelLayer(IModel *f_model, const std::string &f_layer);
public:
    bool SetCamera(Camera *f_cam);
    bool RemoveCamera();
    Camera* GetCamera() const;
    inline bool HasCamera() const { return (m_camera != nullptr); }

    bool SetRenderTarget(RenderTarget *f_rt);
    bool RemoveRenderTarget();
    RenderTarget* GetRenderTarget() const;
    inline bool HasRenderTarget() const { return (m_renderTarget != nullptr); }

    bool AddLight(Light *f_light);
    bool RemoveLight(Light *f_light);
    bool HasLight(Light *f_light) const;
    size_t GetLightsCount() const;

    bool AddShader(Shader *f_shader, const std::string &f_layer, unsigned char f_priority);
    bool RemoveShader(Shader *f_shader);
    bool HasShader(Shader *f_shader) const;

    bool AddModel(Model *f_model, const std::string &f_layer);
    bool RemoveModel(Model *f_model);
    bool HasModel(Model *f_model) const;
    bool SetModelLayer(Model *f_model, const std::string &f_layer);

    bool IsActive() const;
protected:
    Scene();
    ~Scene();

    inline const std::vector<SceneLayer*>& GetLayers() const { return m_layers; }
    const SceneLayer* GetLayer(const std::string &f_layer) const;
    inline const std::vector<Light*>& GetLights() const { return m_lights; }

    void Enable();
    void Disable();

    void OnChildLinkDestroyed(Element *f_child);

    friend class ElementManager;
    friend class RenderManager;
};

}
