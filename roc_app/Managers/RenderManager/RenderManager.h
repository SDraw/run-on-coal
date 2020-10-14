#pragma once
#include "Interfaces/IRenderManager.h"

namespace ROC
{

class Core;
class VRManager;
class Drawable;
class Camera;
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
    float m_time;
    bool m_clearFrame;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    PhysicsDrawer *m_physicsDrawer;
    Camera *m_screenCamera;

    CustomArguments *m_arguments;

    RenderManager(const RenderManager &that) = delete;
    RenderManager& operator=(const RenderManager &that) = delete;

    // ROC::IRenderManager
    bool SetActiveIScene(IScene *f_scene);
    bool Draw(IScene *f_scene);
    bool Draw(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color);
    bool Draw(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    bool Draw(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
public:
    bool SetActiveScene(Scene *f_scene);

    bool Draw(Scene *f_scene);
    bool Draw(Font *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color);
    bool Draw(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    bool Draw(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);
    bool DrawPhysics(float f_width);

    bool SetViewport(const glm::ivec4 &f_area);
    bool ClearViewport(bool f_depth = true, bool f_color = true);
    bool SetClearColor(const glm::vec4 &f_color);
    bool SetPolygonMode(int f_mode);
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RemoveAsActiveScene(Scene *f_scene);
    void UpdateScreenSize(const glm::ivec2 &f_size);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
    friend class SfmlManager;
};

}
