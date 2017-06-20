#pragma once

namespace ROC
{

class Core;
class Model;
class Scene;
class Shader;
class Quad;
class Quad3D;
class Drawable;
class Movie;
class RenderTarget;
class Font;
class LuaArguments;
typedef void(*OnOGLRenderCallback)(void);

class RenderManager
{
    Core *m_core;

    bool m_locked;

    glm::vec3 m_modelPosition;
    glm::mat4 m_textureMatrix;
    glm::ivec2 m_renderTargetSize;
    glm::mat4 m_screenProjection;

    Scene *m_activeScene;
    Shader *m_activeShader;
    RenderTarget *m_activeTarget;
    Quad *m_quad;
    Quad3D *m_quad3D;

    std::vector<Movie*> m_movieVector;
    std::vector<Movie*>::iterator m_movieVectorEnd;

    LuaArguments *m_argument;

    float m_time;

    GLuint m_lastVAO;
    GLuint m_lastTexture;
    bool m_depthEnabled;
    bool m_blendEnabled;
    bool m_cullEnabled;
    void DisableDepth();
    void EnableDepth();
    void DisableBlending();
    void EnableBlending();
    void DisableCulling();
    void EnableCulling();
    bool CompareLastVAO(GLuint f_vao);
    bool CompareLastTexture(GLuint f_texture);
    void EnableNonActiveShader(Shader *f_shader) const;

    OnOGLRenderCallback m_callback;

    RenderManager(const RenderManager& that);
    RenderManager &operator =(const RenderManager &that);
public:
    void ClearRenderArea(GLbitfield f_params);
    static inline void SetClearColour(const glm::vec4 &f_color) { glClearColor(f_color.r, f_color.g, f_color.b, f_color.a); }
    static inline void SetViewport(const glm::ivec4 &f_area) { glViewport(f_area.r, f_area.g, f_area.b, f_area.a); }
    static inline void SetPolygonMode(int f_mode) { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + f_mode); }

    void SetRenderTarget(RenderTarget *f_rt);

    void SetActiveScene(Scene *f_scene);
    void RemoveAsActiveScene(Scene *f_scene);

    void SetActiveShader(Shader *f_shader);
    template<typename T> void SetShaderUniformValue(Shader *f_shader, const std::string &f_uniform, T f_value);
    template<typename T> void SetShaderUniformValueRef(Shader *f_shader, const std::string &f_uniform, T &f_value);
    void RemoveAsActiveShader(Shader *f_shader);

    void Render(Model *f_model, bool f_frustum = true, bool f_texturize = true);
    void Render(Font *f_font, const glm::vec2 &f_pos, const sf::String &f_text, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);

    inline void SetRenderCallback(OnOGLRenderCallback f_callback) { m_callback = f_callback; }
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RestoreActiveShader(Shader *f_shader);
    bool AttachToShader(Shader *f_shader, Drawable *f_element, const std::string &f_uniform);
    void DetachFromShader(Shader *f_shader, Drawable *f_element);

    void AddMovie(Movie *f_movie);
    void RemoveMovie(Movie *f_movie);

    void ResetCallsReducing();

    void DoPulse();

    friend Core;
    friend class ElementManager;
    friend class InheritanceManager;
};

}

template<typename T> void ROC::RenderManager::SetShaderUniformValue(Shader *f_shader, const std::string &f_uniform, T f_value)
{
    EnableNonActiveShader(f_shader);
    f_shader->SetUniformValue(f_uniform, f_value);
    RestoreActiveShader(f_shader);
};
template<typename T> void ROC::RenderManager::SetShaderUniformValueRef(Shader *f_shader, const std::string &f_uniform, T &f_value)
{
    EnableNonActiveShader(f_shader);
    f_shader->SetUniformValue(f_uniform, f_value);
    RestoreActiveShader(f_shader);
};
