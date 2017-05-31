#pragma once
#include "Elements/Element.h"
#define ROC_SHADER_BONES_COUNT 227U
#define ROC_SHADER_BONES_BINDPOINT 0

namespace ROC
{

class Drawable;
class Pool;
class Shader : public Element
{
    GLuint m_program;

    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_modelUniform;
    GLint m_cameraPositionUniform;
    GLint m_cameraDirectionUniform;
    GLint m_lightColorUniform;
    GLint m_lightDirectionUniform;
    GLint m_lightParamUniform;
    GLint m_materialParamUniform;
    GLint m_materialTypeUniform;
    GLint m_animatedUniform;
    GLint m_texture0Uniform;
    GLint m_timeUniform;
    GLint m_colorUniform;

    glm::mat4 m_projectionUniformValue;
    glm::mat4 m_viewUniformValue;
    glm::mat4 m_modelUniformValue;
    glm::vec3 m_cameraPositionUniformValue;
    glm::vec3 m_cameraDirectionUniformValue;
    unsigned int m_lightingUniformValue;
    glm::vec4 m_lightColorUniformValue;
    glm::vec3 m_lightDirectionUniformValue;
    glm::vec4 m_lightParamUniformValue;
    glm::vec4 m_materialParamUniformValue;
    int m_materialTypeUniformValue;
    unsigned int m_animatedUniformValue;
    float m_timeUniformValue;
    glm::vec4 m_colorUniformValue;

    std::unordered_map<std::string, GLint> m_uniformMap;
    std::unordered_map<std::string, GLint>::iterator m_uniformMapEnd;

    Pool *m_bindPool;
    struct drawableBindData
    {
        Drawable *m_element;
        int m_slot;
        int m_uniform;
    };
    std::vector<drawableBindData> m_drawableBind;
    unsigned int m_drawableCount;

    static GLuint m_bonesUBO;
    static bool m_uboFix;

    std::string m_error;

    Shader(const Shader& that);
    Shader &operator =(const Shader &that);

    void SetupDefaultUniformsAndLocations();
protected:
    Shader();
    ~Shader();
    bool Load(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    inline void GetError(std::string &f_str) { f_str.assign(m_error); }

    void SetUniformValue(const std::string &f_uniform, unsigned int f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::uvec2 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::uvec3 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::uvec4 &f_value);

    void SetUniformValue(const std::string &f_uniform, int f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::ivec2 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::ivec3 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::ivec4 &f_value);

    void SetUniformValue(const std::string &f_uniform, float f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::vec2 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::vec3 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::vec4 &f_value);

    void SetUniformValue(const std::string &f_uniform, double f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::dvec2 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::dvec3 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::dvec4 &f_value);

    void SetUniformValue(const std::string &f_uniform, const glm::mat2 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::mat3 &f_value);
    void SetUniformValue(const std::string &f_uniform, const glm::mat4 &f_value);

    void SetProjectionUniformValue(const glm::mat4 &f_value);
    void SetViewUniformValue(const glm::mat4 &f_value);
    void SetModelUniformValue(const glm::mat4 &f_value);
    void SetCameraPositionUniformValue(const glm::vec3 &f_value);
    void SetCameraDirectionUniformValue(const glm::vec3 &f_value);
    void SetLightColorUniformValue(const glm::vec4 &f_value);
    void SetLightDirectionUniformValue(const glm::vec3 &f_value);
    void SetLightParamUniformValue(const glm::vec4 &f_value);
    void SetMaterialParamUniformValue(const glm::vec4 &f_value);
    void SetMaterialTypeUniformValue(int f_value);
    void SetAnimatedUniformValue(unsigned int f_value);
    static void SetBonesUniformValue(const std::vector<glm::mat4> &f_value);
    void SetTimeUniformValue(float f_value);
    void SetColorUniformValue(const glm::vec4 &f_value);

    bool Attach(Drawable *f_drawable, const std::string &f_uniform);
    bool Detach(Drawable *f_drawable);

    static void CreateBonesUBO();
    static void DestroyBonesUBO();
    static void EnableUBOFix();

    void Enable(bool f_full = true);

    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
    friend class SfmlManager;
};

}
