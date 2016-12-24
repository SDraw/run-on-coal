#include "stdafx.h"
#include "Scene/Shader.h"
#include "Scene/Texture.h"
#include "Scene/RenderTarget.h"
#include "Utils/Pool.h"
#include "Utils/Utils.h"

ROC::Pool *ROC::Shader::m_uboBindPool = NULL;

ROC::Shader::Shader()
{
    m_program = 0;

    m_projectionUniform = -1;
    m_viewUniform = -1;
    m_modelUniform = -1;
    m_cameraPositionUniform = -1;
    m_cameraDirectionUniform = -1;
    m_lightColorUniform = -1;
    m_lightDirectionUniform = -1;
    m_lightParamUniform = -1;
    m_materialParamUniform = -1;
    m_materialTypeUniform = -1;
    m_animatedUniform = -1;
    m_bonesUBO = 0xFFFFFFFF;
    m_boneBindIndex = -1;
    m_texture0Uniform = -1;
    m_timeUniform = -1;
    m_colorUniform = -1;

    m_projectionUniformValue = glm::mat4(0.f);
    m_viewUniformValue = glm::mat4(0.f);
    m_modelUniformValue = glm::mat4(0.f);
    m_cameraPositionUniformValue = glm::vec3(0.f);
    m_cameraDirectionUniformValue = glm::vec3(0.f);
    m_lightingUniformValue = 0U;
    m_lightColorUniformValue = glm::vec4(0.f);
    m_lightDirectionUniformValue = glm::vec3(0.f);
    m_lightParamUniformValue = glm::vec4(0.f);
    m_materialParamUniformValue = glm::vec4(0.f);
    m_materialTypeUniformValue = 0;
    m_animatedUniformValue = 0U;
    m_timeUniformValue = 0.f;
    m_colorUniformValue = glm::vec4(0.f);

    m_bindPool = new Pool(31U);
}
ROC::Shader::~Shader()
{
    if(m_program) glDeleteProgram(m_program);
    delete m_bindPool;
    if(m_bonesUBO != 0xFFFFFFFF)
    {
        glDeleteBuffers(1, &m_bonesUBO);
        m_uboBindPool->Reset(m_boneBindIndex);
    }
    m_textureBind.clear();
    m_targetBind.clear();
}

