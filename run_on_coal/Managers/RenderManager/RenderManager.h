#pragma once

namespace ROC
{

class Core;
class Model;
class Scene;
class Shader;
class Quad2D;
class Quad3D;
class Drawable;
class Movie;
class RenderTarget;
class Texture;
class Font;
class LuaArguments;
typedef void(*OnRenderCallback)(void);

class RenderManager final
{
    Core *m_core;

    bool m_locked;

    glm::vec3 m_modelPosition;
    glm::mat4 m_textureMatrix;
    glm::ivec2 m_windowSize;
    glm::bvec2 m_materialBind;
    glm::bvec2 m_fontBind;

    Scene *m_activeScene;
    Quad2D *m_quad2D;
    Quad3D *m_quad3D;
    Texture *m_dummyTexture;

    std::vector<Movie*> m_movieVector;
    std::vector<Movie*>::iterator m_movieVectorEnd;

    LuaArguments *m_argument;

    float m_time;

    GLuint m_lastVAO;
    GLuint m_lastTexture;
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
    bool CompareLastVAO(GLuint f_vao);
    bool CompareLastTexture(GLuint f_texture);
    void EnableNonActiveShader(Shader *f_shader) const;

    OnRenderCallback m_callback;

    RenderManager(const RenderManager& that);
    RenderManager &operator =(const RenderManager &that);
public:
    void SetActiveScene(Scene *f_scene);

    void Render(Model *f_model);
    void Render(Font *f_font, const glm::vec2 &f_pos, const sf::String &f_text, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color);
    void Render(Drawable *f_drawable, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params);

    void ClearRenderArea(bool f_depth = true, bool f_color = true);
    static inline void SetClearColour(const glm::vec4 &f_color) { glClearColor(f_color.r, f_color.g, f_color.b, f_color.a); }
    static inline void SetViewport(const glm::ivec4 &f_area) { glViewport(f_area.r, f_area.g, f_area.b, f_area.a); }
    static inline void SetPolygonMode(int f_mode) { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + f_mode); }

    inline void SetRenderCallback(OnRenderCallback f_callback) { m_callback = f_callback; }
protected:
    explicit RenderManager(Core *f_core);
    ~RenderManager();

    void RemoveAsActiveScene(Scene *f_scene);

    void AddMovie(Movie *f_movie);
    void RemoveMovie(Movie *f_movie);

    void ResetCallsReducing();

    void DoPulse();

    friend class Core;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
