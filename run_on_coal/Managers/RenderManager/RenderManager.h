#pragma once

namespace ROC
{

class Core;
class VRManager;
class Model;
class Scene;
class Shader;
class Quad2D;
class Quad3D;
class Drawable;
class RenderTarget;
class Texture;
class Font;
class PhysicsDrawer;
class LuaArguments;

typedef void(*OnRenderCallback)(void);
typedef void(*OnVRRenderCallback)(const std::string &f_side);

class RenderManager final
{
    Core *m_core;
    VRManager *m_vrManager;

    bool m_locked;
    bool m_vrLock;

    glm::vec3 m_modelPosition;
    glm::mat4 m_textureMatrix;

    glm::vec4 m_clearColor;
    glm::ivec2 m_viewportSize;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    Texture *m_dummyTexture;
    PhysicsDrawer *m_physicsDrawer;
    Texture *m_lastTexture;

    LuaArguments *m_luaArguments;

    float m_time;

    bool m_depthEnabled;
    bool m_blendEnabled;
    bool m_cullEnabled;
    bool m_skipNoDepthMaterials;
    void DisableDepth();
    void EnableDepth();
    void DisableBlending();
    void EnableBlending();
    void DisableCulling();
    void EnableCulling();

    OnRenderCallback m_callback;
    OnVRRenderCallback m_vrCallback;

    RenderManager(const RenderManager &that) = delete;
    RenderManager& operator=(const RenderManager &that) = delete;
public:
    void SetActiveScene(Scene *f_scene);

    void Render(Font *f_font, const glm::vec2 &f_pos, const sf::String &f_text, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
    void DrawScene(Scene *f_scene);
    void DrawPhysics(float f_width);

    void ClearRenderArea(bool f_depth = true, bool f_color = true);
    void SetClearColour(const glm::vec4 &f_color);
    void SetViewport(const glm::ivec4 &f_area);
    void SetPolygonMode(int f_mode);

    inline void SetRenderCallback(OnRenderCallback f_callback) { m_callback = f_callback; }
    inline void SetVRRenderCallback(OnVRRenderCallback f_callback) { m_vrCallback = f_callback; }
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RemoveAsActiveScene(Scene *f_scene);

    void UpdateViewportSize(const glm::ivec2 &f_size);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
    friend class InheritanceManager;
    friend class SfmlManager;
};

}