bool ROC::Shader::Load(std::string &f_vpath, std::string &f_fpath, std::string &f_gpath)
{
    if(m_program) return false;
    GLuint l_vertexShader = 0U;
    GLuint l_fragmentShader = 0U;
    GLuint l_geometryShader = 0U;
    bool l_result = false;
    if(!f_vpath.empty())
    {
        std::string l_data;
        if(Utils::ReadFile(f_vpath, l_data))
        {
            l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
            if(l_vertexShader)
            {
                const char *l_source = l_data.c_str();
                glShaderSource(l_vertexShader, 1, &l_source, NULL);
                glCompileShader(l_vertexShader);
                if(!Utils::CheckShader(l_vertexShader))
                {
                    Utils::GetShaderInfoLog(l_vertexShader, m_error);
                    glDeleteShader(l_vertexShader);
                    l_vertexShader = 0U;
                }
            }
        }
    }
    if(!f_fpath.empty())
    {
        std::string l_data;
        if(Utils::ReadFile(f_fpath, l_data))
        {
            l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            if(l_fragmentShader)
            {
                const char *l_source = l_data.c_str();
                glShaderSource(l_fragmentShader, 1, &l_source, NULL);
                glCompileShader(l_fragmentShader);
                if(!Utils::CheckShader(l_fragmentShader))
                {
                    Utils::GetShaderInfoLog(l_fragmentShader, m_error);
                    glDeleteShader(l_fragmentShader);
                    l_fragmentShader = 0U;
                }
            }
        }
    }
    if(!f_gpath.empty())
    {
        std::string l_data;
        if(Utils::ReadFile(f_gpath, l_data))
        {
            l_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            if(l_geometryShader)
            {
                const char *l_source = l_data.c_str();
                glShaderSource(l_geometryShader, 1, &l_source, NULL);
                glCompileShader(l_geometryShader);
                if(!Utils::CheckShader(l_geometryShader))
                {
                    Utils::GetShaderInfoLog(l_geometryShader, m_error);
                    glDeleteShader(l_geometryShader);
                    l_geometryShader = 0U;
                }
            }
        }
    }
    if(l_vertexShader && l_fragmentShader)
    {
        m_program = glCreateProgram();
        if(m_program)
        {
            glAttachShader(m_program, l_vertexShader);
            glAttachShader(m_program, l_fragmentShader);
            if(l_geometryShader) glAttachShader(m_program, l_geometryShader);
            GLint l_link = 0;
            glLinkProgram(m_program);
            glGetProgramiv(m_program, GL_LINK_STATUS, &l_link);
            if(!l_link)
            {
                GLint l_logLength = 0;
                glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &l_logLength);
                m_error.resize(l_logLength);
                glGetProgramInfoLog(m_program, l_logLength, &l_logLength, (GLchar*)m_error.data());
                glDeleteProgram(m_program);
                m_program = 0U;
            }
            else
            {
                glDetachShader(m_program, l_vertexShader);
                glDetachShader(m_program, l_fragmentShader);
                if(l_geometryShader) glDetachShader(m_program, l_vertexShader);
                SetupDefaultUniformsAndLocations();
                l_result = true;
            }
            glDeleteShader(l_vertexShader);
            glDeleteShader(l_fragmentShader);
            if(l_geometryShader) glDeleteShader(l_geometryShader);
        }
    }
    return l_result;
}
void ROC::Shader::SetupDefaultUniformsAndLocations()
{
    glBindAttribLocation(m_program, 0, "gVertexPosition");
    glBindAttribLocation(m_program, 1, "gVertexUV");
    glBindAttribLocation(m_program, 2, "gVertexNormal");
    glBindAttribLocation(m_program, 3, "gVertexWeight");
    glBindAttribLocation(m_program, 4, "gVertexIndex");

    glUseProgram(m_program);

    //Matrices
    m_projectionUniform = glGetUniformLocation(m_program, "gProjectionMatrix");
    m_viewUniform = glGetUniformLocation(m_program, "gViewMatrix");
    m_modelUniform = glGetUniformLocation(m_program, "gModelMatrix");
    //Vectors
    m_cameraPositionUniform = glGetUniformLocation(m_program, "gCameraPosition");
    m_cameraDirectionUniform = glGetUniformLocation(m_program, "gCameraDirection");
    //Light
    m_lightColorUniform = glGetUniformLocation(m_program, "gLightColor");
    m_lightDirectionUniform = glGetUniformLocation(m_program, "gLightDirection");
    m_lightParamUniform = glGetUniformLocation(m_program, "gLightParam");
    //Material
    m_materialParamUniform = glGetUniformLocation(m_program, "gMaterialParam");
    m_materialTypeUniform = glGetUniformLocation(m_program, "gMaterialType");
    //Animation
    m_animatedUniform = glGetUniformLocation(m_program, "gAnimated");
    unsigned int l_boneUniform = glGetUniformBlockIndex(m_program, "gBonesUniform");
    if(l_boneUniform != GL_INVALID_INDEX)
    {
        m_boneBindIndex = m_uboBindPool->Allocate();
        if(m_boneBindIndex != -1)
        {
            glGenBuffers(1, &m_bonesUBO);
            glBindBuffer(GL_UNIFORM_BUFFER, m_bonesUBO);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * MAX_BONES_PER_MODEL, NULL, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, m_boneBindIndex, m_bonesUBO);
            glUniformBlockBinding(m_program, l_boneUniform, m_boneBindIndex);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }
    //Samplers
    m_texture0Uniform = glGetUniformLocation(m_program, "gTexture0");
    if(m_texture0Uniform != -1) glUniform1i(m_texture0Uniform, 0);
    //Time
    m_timeUniform = glGetUniformLocation(m_program, "gTime");
    //Color (RenderTarget, Texture, Font)
    m_colorUniform = glGetUniformLocation(m_program, "gColor");
}

GLint ROC::Shader::GetUniform(std::string &f_uname)
{
    return glGetUniformLocation(m_program, f_uname.c_str());
}

