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

    enum ShaderDefaultUniform : size_t
    {
        SDU_Projection,
        SDU_View,
        SDU_ViewProjection,
        SDU_Model,
        SDU_BoneMatrices,
        SDU_CameraPosition,
        SDU_CameraDirection,
        SDU_LightColor,
        SDU_LightDirection,
        SDU_LightParam,
        SDU_SkyGradientDown,
        SDU_SkyGradientUp,
        SDU_MaterialParam,
        SDU_MaterialType,
        SDU_Animated,
        SDU_DiffuseTexture,
        SDU_Time,
        SDU_Color,

        SDU_UniformCount
    };
    ShaderUniform *m_defaultUniforms[SDU_UniformCount];

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

    std::string m_error;

    Shader(const Shader &that);
    Shader& operator=(const Shader &that);

    void SetupUniformsAndLocations();
    void FindDefaultUniform(ShaderDefaultUniform f_sud, const char *f_name, unsigned int f_type);
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
    void SetBoneMatrices(const std::vector<glm::mat4> &f_value);
    void SetCameraPosition(const glm::vec3 &f_value);
    void SetCameraDirection(const glm::vec3 &f_value);
    void SetLightColor(const glm::vec4 &f_value);
    void SetLightDirection(const glm::vec3 &f_value);
    void SetLightParam(const glm::vec4 &f_value);
    void SetSkyGradientDown(const glm::vec3 &f_value);
    void SetSkyGradientUp(const glm::vec3 &f_value);
    void SetMaterialParam(const glm::vec4 &f_value);
    void SetMaterialType(int f_value);
    void SetAnimated(bool f_value);
    void SetTime(float f_value);
    void SetColor(const glm::vec4 &f_value);

    bool Attach(Drawable *f_drawable, const std::string &f_uniform);
    bool Detach(Drawable *f_drawable);
    bool HasAttached(Drawable *f_drawable) const;

    static void UpdateDrawableMaxCount();

    void Enable();
    void Disable();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
    friend class SfmlManager;
    friend class Scene;
};

}
