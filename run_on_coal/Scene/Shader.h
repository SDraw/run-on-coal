#pragma once

namespace ROC
{

class Pool;
class Texture;
class RenderTarget;
class Shader
{
    GLuint m_program;

    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_modelUniform;
    GLint m_cameraPositionUniform;
    GLint m_cameraDirectionUniform;
    GLint m_lightingUniform;
    GLint m_lightColorUniform;
    GLint m_lightDirectionUniform;
    GLint m_lightParamUniform;
    GLint m_materialParamUniform;
    GLint m_materialTypeUniform;
    GLint m_animatedUniform;
    //GLint m_bonesUniform;
    GLint m_texture0Uniform; //Useless to store?
    GLint m_timeUniform;
    GLint m_colorUniform;
	
	GLuint m_bonesUBO;
	static Pool m_bonesBindPool;
	int m_boneBindIndex;
    
    glm::mat4 m_projectionUniformValue;
    glm::mat4 m_viewUniformValue;
    glm::mat4 m_modelUniformValue;
    glm::vec3 m_cameraPositionUniformValue;
    glm::vec3 m_cameraDirectionUniformValue;
    unsigned int m_lightingUniformValue;
    glm::vec3 m_lightColorUniformValue;
    glm::vec3 m_lightDirectionUniformValue;
    glm::vec4 m_lightParamUniformValue;
    glm::vec4 m_materialParamUniformValue;
    int m_materialTypeUniformValue;
    unsigned int m_animatedUniformValue;
    float m_timeUniformValue;
    glm::vec4 m_colorUniformValue;


    Pool *m_bindPool;
    struct textureBindData
    {
        Texture *m_texture;
        int m_slot;
        int m_uniform;
    };
    struct targetBindData
    {
        RenderTarget *m_target;
        int m_slot;
        int m_uniform;
    };
    std::vector<textureBindData> m_textureBind;
    std::vector<targetBindData> m_targetBind;
    std::string m_error;

    void SetupDefaultUniformsAndLocations();
public:
    GLint GetUniform(std::string &f_uname);
protected:
    Shader();
    ~Shader();
    bool Load(std::string &f_vpath,std::string &f_fpath,std::string &f_gpath);
    void Enable(bool f_textureBind = false);

    void SetUniformValue(GLint f_uValue, unsigned int f_value);
    void SetUniformValue(GLint f_uValue, std::vector<unsigned int> &f_value);
    void SetUniformValue(GLint f_uValue, glm::uvec2 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::uvec2> &f_value);
    void SetUniformValue(GLint f_uValue, glm::uvec3 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::uvec3> &f_value);
    void SetUniformValue(GLint f_uValue, glm::uvec4 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::uvec4> &f_value);

    void SetUniformValue(GLint f_uValue, int f_value);
    void SetUniformValue(GLint f_uValue, std::vector<int> &f_value);
    void SetUniformValue(GLint f_uValue, glm::ivec2 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::ivec2> &f_value);
    void SetUniformValue(GLint f_uValue, glm::ivec3 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::ivec3> &f_value);
    void SetUniformValue(GLint f_uValue, glm::ivec4 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::ivec4> &f_value);

    void SetUniformValue(GLint f_uValue, float f_value);
    void SetUniformValue(GLint f_uValue, std::vector<float> &f_value);
    void SetUniformValue(GLint f_uValue, glm::vec2 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::vec2> &f_value);
    void SetUniformValue(GLint f_uValue, glm::vec3 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::vec3> &f_value);
    void SetUniformValue(GLint f_uValue, glm::vec4 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::vec4> &f_value);

    void SetUniformValue(GLint f_uValue, double f_value);
    void SetUniformValue(GLint f_uValue, std::vector<double> &f_value);
    void SetUniformValue(GLint f_uValue, glm::dvec2 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::dvec2> &f_value);
    void SetUniformValue(GLint f_uValue, glm::dvec3 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::dvec3> &f_value);
    void SetUniformValue(GLint f_uValue, glm::dvec4 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::dvec4> &f_value);

    void SetUniformValue(GLint f_uValue, glm::mat2 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::mat2> &f_value);
    void SetUniformValue(GLint f_uValue, glm::mat3 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::mat3> &f_value);
    void SetUniformValue(GLint f_uValue, glm::mat4 &f_value);
    void SetUniformValue(GLint f_uValue, std::vector<glm::mat4> &f_value);

    void SetProjectionUniformValue(glm::mat4 &f_value);
    void SetViewUniformValue(glm::mat4 &f_value);
    void SetModelUniformValue(glm::mat4 &f_value);
    void SetCameraPositionUniformValue(glm::vec3 &f_value);
    void SetCameraDirectionUniformValue(glm::vec3 &f_value);
    void SetLightingUniformValue(unsigned int f_value);
    void SetLightColorUniformValue(glm::vec3 &f_value);
    void SetLightDirectionUniformValue(glm::vec3 &f_value);
    void SetLightParamUniformValue(glm::vec4 &f_value);
    void SetMaterialParamUniformValue(glm::vec4 &f_value);
    void SetMaterialTypeUniformValue(int f_value);
    void SetAnimatedUniformValue(unsigned int f_value);
    void SetBonesUniformValue(std::vector<glm::mat4> &f_value);
    void SetTimeUniformValue(float f_value);
    void SetColorUniformValue(glm::vec4 &f_value);

    bool Attach(Texture *f_texture, int f_uniform);
    void Dettach(Texture *f_texture);
    bool Attach(RenderTarget *f_target, int f_uniform);
    void Dettach(RenderTarget *f_target);

    inline void GetError(std::string &f_str) { f_str.append(m_error); }
    friend class ElementManager;
    friend class InheritanceManager;
    friend class RenderManager;
};

}