void ROC::Shader::SetUniformValue(GLint f_uValue, unsigned int f_value)
{
    glUniform1ui(f_uValue, f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<unsigned int> &f_value)
{
    glUniform1uiv(f_uValue, f_value.size(), f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::uvec2 &f_value)
{
    glUniform2ui(f_uValue, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::uvec2> &f_value)
{
    glUniform2uiv(f_uValue, f_value.size(), (GLuint*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::uvec3 &f_value)
{
    glUniform3ui(f_uValue, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::uvec3> &f_value)
{
    glUniform3uiv(f_uValue, f_value.size(), (GLuint*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::uvec4 &f_value)
{
    glUniform4ui(f_uValue, f_value.x, f_value.y, f_value.z, f_value.w);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::uvec4> &f_value)
{
    glUniform4uiv(f_uValue, f_value.size(), (GLuint*)f_value.data());
}

void ROC::Shader::SetUniformValue(GLint f_uValue, int f_value)
{
    glUniform1i(f_uValue, f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<int> &f_value)
{
    glUniform1iv(f_uValue, f_value.size(), f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::ivec2 &f_value)
{
    glUniform2i(f_uValue, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::ivec2> &f_value)
{
    glUniform2iv(f_uValue, f_value.size(), (GLint*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::ivec3 &f_value)
{
    glUniform3i(f_uValue, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::ivec3> &f_value)
{
    glUniform3iv(f_uValue, f_value.size(), (GLint*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::ivec4 &f_value)
{
    glUniform4i(f_uValue, f_value.x, f_value.y, f_value.z, f_value.w);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::ivec4> &f_value)
{
    glUniform4iv(f_uValue, f_value.size(), (GLint*)f_value.data());
}

void ROC::Shader::SetUniformValue(GLint f_uValue, float f_value)
{
    glUniform1f(f_uValue, f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<float> &f_value)
{
    glUniform1fv(f_uValue, f_value.size(), f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::vec2 &f_value)
{
    glUniform2f(f_uValue, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::vec2> &f_value)
{
    glUniform2fv(f_uValue, f_value.size(), (GLfloat*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::vec3 &f_value)
{
    glUniform3f(f_uValue, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::vec3> &f_value)
{
    glUniform3fv(f_uValue, f_value.size(), (GLfloat*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::vec4 &f_value)
{
    glUniform4f(f_uValue, f_value.x, f_value.y, f_value.z, f_value.w);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::vec4> &f_value)
{
    glUniform4fv(f_uValue, f_value.size(), (GLfloat*)f_value.data());
}

void ROC::Shader::SetUniformValue(GLint f_uValue, double f_value)
{
    glUniform1d(f_uValue, f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<double> &f_value)
{
    glUniform1dv(f_uValue, f_value.size(), f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::dvec2 &f_value)
{
    glUniform2d(f_uValue, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::dvec2> &f_value)
{
    glUniform2dv(f_uValue, f_value.size(), (GLdouble*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::dvec3 &f_value)
{
    glUniform3d(f_uValue, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::dvec3> &f_value)
{
    glUniform3dv(f_uValue, f_value.size(), (GLdouble*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::dvec4 &f_value)
{
    glUniform4d(f_uValue, f_value.x, f_value.y, f_value.z, f_value.w);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::dvec4> &f_value)
{
    glUniform4dv(f_uValue, f_value.size(), (GLdouble*)f_value.data());
}

void ROC::Shader::SetUniformValue(GLint f_uValue, glm::mat2 &f_value)
{
    glUniformMatrix2fv(f_uValue, 1, GL_FALSE, (GLfloat*)&f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::mat2> &f_value)
{
    glUniformMatrix2fv(f_uValue, f_value.size(), GL_FALSE, (GLfloat*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::mat3 &f_value)
{
    glUniformMatrix3fv(f_uValue, 1, GL_FALSE, (GLfloat*)&f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::mat3> &f_value)
{
    glUniformMatrix3fv(f_uValue, f_value.size(), GL_FALSE, (GLfloat*)f_value.data());
}
void ROC::Shader::SetUniformValue(GLint f_uValue, glm::mat4 &f_value)
{
    glUniformMatrix4fv(f_uValue, 1, GL_FALSE, (GLfloat*)&f_value);
}
void ROC::Shader::SetUniformValue(GLint f_uValue, std::vector<glm::mat4> &f_value)
{
    glUniformMatrix4fv(f_uValue, f_value.size(), GL_FALSE, (GLfloat*)f_value.data());
}

void ROC::Shader::SetProjectionUniformValue(glm::mat4 &f_value)
{
    if(m_projectionUniform == -1) return;
    if(!std::memcmp(&m_projectionUniformValue, &f_value, sizeof(glm::mat4))) return;
    std::memcpy(&m_projectionUniformValue, &f_value, sizeof(glm::mat4));
    SetUniformValue(m_projectionUniform, m_projectionUniformValue);
}
void ROC::Shader::SetViewUniformValue(glm::mat4 &f_value)
{
    if(m_viewUniform == -1) return;
    if(!std::memcmp(&m_viewUniformValue, &f_value, sizeof(glm::mat4))) return;
    std::memcpy(&m_viewUniformValue, &f_value, sizeof(glm::mat4));
    SetUniformValue(m_viewUniform, m_viewUniformValue);
}
void ROC::Shader::SetModelUniformValue(glm::mat4 &f_value)
{
    if(m_modelUniform == -1) return;
    if(!std::memcmp(&m_modelUniformValue, &f_value, sizeof(glm::mat4))) return;
    std::memcpy(&m_modelUniformValue, &f_value, sizeof(glm::mat4));
    SetUniformValue(m_modelUniform, m_modelUniformValue);
}
void ROC::Shader::SetCameraPositionUniformValue(glm::vec3 &f_value)
{
    if(m_cameraPositionUniform == -1) return;
    if(!std::memcmp(&m_cameraPositionUniformValue, &f_value, sizeof(glm::vec3))) return;
    std::memcpy(&m_cameraPositionUniformValue, &f_value, sizeof(glm::vec3));
    SetUniformValue(m_cameraPositionUniform, m_cameraPositionUniformValue);
}
void ROC::Shader::SetCameraDirectionUniformValue(glm::vec3 &f_value)
{
    if(m_cameraDirectionUniform == -1) return;
    if(!std::memcmp(&m_cameraDirectionUniformValue, &f_value, sizeof(glm::vec3))) return;
    std::memcpy(&m_cameraDirectionUniformValue, &f_value, sizeof(glm::vec3));
    SetUniformValue(m_cameraDirectionUniform, m_cameraDirectionUniformValue);
}
void ROC::Shader::SetLightColorUniformValue(glm::vec4 &f_value)
{
    if(m_lightColorUniform == -1) return;
    if(!std::memcmp(&m_lightColorUniformValue, &f_value, sizeof(glm::vec4))) return;
    std::memcpy(&m_lightColorUniformValue, &f_value, sizeof(glm::vec4));
    SetUniformValue(m_lightColorUniform, m_lightColorUniformValue);
}
void ROC::Shader::SetLightDirectionUniformValue(glm::vec3 &f_value)
{
    if(m_lightDirectionUniform == -1) return;
    if(!std::memcmp(&m_lightDirectionUniformValue, &f_value, sizeof(glm::vec3))) return;
    std::memcpy(&m_lightDirectionUniformValue, &f_value, sizeof(glm::vec3));
    SetUniformValue(m_lightDirectionUniform, m_lightDirectionUniformValue);
}
void ROC::Shader::SetLightParamUniformValue(glm::vec4 &f_value)
{
    if(m_lightParamUniform == -1) return;
    if(!std::memcmp(&m_lightParamUniformValue, &f_value, sizeof(glm::vec4))) return;
    std::memcpy(&m_lightParamUniformValue, &f_value, sizeof(glm::vec4));
    SetUniformValue(m_lightParamUniform, m_lightParamUniformValue);
}
void ROC::Shader::SetMaterialParamUniformValue(glm::vec4 &f_value)
{
    if(m_materialParamUniform == -1) return;
    if(!std::memcmp(&m_materialParamUniformValue, &f_value, sizeof(glm::vec4))) return;
    std::memcpy(&m_materialParamUniformValue, &f_value, sizeof(glm::vec4));
    SetUniformValue(m_materialParamUniform, m_materialParamUniformValue);
}
void ROC::Shader::SetMaterialTypeUniformValue(int f_value)
{
    if(m_materialTypeUniform == -1) return;
    if(m_materialTypeUniformValue == f_value) return;
    m_materialTypeUniformValue = f_value;
    SetUniformValue(m_materialTypeUniform, m_materialTypeUniformValue);
}
void ROC::Shader::SetAnimatedUniformValue(unsigned int f_value)
{
    if(m_animatedUniform == -1) return;
    if(m_animatedUniformValue == f_value) return;
    m_animatedUniformValue = f_value;
    SetUniformValue(m_animatedUniform, m_animatedUniformValue);
}
void ROC::Shader::SetBonesUniformValue(std::vector<glm::mat4> &f_value)
{
    if(m_bonesUBO == 0xFFFFFFFF) return;
    size_t l_vectorSize = f_value.size();
    if(l_vectorSize > MAX_BONES_PER_MODEL) return;
    glBindBuffer(GL_UNIFORM_BUFFER, m_bonesUBO);
    void *l_data = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    std::memcpy(l_data, f_value.data(), l_vectorSize*sizeof(glm::mat4));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}
void ROC::Shader::SetTimeUniformValue(float f_value)
{
    if(m_timeUniform == -1) return;
    if(m_timeUniformValue == f_value) return;
    m_timeUniformValue = f_value;
    SetUniformValue(m_timeUniform, m_timeUniformValue);
}
void ROC::Shader::SetColorUniformValue(glm::vec4 &f_value)
{
    if(m_colorUniform == -1) return;
    if(!std::memcmp(&f_value, &m_colorUniformValue, sizeof(glm::vec4))) return;
    std::memcpy(&m_colorUniformValue, &f_value, sizeof(glm::vec4));
    SetUniformValue(m_colorUniform, m_colorUniformValue);
}

bool ROC::Shader::Attach(Texture *f_texture, int f_uniform)
{
    for(auto iter : m_textureBind)
    {
        if(iter.m_texture == f_texture) return false;
    }
    int l_slot = m_bindPool->Allocate();
    if(l_slot == -1) return false;
    textureBindData l_bind{ f_texture, l_slot + 1, f_uniform };
    m_textureBind.push_back(l_bind);
    SetUniformValue(f_uniform, l_slot + 1);
    return true;
}
void ROC::Shader::Dettach(Texture *f_texture)
{
    for(unsigned int i = 0U, j = m_textureBind.size(); i < j; i++)
    {
        textureBindData &l_bind = m_textureBind[i];
        if(l_bind.m_texture == f_texture)
        {
            m_bindPool->Reset(static_cast<unsigned int>(l_bind.m_slot - 1));
            SetUniformValue(l_bind.m_uniform, -1);
            m_textureBind.erase(m_textureBind.begin() + i);
            break;
        }
    }
}

bool ROC::Shader::Attach(RenderTarget *f_target, int f_uniform)
{
    for(auto iter : m_targetBind)
    {
        if(iter.m_target == f_target) return false;
    }
    int l_slot = m_bindPool->Allocate();
    if(l_slot == -1) return false;
    targetBindData l_bind{ f_target, l_slot + 1, f_uniform };
    m_targetBind.push_back(l_bind);
    SetUniformValue(f_uniform, l_slot + 1);
    return true;
}
void ROC::Shader::Dettach(RenderTarget *f_target)
{
    for(unsigned int i = 0U, j = m_targetBind.size(); i < j; i++)
    {
        targetBindData &l_bind = m_targetBind[i];
        if(l_bind.m_target == f_target)
        {
            m_bindPool->Reset(static_cast<unsigned int>(l_bind.m_slot - 1));
            SetUniformValue(l_bind.m_uniform, -1);
            m_targetBind.erase(m_targetBind.begin() + i);
            break;
        }
    }
}

void ROC::Shader::Enable(bool f_textureBind)
{
    glUseProgram(m_program);
    if(f_textureBind)
    {
        for(auto iter : m_textureBind) iter.m_texture->Bind(iter.m_slot);
        for(auto iter : m_targetBind) iter.m_target->BindTexture(iter.m_slot);
    }
}