#pragma once
#include "Interfaces/IRenderManager.h"

namespace ROC
{

class Core;
class VRManager;
class Drawable;
class Font;
class Scene;
class PhysicsDrawer;
class Quad2D;
class Quad3D;
class CustomArguments;

class RenderManager final : public IRenderManager
{
    enum RenderStage : unsigned char
    {
        RS_None = 0U,
        RS_Main,
        RS_VR
    };

    Core *m_core;
    VRManager *m_vrManager;

    RenderStage m_renderStage;
    bool m_clearFrame;

    float m_time;
    glm::ivec2 m_viewportSize;
    glm::mat4 m_screenProjection;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    PhysicsDrawer *m_physicsDrawer;

    CustomArguments *m_arguments;

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

    bool SetViewport(const glm::ivec4 &f_area);
    bool ClearViewport(bool f_depth = true, bool f_color = true);
    bool SetClearColor(const glm::vec4 &f_color);
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
