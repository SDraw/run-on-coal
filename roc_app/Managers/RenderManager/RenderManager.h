#pragma once
#include "Interfaces/IRenderManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class VRManager;
class Drawable;
class Font;
class Scene;
class Texture;
class PhysicsDrawer;
class Quad2D;
class Quad3D;

class RenderManager final : public IRenderManager
{
    Core *m_core;
    VRManager *m_vrManager;

    bool m_active;
    bool m_vrActive;

    glm::vec4 m_clearColor;
    glm::ivec2 m_viewportSize;
    glm::mat4 m_screenProjection;
    int m_lastFillMode;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    Texture *m_dummyTexture;
    PhysicsDrawer *m_physicsDrawer;
    Texture *m_lastTexture;

    CustomArguments m_arguments;

    float m_time;

    bool m_depthEnabled;
    bool m_blendEnabled;
    bool m_cullEnabled;
    void DisableDepth();
    void EnableDepth();
    void DisableBlending();
    void EnableBlending();
    void DisableCulling();
    void EnableCulling();

    void PrepareActiveScene();

    RenderManager(const RenderManager &that) = delete;
    RenderManager& operator=(const RenderManager &that) = delete;

    // ROC::IRenderManager
    bool SetActiveScene(IScene *f_scene);
    bool Render(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color, const std::string &f_layer);
    bool Render(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer);
    bool Render(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params);
    bool DrawScene(IScene *f_scene);
public:
    bool SetActiveScene(Scene *f_scene);

    bool Render(Font *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color, const std::string &f_layer);
    bool Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer);
    bool Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params);
    bool DrawScene(Scene *f_scene);
    bool DrawPhysics(float f_width, const std::string &f_layer);

    bool ClearRenderArea(bool f_depth = true, bool f_color = true);
    bool SetClearColour(const glm::vec4 &f_color);
    bool SetViewport(const glm::ivec4 &f_area);
    bool SetPolygonMode(int f_mode);
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RemoveAsActiveScene(Scene *f_scene);
    void UpdateViewportSize(const glm::ivec2 &f_size);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
    friend class SfmlManager;
};

}
