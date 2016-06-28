#pragma once

namespace ROC
{

class Core;
class Model;
class Scene;
class Shader;
class Quad;
class RenderTarget;
class Texture;
class Font;
class RenderManager
{
    Core *m_core;

    Scene *m_currentScene;
    Shader *m_currentShader;
    RenderTarget *m_currentRT;
    Quad *m_quad;
    glm::mat4 m_screenProjection;
    bool m_locked;

    //OpenGL calls reducing
    GLuint m_lastVAO;
    GLuint m_lastTexture;
    bool m_depthEnabled;
    bool m_blendEnabled;
    bool m_cullEnabled;
    float m_time;

    void DisableDepth();
    void EnableDepth();
    void DisableBlending();
    void EnableBlending();
    void DisableCulling();
    void EnableCulling();
    bool CompareLastVAO(GLuint f_vao);
    bool CompareLastTexture(GLuint f_texture);
    void CheckShader(Shader *f_shader);
public:

    void ClearRenderArea(GLbitfield f_params);
    void SetClearColour(glm::vec4 &f_color);
    void SetViewport(glm::ivec4 &f_area);
    void SetPolygonMode(unsigned int f_mode);

    void SetCurrentScene(Scene *f_scene);
    void CheckAndRemoveSceneIfCurrent(Scene *f_scene);
    void SetCurrentShader(Shader *f_shader);
    template<typename T> void SetShaderUniformValueO(Shader *f_shader, GLint f_uValue, T f_value)
    {
        CheckShader(f_shader);
        f_shader->SetUniformValue(f_uValue,f_value);
        RestoreShader(f_shader);
    };
    template<typename T> void SetShaderUniformValueM(Shader *f_shader, GLint f_uValue, T &f_value)
    {
        CheckShader(f_shader);
        f_shader->SetUniformValue(f_uValue,f_value);
        RestoreShader(f_shader);
    };
    void CheckShaderForCurrent(Shader *f_shader);

    void SetRenderTarget(RenderTarget *f_rt);

    void Render(Model *f_model, bool f_texturize);
    void Render(Font *f_font, glm::vec2 &f_pos, std::wstring &f_text, glm::vec4 &f_color);
    void Render(Texture *f_texture, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color);
    void Render(RenderTarget *f_rt, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color);
protected:
    RenderManager(Core *f_core);
    ~RenderManager();
    void DoPulse();
    void ResetCallsReducing();
    void RestoreShader(Shader *f_shader);
    friend Core;
    friend class ElementManager;
};

}
