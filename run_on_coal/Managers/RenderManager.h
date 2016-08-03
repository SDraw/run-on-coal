#pragma once
#ifdef __linux__
#include "Scene/Shader.h"
#endif

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
class LuaArguments;
class RenderManager
{
    Core *m_core;

    Scene *m_activeScene;
    Shader *m_activeShader;
    RenderTarget *m_activeTarget;
    Quad *m_quad;
    glm::mat4 m_screenProjection;
    bool m_locked;

    std::vector<glm::mat4> m_boneData;
    glm::mat4 m_modelMatrix;
    glm::vec3 m_modelPosition;
    glm::vec4 m_materialParam;
    glm::mat4 m_textureMatrix;
    glm::ivec2 m_renderTargetSize;
    glm::mat4 m_sceneMatrix;
    glm::vec3 m_sceneVector;
    glm::vec4 m_sceneParam;

    static LuaArguments m_argument;

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
    void EnableNonActiveShader(Shader *f_shader);
public:
    void ClearRenderArea(GLbitfield f_params);
    static void SetClearColour(glm::vec4 &f_color) { glClearColor(f_color.r,f_color.g,f_color.b,f_color.a); }
    inline void SetViewport(glm::ivec4 &f_area) { if(!m_locked) glViewport(f_area.r,f_area.g,f_area.b,f_area.a); }
    static void SetPolygonMode(unsigned int f_mode) { glPolygonMode(GL_FRONT_AND_BACK,GL_POINT+static_cast<int>(f_mode)); }

    void SetActiveScene(Scene *f_scene);
    inline void RemoveAsActiveScene(Scene *f_scene) { if(m_activeScene == f_scene) m_activeScene = NULL; }

    void SetActiveShader(Shader *f_shader);
    template<typename T> void SetShaderUniformValueO(Shader *f_shader, GLint f_uValue, T f_value)
    {
        EnableNonActiveShader(f_shader);
        f_shader->SetUniformValue(f_uValue,f_value);
        RestoreActiveShader(f_shader);
    };
    template<typename T> void SetShaderUniformValueM(Shader *f_shader, GLint f_uValue, T &f_value)
    {
        EnableNonActiveShader(f_shader);
        f_shader->SetUniformValue(f_uValue,f_value);
        RestoreActiveShader(f_shader);
    };
    inline void RemoveAsActiveShader(Shader *f_shader) { if(m_activeShader == f_shader) m_activeShader = NULL; }

    void SetRenderTarget(RenderTarget *f_rt);

    void Render(Model *f_model, bool f_texturize, bool f_frustum = false, float f_radius = 1.f);
    void Render(Font *f_font, glm::vec2 &f_pos, sf::String &f_text, glm::vec4 &f_color);
    void Render(Texture *f_texture, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color);
    void Render(RenderTarget *f_rt, glm::vec2 &f_pos, glm::vec2 &f_size, float f_rot, glm::vec4 &f_color);
protected:
    RenderManager(Core *f_core);
    ~RenderManager();
    void DoPulse();
    void ResetCallsReducing();

    void RestoreActiveShader(Shader *f_shader);
    template<typename T> bool AttachToShader(Shader *f_shader, T *f_element, int f_uniform)
    {
        EnableNonActiveShader(f_shader);
        bool l_result = f_shader->Attach(f_element,f_uniform);
        RestoreActiveShader(f_shader);
        return l_result;
    }
    template<typename T> void DettachFromShader(Shader *f_shader, T *f_element)
    {
        EnableNonActiveShader(f_shader);
        f_shader->Dettach(f_element);
        RestoreActiveShader(f_shader);
    }

    friend Core;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
