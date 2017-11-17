#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Drawable;
class Pool;
class ShaderUniform;

class Shader final : public Element
{
    GLuint m_program;

    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_viewProjectionUniform;
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
    glm::mat4 m_viewProjectionUniformValue;
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

    std::unordered_map<std::string, ShaderUniform*> m_uniformMap;
    std::unordered_map<std::string, ShaderUniform*>::iterator m_uniformMapEnd;

    Pool *m_bindPool;
    struct drawableBindData
    {
        Drawable *m_element;
        int m_slot;
        ShaderUniform *m_uniform;
    };
    std::vector<drawableBindData> m_drawableBind;
    static int ms_drawableMaxCount;

    static GLuint ms_bonesUBO;
    static bool ms_uboFix;

    bool m_active;
    std::string m_error;

    void SetupUniformsAndLocations();
public:
    ShaderUniform* GetUniform(const std::string &f_uniform);
protected:
    Shader();
    ~Shader();
    bool Load(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    inline const std::string& GetError() const { return m_error; }

    void SetProjectionMatrix(const glm::mat4 &f_value);
    void SetViewMatrix(const glm::mat4 &f_value);
    void SetViewProjectionMatrix(const glm::mat4 &f_value);
    void SetModelMatrix(const glm::mat4 &f_value);
    void SetCameraPosition(const glm::vec3 &f_value);
    void SetCameraDirection(const glm::vec3 &f_value);
    void SetLightColor(const glm::vec4 &f_value);
    void SetLightDirection(const glm::vec3 &f_value);
    void SetLightParam(const glm::vec4 &f_value);
    void SetMaterialParam(const glm::vec4 &f_value);
    void SetMaterialType(int f_value);
    void SetAnimated(unsigned int f_value);
    static void SetBoneMatrices(const std::vector<glm::mat4> &f_value);
    void SetTime(float f_value);
    void SetColor(const glm::vec4 &f_value);

    bool Attach(Drawable *f_drawable, const std::string &f_uniform);
    bool Detach(Drawable *f_drawable);
    bool HasAttached(Drawable *f_drawable) const;

    static void UpdateDrawableMaxCount();
    static void CreateBonesUBO();
    static void DestroyBonesUBO();
    static void EnableUBOFix();

    void Enable();
    void Disable();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
    friend class SfmlManager;
    friend class Scene;
};

}
