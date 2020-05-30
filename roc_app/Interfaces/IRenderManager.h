#pragma once

namespace ROC
{

class IDrawable;
class IFont;
class IScene;

class IRenderManager
{
public:
    virtual bool SetActiveScene(IScene *f_scene) = 0;
    virtual bool Render(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color, const std::string &f_layer) = 0;
    virtual bool Render(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer) = 0;
    virtual bool Render(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params) = 0;
    virtual bool DrawScene(IScene *f_scene) = 0;
    virtual bool DrawPhysics(float f_width, const std::string &f_layer) = 0;
    virtual bool SetViewport(const glm::ivec4 &f_area) = 0;
    virtual bool ClearViewport(bool f_depth = true, bool f_color = true) = 0;
    virtual bool SetClearColor(const glm::vec4 &f_color) = 0;
    virtual bool SetPolygonMode(int f_mode) = 0;
};

}
