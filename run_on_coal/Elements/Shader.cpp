#include "stdafx.h"

#include "Elements/Shader.h"
#include "Elements/Drawable.h"
#include "Utils/Pool.h"

#include "Utils/EnumUtils.h"

#define ROC_SHADER_BONES_BINDPOINT 0

namespace ROC
{

const std::vector<std::string> g_DefaultUniformsTable = {
    "gProjectionMatrix", "gViewMatrix", "gModelMatrix", "gAnimated", "gBonesUniform", "gBoneMatrix",
    "gLightColor", "gLightDirection", "gLightParam",
    "gCameraPosition", "gCameraDirection",
    "gMaterialType", "gMaterialParam",
    "gTexture0", "gColor",
    "gTime"
};

}

GLuint ROC::Shader::ms_bonesUBO = GL_INVALID_INDEX;
bool ROC::Shader::ms_uboFix = false;

ROC::Shader::Shader()
{
    m_elementType = ElementType::ShaderElement;
    m_elementTypeName.assign("Shader");

    m_program = 0U;

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

    m_uniformMapEnd = m_uniformMap.end();

    m_bindPool = new Pool(31U);
    m_drawableCount = 0U;
}
ROC::Shader::~Shader()
{
    if(m_program) glDeleteProgram(m_program);
    delete m_bindPool;
    m_drawableBind.clear();
}

