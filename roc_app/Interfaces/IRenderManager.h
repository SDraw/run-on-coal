#pragma once

namespace ROC
{

class IDrawable;
class IFont;
class IScene;

class IRenderManager
{
public:
    virtual bool SetActiveIScene(IScene *f_scene) = 0;
    virtual bool Draw(IScene *f_scene) = 0;
    virtual bool Draw(IFont *f_font, const glm::vec2 &f_pos, const std::string &f_text, const glm::vec4 &f_color) = 0;
    virtual bool Draw(IDrawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color) = 0;
    virtual bool Draw(IDrawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params) = 0;
    virtual bool DrawPhysics(float f_width) = 0;
    virtual bool SetViewport(const glm::ivec4 &f_area) = 0;
    virtual bool ClearViewport(bool f_depth = true, bool f_color = true) = 0;
    virtual bool SetClearColor(const glm::vec4 &f_color) = 0;
    virtual bool SetPolygonMode(int f_mode) = 0;
};

}
