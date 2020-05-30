#pragma once

namespace ROC
{

class Camera;
class Model;
class Shader;

class SceneLayer
{
    struct RenderModel
    {
        Model *m_model;
        float m_distance;
        bool m_visible;
        RenderModel(Model *f_model)
        {
            m_model = f_model;
            m_distance = 0.f;
            m_visible = true;
        }
    };

    size_t m_hash;
    Shader *m_shader;
    unsigned char m_priority;
    std::vector<RenderModel*> m_renderModels;
    bool m_resortGeometry;

    static bool RenderModelComparator_Geometry(const RenderModel *f_modelA, const RenderModel *f_modelB);
    static bool RenderModelComparator_Distance(const RenderModel *f_modelA, const RenderModel *f_modelB);
protected:
    SceneLayer(size_t f_hash, Shader *f_shader, unsigned char f_priority);
    ~SceneLayer();

    inline void SetShader(Shader *f_shader) { m_shader = f_shader; }
    inline Shader* GetShader() const { return m_shader; }

    inline void SetPriority(unsigned char f_priority) { m_priority = f_priority; }
    inline unsigned char GetPriority() const { return m_priority; }

    inline size_t GetHash() const { return m_hash; }

    void AddModel(Model *f_model);
    bool RemoveModel(Model *f_model);
    bool HasModel(Model *f_model) const;
    bool HasModels() const;

    const std::vector<RenderModel*>& GetRenderModels() const { return m_renderModels; }

    void Update(Camera *f_camera);

    friend class Scene;
    friend class RenderManager;
};

}