bool ROC::Shader::Load(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath)
{
    if(!m_program && !f_vpath.empty() && !f_fpath.empty())
    {
        std::ifstream l_file;

        GLuint l_vertexShader = 0U;
        l_file.open(f_vpath, std::ios::in);
        if(!l_file.fail())
        {
            std::string l_shaderData((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
            if(l_vertexShader)
            {
                const char *l_source = l_shaderData.c_str();
                glShaderSource(l_vertexShader, 1, &l_source, NULL);
                glCompileShader(l_vertexShader);

                GLint l_state;
                glGetShaderiv(l_vertexShader, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    glGetShaderiv(l_vertexShader, GL_INFO_LOG_LENGTH, &l_logSize);
                    m_error.resize(l_logSize);
                    glGetShaderInfoLog(l_vertexShader, l_logSize, &l_logSize, &m_error[0]);
                    m_error.insert(0U, "Vertex shader error: ");
                    glDeleteShader(l_vertexShader);
                    l_vertexShader = 0U;
                }
            }
        }
        else m_error.assign("Unable to load vertex shader");
        l_file.clear();

        GLuint l_fragmentShader = 0U;
        l_file.open(f_fpath, std::ios::in);
        if(!l_file.fail())
        {
            std::string l_shaderData((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            if(l_fragmentShader)
            {
                const char *l_source = l_shaderData.c_str();
                glShaderSource(l_fragmentShader, 1, &l_source, NULL);
                glCompileShader(l_fragmentShader);

                GLint l_state;
                glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    glGetShaderiv(l_fragmentShader, GL_INFO_LOG_LENGTH, &l_logSize);
                    m_error.resize(l_logSize);
                    glGetShaderInfoLog(l_fragmentShader, l_logSize, &l_logSize, &m_error[0]);
                    m_error.insert(0U, "Fragment shader error: ");
                    glDeleteShader(l_fragmentShader);
                    l_fragmentShader = 0U;
                }
            }
        }
        else m_error.assign("Unable to load fragment shader");
        l_file.clear();

        GLuint l_geometryShader = 0U;
        if(!f_gpath.empty())
        {
            l_file.open(f_gpath, std::ios::in);
            if(!l_file.fail())
            {
                std::string l_shaderData((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
                l_file.close();

                l_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
                if(l_geometryShader)
                {
                    const char *l_source = l_shaderData.c_str();
                    glShaderSource(l_geometryShader, 1, &l_source, NULL);
                    glCompileShader(l_geometryShader);

                    GLint l_state;
                    glGetShaderiv(l_geometryShader, GL_COMPILE_STATUS, &l_state);
                    if(!l_state)
                    {
                        GLint l_logSize = 0;
                        glGetShaderiv(l_geometryShader, GL_INFO_LOG_LENGTH, &l_logSize);
                        m_error.resize(l_logSize);
                        glGetShaderInfoLog(l_geometryShader, l_logSize, &l_logSize, &m_error[0]);
                        m_error.insert(0U, "Geometry shader error: ");
                        glDeleteShader(l_geometryShader);
                        l_geometryShader = 0U;
                    }
                }
            }
            l_file.clear();
        }

        if(m_error.empty())
        {
            m_program = glCreateProgram();
            if(m_program)
            {
                if(l_vertexShader) glAttachShader(m_program, l_vertexShader);
                if(l_fragmentShader) glAttachShader(m_program, l_fragmentShader);
                if(l_geometryShader) glAttachShader(m_program, l_geometryShader);

                GLint l_link = 0;
                glLinkProgram(m_program);
                glGetProgramiv(m_program, GL_LINK_STATUS, &l_link);
                if(!l_link)
                {
                    GLint l_logLength = 0;
                    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &l_logLength);
                    m_error.resize(l_logLength);
                    glGetProgramInfoLog(m_program, l_logLength, &l_logLength, &m_error[0]);
                    glDeleteProgram(m_program);
                    m_program = 0U;
                }
                else
                {
                    if(l_vertexShader) glDetachShader(m_program, l_vertexShader);
                    if(l_fragmentShader) glDetachShader(m_program, l_fragmentShader);
                    if(l_geometryShader) glDetachShader(m_program, l_geometryShader);
                    SetupDefaultUniformsAndLocations();
                }
            }
        }

        if(l_vertexShader) glDeleteShader(l_vertexShader);
        if(l_fragmentShader) glDeleteShader(l_fragmentShader);
        if(l_geometryShader) glDeleteShader(l_geometryShader);
    }
    return (m_program != 0U);
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
    if(l_boneUniform != GL_INVALID_INDEX) glUniformBlockBinding(m_program, l_boneUniform, ROC_SHADER_BONES_BINDPOINT);
    //Samplers
    m_texture0Uniform = glGetUniformLocation(m_program, "gTexture0");
    if(m_texture0Uniform != -1) glUniform1i(m_texture0Uniform, 0);
    //Time
    m_timeUniform = glGetUniformLocation(m_program, "gTime");
    //Color (RenderTarget, Texture, Font)
    m_colorUniform = glGetUniformLocation(m_program, "gColor");

    //Get list of custom uniforms
    std::vector<GLchar> l_uniformName(256U);
    int l_activeUniformCount = 0;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &l_activeUniformCount);
    for(int i = 0; i < l_activeUniformCount; i++)
    {
        int l_uniformNameSize, l_uniformSize;
        GLenum l_uniformType;
        glGetActiveUniform(m_program, static_cast<GLuint>(i), 256, &l_uniformNameSize, &l_uniformSize, &l_uniformType, l_uniformName.data());

        std::string l_uniformNameString(l_uniformName.data(), l_uniformNameSize);
        if(EnumUtils::ReadEnumVector(l_uniformNameString, g_DefaultUniformsTable) == -1)
        {
            GLint l_uniformLocation = glGetUniformLocation(m_program, l_uniformNameString.c_str());
            m_uniformMap.insert(std::make_pair(l_uniformNameString, l_uniformLocation));
        }
    }
    m_uniformMapEnd = m_uniformMap.end();
}

void ROC::Shader::SetUniformValue(const std::string &f_uniform, unsigned int f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform1ui(l_result->second, f_value);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::uvec2 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform2ui(l_result->second, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::uvec3 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform3ui(l_result->second, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::uvec4 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform4ui(l_result->second, f_value.x, f_value.y, f_value.z, f_value.w);
}

void ROC::Shader::SetUniformValue(const std::string &f_uniform, int f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform1i(l_result->second, f_value);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::ivec2 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform2i(l_result->second, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::ivec3 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform3i(l_result->second, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::ivec4 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform4i(l_result->second, f_value.x, f_value.y, f_value.z, f_value.w);
}

void ROC::Shader::SetUniformValue(const std::string &f_uniform, float f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform1f(l_result->second, f_value);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::vec2 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform2f(l_result->second, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::vec3 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform3f(l_result->second, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::vec4 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform4f(l_result->second, f_value.x, f_value.y, f_value.z, f_value.w);
}

void ROC::Shader::SetUniformValue(const std::string &f_uniform, double f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform1d(l_result->second, f_value);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::dvec2 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform2d(l_result->second, f_value.x, f_value.y);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::dvec3 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform3d(l_result->second, f_value.x, f_value.y, f_value.z);
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::dvec4 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniform4d(l_result->second, f_value.x, f_value.y, f_value.z, f_value.w);
}

void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::mat2 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniformMatrix2fv(l_result->second, 1, GL_FALSE, glm::value_ptr(f_value));
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::mat3 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniformMatrix3fv(l_result->second, 1, GL_FALSE, glm::value_ptr(f_value));
}
void ROC::Shader::SetUniformValue(const std::string &f_uniform, const glm::mat4 &f_value)
{
    auto l_result = m_uniformMap.find(f_uniform);
    if(l_result != m_uniformMapEnd) glUniformMatrix4fv(l_result->second, 1, GL_FALSE, glm::value_ptr(f_value));
}

void ROC::Shader::SetProjectionUniformValue(const glm::mat4 &f_value)
{
    if(m_projectionUniform != -1)
    {
        if(std::memcmp(&m_projectionUniformValue, &f_value, sizeof(glm::mat4)) != 0)
        {
            std::memcpy(&m_projectionUniformValue, &f_value, sizeof(glm::mat4));
            glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(m_projectionUniformValue));
        }
    }
}
void ROC::Shader::SetViewUniformValue(const glm::mat4 &f_value)
{
    if(m_viewUniform != -1)
    {
        if(std::memcmp(&m_viewUniformValue, &f_value, sizeof(glm::mat4)) != 0)
        {
            std::memcpy(&m_viewUniformValue, &f_value, sizeof(glm::mat4));
            glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(m_viewUniformValue));
        }
    }
}
void ROC::Shader::SetModelUniformValue(const glm::mat4 &f_value)
{
    if(m_modelUniform != -1)
    {
        if(std::memcmp(&m_modelUniformValue, &f_value, sizeof(glm::mat4)) != 0)
        {
            std::memcpy(&m_modelUniformValue, &f_value, sizeof(glm::mat4));
            glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, glm::value_ptr(m_modelUniformValue));
        }
    }
}
void ROC::Shader::SetCameraPositionUniformValue(const glm::vec3 &f_value)
{
    if(m_cameraPositionUniform != -1)
    {
        if(m_cameraPositionUniformValue != f_value)
        {
            std::memcpy(&m_cameraPositionUniformValue, &f_value, sizeof(glm::vec3));
            glUniform3f(m_cameraPositionUniform, m_cameraPositionUniformValue.x, m_cameraPositionUniformValue.y, m_cameraPositionUniformValue.z);
        }
    }
}
void ROC::Shader::SetCameraDirectionUniformValue(const glm::vec3 &f_value)
{
    if(m_cameraDirectionUniform != -1)
    {
        if(m_cameraDirectionUniformValue != f_value)
        {
            std::memcpy(&m_cameraDirectionUniformValue, &f_value, sizeof(glm::vec3));
            glUniform3f(m_cameraDirectionUniform, m_cameraDirectionUniformValue.x, m_cameraDirectionUniformValue.y, m_cameraDirectionUniformValue.z);
        }
    }
}
void ROC::Shader::SetLightColorUniformValue(const glm::vec4 &f_value)
{
    if(m_lightColorUniform != -1)
    {
        if(m_lightColorUniformValue != f_value)
        {
            std::memcpy(&m_lightColorUniformValue, &f_value, sizeof(glm::vec4));
            glUniform4f(m_lightColorUniform, m_lightColorUniformValue.r, m_lightColorUniformValue.g, m_lightColorUniformValue.b, m_lightColorUniformValue.a);
        }
    }
}
void ROC::Shader::SetLightDirectionUniformValue(const glm::vec3 &f_value)
{
    if(m_lightDirectionUniform != -1)
    {
        if(m_lightDirectionUniformValue != f_value)
        {
            std::memcpy(&m_lightDirectionUniformValue, &f_value, sizeof(glm::vec3));
            glUniform3f(m_lightDirectionUniform, m_lightDirectionUniformValue.x, m_lightDirectionUniformValue.y, m_lightDirectionUniformValue.z);
        }
    }
}
void ROC::Shader::SetLightParamUniformValue(const glm::vec4 &f_value)
{
    if(m_lightParamUniform != -1)
    {
        if(m_lightParamUniformValue != f_value)
        {
            std::memcpy(&m_lightParamUniformValue, &f_value, sizeof(glm::vec4));
            glUniform4f(m_lightParamUniform, m_lightParamUniformValue.x, m_lightParamUniformValue.y, m_lightParamUniformValue.z, m_lightParamUniformValue.w);
        }
    }
}
void ROC::Shader::SetMaterialParamUniformValue(const glm::vec4 &f_value)
{
    if(m_materialParamUniform != -1)
    {
        if(m_materialParamUniformValue != f_value)
        {
            std::memcpy(&m_materialParamUniformValue, &f_value, sizeof(glm::vec4));
            glUniform4f(m_materialParamUniform, m_materialParamUniformValue.x, m_materialParamUniformValue.y, m_materialParamUniformValue.z, m_materialParamUniformValue.w);
        }
    }
}
void ROC::Shader::SetMaterialTypeUniformValue(int f_value)
{
    if(m_materialTypeUniform != -1)
    {
        if(m_materialTypeUniformValue != f_value)
        {
            m_materialTypeUniformValue = f_value;
            glUniform1i(m_materialTypeUniform, m_materialTypeUniformValue);
        }
    }
}
void ROC::Shader::SetAnimatedUniformValue(unsigned int f_value)
{
    if(m_animatedUniform != -1)
    {
        if(m_animatedUniformValue != f_value)
        {
            m_animatedUniformValue = f_value;
            glUniform1ui(m_animatedUniform, m_animatedUniformValue);
        }
    }
}
void ROC::Shader::SetBonesUniformValue(const std::vector<glm::mat4> &f_value)
{
    if(ms_bonesUBO != GL_INVALID_INDEX)
    {
        if(ms_uboFix) glFinish();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, f_value.size()*sizeof(glm::mat4), f_value.data());
    }
}
void ROC::Shader::SetTimeUniformValue(float f_value)
{
    if(m_timeUniform != -1)
    {
        if(m_timeUniformValue != f_value)
        {
            m_timeUniformValue = f_value;
            glUniform1f(m_timeUniform, m_timeUniformValue);
        }
    }
}
void ROC::Shader::SetColorUniformValue(const glm::vec4 &f_value)
{
    if(m_colorUniform != -1)
    {
        if(m_colorUniformValue != f_value)
        {
            std::memcpy(&m_colorUniformValue, &f_value, sizeof(glm::vec4));
            glUniform4f(m_colorUniform, m_colorUniformValue.r, m_colorUniformValue.g, m_colorUniformValue.b, m_colorUniformValue.a);
        }
    }
}

bool ROC::Shader::Attach(Drawable *f_drawable, const std::string &f_uniform)
{
    bool l_result = false;
    auto l_mapResult = m_uniformMap.find(f_uniform);
    if(l_mapResult != m_uniformMapEnd)
    {
        int l_slot = m_bindPool->Allocate();
        if(l_slot != -1)
        {
            drawableBindData l_bind{ f_drawable, l_slot + 1, l_mapResult->second };
            m_drawableBind.push_back(l_bind);
            m_drawableCount++;
            glUniform1i(l_bind.m_uniform, l_slot + 1);
            l_result = true;
        }
    }
    return l_result;
}
bool ROC::Shader::Detach(Drawable *f_drawable)
{
    bool l_result = false;
    for(auto iter = m_drawableBind.begin(), iterEnd = m_drawableBind.end(); iter != iterEnd; ++iter)
    {
        if(iter->m_element == f_drawable)
        {
            m_bindPool->Reset(static_cast<unsigned int>(iter->m_slot - 1));
            glUniform1i(iter->m_uniform, 0);

            m_drawableBind.erase(iter);
            m_drawableCount--;
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::Shader::HasAttached(Drawable *f_drawable)
{
    bool l_result = false;
    for(auto &iter : m_drawableBind)
    {
        if(iter.m_element == f_drawable)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}

void ROC::Shader::CreateBonesUBO()
{
    if(ms_bonesUBO == GL_INVALID_INDEX)
    {
        glGenBuffers(1, &ms_bonesUBO);
        glBindBufferBase(GL_UNIFORM_BUFFER, ROC_SHADER_BONES_BINDPOINT, ms_bonesUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * ROC_SHADER_BONES_COUNT, NULL, GL_DYNAMIC_DRAW);
    }
}
void ROC::Shader::DestroyBonesUBO()
{
    if(ms_bonesUBO != GL_INVALID_INDEX)
    {
        glDeleteBuffers(1, &ms_bonesUBO);
        ms_bonesUBO = GL_INVALID_INDEX;
    }
}
void ROC::Shader::EnableUBOFix()
{
    ms_uboFix = true;
}

void ROC::Shader::Enable(bool f_full)
{
    glUseProgram(m_program);
    if(f_full)
    {
        for(unsigned int i = 0; i < m_drawableCount; i++)
        {
            glActiveTexture(GL_TEXTURE1 + i);
            m_drawableBind[i].m_element->Bind();
        }
        if(m_drawableCount > 0U) glActiveTexture(GL_TEXTURE0);
    }
}
