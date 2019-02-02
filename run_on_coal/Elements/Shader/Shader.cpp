#include "stdafx.h"

#include "Elements/Shader/Shader.h"
#include "Elements/Shader/ShaderUniform.h"
#include "Elements/Drawable.h"
#include "Utils/Pool.h"

#include "Utils/EnumUtils.h"
#include "Utils/GLBinder.h"

namespace ROC
{

const std::vector<std::string> g_DefaultUniformsTable = {
    "gProjectionMatrix", "gViewMatrix", "gViewProjectionMatrix", "gModelMatrix", "gAnimated", "gBonesUniform", "gBoneMatrix", "gBoneMatrix[0]",
    "gLightColor", "gLightDirection", "gLightParam",
    "gCameraPosition", "gCameraDirection",
    "gSkyGradientDown", "gSkyGradientUp",
    "gMaterialType", "gMaterialParam",
    "gTexture0", "gColor",
    "gTime"
};

}

#define ROC_SHADER_BONES_BINDPOINT 0U
#if defined _WIN64
#define ROC_SHADER_BONES_COUNT 227ULL
#elif defined _WIN32
#define ROC_SHADER_BONES_COUNT 227U
#else
#define ROC_SHADER_BONES_COUNT 227
#endif

int ROC::Shader::ms_drawableMaxCount = 0;
GLuint ROC::Shader::ms_bonesUBO = 0U;
bool ROC::Shader::ms_uboFix = false;

ROC::Shader::Shader()
{
    m_elementType = ET_Shader;
    m_elementTypeName.assign("Shader");

    m_program = 0U;

    for(size_t i = 0U; i < SDU_UniformCount; i++) m_defaultUniforms[i] = nullptr;

    m_uniformMapEnd = m_uniformMap.end();

    m_bindPool = new Pool(static_cast<size_t>(ms_drawableMaxCount));
}
ROC::Shader::~Shader()
{
    if(m_program)
    {
        GLBinder::ResetShaderProgram(m_program);
        glDeleteProgram(m_program);
    }
    for(auto iter : m_defaultUniforms) delete iter;
    for(auto &iter : m_uniformMap) delete iter.second;
    m_uniformMap.clear();
    delete m_bindPool;
    m_drawableBind.clear();
}

bool ROC::Shader::Load(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath)
{
    if(!m_program)
    {
        std::ifstream l_file;

        GLuint l_vertexShader = 0U;
        l_file.open(f_vpath, std::ios::in);
        if(!l_file.fail())
        {
            std::string l_shaderData((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            if(!l_shaderData.empty())
            {
                l_vertexShader = glCreateShader(GL_VERTEX_SHADER);
                if(l_vertexShader)
                {
                    const char *l_source = l_shaderData.data();
                    int l_sourceLength = static_cast<int>(l_shaderData.length());
                    glShaderSource(l_vertexShader, 1, &l_source, &l_sourceLength);
                    glCompileShader(l_vertexShader);

                    GLint l_state;
                    glGetShaderiv(l_vertexShader, GL_COMPILE_STATUS, &l_state);
                    if(!l_state)
                    {
                        GLint l_logSize = 0;
                        glGetShaderiv(l_vertexShader, GL_INFO_LOG_LENGTH, &l_logSize);
                        m_error.resize(static_cast<size_t>(l_logSize));
                        glGetShaderInfoLog(l_vertexShader, l_logSize, &l_logSize, &m_error[0]);
                        m_error.insert(0U, "Vertex shader error: ");
                        glDeleteShader(l_vertexShader);
                        l_vertexShader = 0U;
                    }
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

            if(!l_shaderData.empty())
            {
                l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                if(l_fragmentShader)
                {
                    const char *l_source = l_shaderData.data();
                    int l_sourceLength = static_cast<int>(l_shaderData.length());
                    glShaderSource(l_fragmentShader, 1, &l_source, &l_sourceLength);
                    glCompileShader(l_fragmentShader);

                    GLint l_state;
                    glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &l_state);
                    if(!l_state)
                    {
                        GLint l_logSize = 0;
                        glGetShaderiv(l_fragmentShader, GL_INFO_LOG_LENGTH, &l_logSize);
                        m_error.resize(static_cast<size_t>(l_logSize));
                        glGetShaderInfoLog(l_fragmentShader, l_logSize, &l_logSize, &m_error[0]);
                        m_error.insert(0U, "Fragment shader error: ");
                        glDeleteShader(l_fragmentShader);
                        l_fragmentShader = 0U;
                    }
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

                if(!l_shaderData.empty())
                {
                    l_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
                    if(l_geometryShader)
                    {
                        const char *l_source = l_shaderData.data();
                        int l_sourceLength = static_cast<int>(l_shaderData.length());
                        glShaderSource(l_geometryShader, 1, &l_source, &l_sourceLength);
                        glCompileShader(l_geometryShader);

                        GLint l_state;
                        glGetShaderiv(l_geometryShader, GL_COMPILE_STATUS, &l_state);
                        if(!l_state)
                        {
                            GLint l_logSize = 0;
                            glGetShaderiv(l_geometryShader, GL_INFO_LOG_LENGTH, &l_logSize);
                            m_error.resize(static_cast<size_t>(l_logSize));
                            glGetShaderInfoLog(l_geometryShader, l_logSize, &l_logSize, &m_error[0]);
                            m_error.insert(0U, "Geometry shader error: ");
                            glDeleteShader(l_geometryShader);
                            l_geometryShader = 0U;
                        }
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
                    m_error.resize(static_cast<size_t>(l_logLength));
                    glGetProgramInfoLog(m_program, l_logLength, &l_logLength, &m_error[0]);
                    glDeleteProgram(m_program);
                    m_program = 0U;
                }
                else
                {
                    if(l_vertexShader) glDetachShader(m_program, l_vertexShader);
                    if(l_fragmentShader) glDetachShader(m_program, l_fragmentShader);
                    if(l_geometryShader) glDetachShader(m_program, l_geometryShader);

                    GLint l_lastProgram = GLBinder::GetUsedShaderProgram();
                    SetupUniformsAndLocations();
                    GLBinder::UseShaderProgram(l_lastProgram);
                }
            }
        }

        if(l_vertexShader) glDeleteShader(l_vertexShader);
        if(l_fragmentShader) glDeleteShader(l_fragmentShader);
        if(l_geometryShader) glDeleteShader(l_geometryShader);
    }
    return (m_program != 0U);
}

void ROC::Shader::SetupUniformsAndLocations()
{
    glBindAttribLocation(m_program, 0, "gVertexPosition");
    glBindAttribLocation(m_program, 1, "gVertexNormal");
    glBindAttribLocation(m_program, 2, "gVertexUV");
    glBindAttribLocation(m_program, 3, "gVertexWeight");
    glBindAttribLocation(m_program, 4, "gVertexIndex");

    GLBinder::UseShaderProgram(m_program);

    // Matrices
    FindDefaultUniform(SDU_Projection, "gProjectionMatrix", ShaderUniform::SUT_Mat4);
    FindDefaultUniform(SDU_View, "gViewMatrix", ShaderUniform::SUT_Mat4);
    FindDefaultUniform(SDU_ViewProjection, "gViewProjectionMatrix", ShaderUniform::SUT_Mat4);
    FindDefaultUniform(SDU_Model, "gModelMatrix", ShaderUniform::SUT_Mat4);
    // Camera
    FindDefaultUniform(SDU_CameraPosition, "gCameraPosition", ShaderUniform::SUT_Float3);
    FindDefaultUniform(SDU_CameraDirection, "gCameraDirection", ShaderUniform::SUT_Float3);
    // Light
    FindDefaultUniform(SDU_LightColor, "gLightColor", ShaderUniform::SUT_Float4);
    FindDefaultUniform(SDU_LightDirection, "gLightDirection", ShaderUniform::SUT_Float3);
    FindDefaultUniform(SDU_LightParam, "gLightParam", ShaderUniform::SUT_Float4);
    // Sky
    FindDefaultUniform(SDU_SkyGradientDown, "gSkyGradientDown", ShaderUniform::SUT_Float3);
    FindDefaultUniform(SDU_SkyGradientUp, "gSkyGradientUp", ShaderUniform::SUT_Float3);
    // Material
    FindDefaultUniform(SDU_MaterialParam, "gMaterialParam", ShaderUniform::SUT_Float4);
    FindDefaultUniform(SDU_MaterialType, "gMaterialType", ShaderUniform::SUT_Int);
    // Animation
    FindDefaultUniform(SDU_Animated, "gAnimated", ShaderUniform::SUT_Bool);
    unsigned int l_boneUniform = glGetUniformBlockIndex(m_program, "gBonesUniform");
    if(l_boneUniform != 0U) glUniformBlockBinding(m_program, l_boneUniform, ROC_SHADER_BONES_BINDPOINT);
    // Samplers
    FindDefaultUniform(SDU_DiffuseTexture, "gTexture0", ShaderUniform::SUT_Sampler);
    if(m_defaultUniforms[SDU_DiffuseTexture]) m_defaultUniforms[SDU_DiffuseTexture]->SetSampler(0);
    // Time
    FindDefaultUniform(SDU_Time, "gTime", ShaderUniform::SUT_Float);
    // Color (RenderTarget, Texture, Font)
    FindDefaultUniform(SDU_Color, "gColor", ShaderUniform::SUT_Float4);

    // Get list of custom uniforms
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
            ShaderUniform *l_uniform = new ShaderUniform(l_uniformType, l_uniformLocation);
            m_uniformMap.insert(std::make_pair(l_uniformNameString, l_uniform));
        }
    }
    m_uniformMapEnd = m_uniformMap.end();
}
void ROC::Shader::FindDefaultUniform(ShaderDefaultUniform f_sud, const char *f_name, unsigned int f_type)
{
    int l_tempUniform = glGetUniformLocation(m_program, f_name);
    if(l_tempUniform != -1) m_defaultUniforms[f_sud] = new ShaderUniform(f_type, l_tempUniform);
}

ROC::ShaderUniform* ROC::Shader::GetUniform(const std::string &f_uniform)
{
    ShaderUniform *l_shaderUniform = nullptr;
    auto iter = m_uniformMap.find(f_uniform);
    if(iter != m_uniformMapEnd) l_shaderUniform = iter->second;
    return l_shaderUniform;
}

void ROC::Shader::SetProjectionMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_Projection]) m_defaultUniforms[SDU_Projection]->SetValue(f_value);
}
void ROC::Shader::SetViewMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_View]) m_defaultUniforms[SDU_View]->SetValue(f_value);
}
void ROC::Shader::SetViewProjectionMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_ViewProjection]) m_defaultUniforms[SDU_ViewProjection]->SetValue(f_value);
}
void ROC::Shader::SetModelMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_Model]) m_defaultUniforms[SDU_Model]->SetValue(f_value);
}
void ROC::Shader::SetCameraPosition(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_CameraPosition]) m_defaultUniforms[SDU_CameraPosition]->SetValue(f_value);
}
void ROC::Shader::SetCameraDirection(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_CameraDirection]) m_defaultUniforms[SDU_CameraDirection]->SetValue(f_value);
}
void ROC::Shader::SetLightColor(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_LightColor]) m_defaultUniforms[SDU_LightColor]->SetValue(f_value);
}
void ROC::Shader::SetLightDirection(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_LightDirection]) m_defaultUniforms[SDU_LightDirection]->SetValue(f_value);
}
void ROC::Shader::SetLightParam(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_LightParam]) m_defaultUniforms[SDU_LightParam]->SetValue(f_value);
}
void ROC::Shader::SetSkyGradientDown(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_SkyGradientDown]) m_defaultUniforms[SDU_SkyGradientDown]->SetValue(f_value);
}
void ROC::Shader::SetSkyGradientUp(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_SkyGradientUp]) m_defaultUniforms[SDU_SkyGradientUp]->SetValue(f_value);
}
void ROC::Shader::SetMaterialParam(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_MaterialParam]) m_defaultUniforms[SDU_MaterialParam]->SetValue(f_value);
}
void ROC::Shader::SetMaterialType(int f_value)
{
    if(m_defaultUniforms[SDU_MaterialType]) m_defaultUniforms[SDU_MaterialType]->SetValue(f_value);
}
void ROC::Shader::SetAnimated(bool f_value)
{
    if(m_defaultUniforms[SDU_Animated]) m_defaultUniforms[SDU_Animated]->SetValue(f_value);
}
void ROC::Shader::SetBoneMatrices(const std::vector<glm::mat4> &f_value)
{
    if(ms_bonesUBO != 0U)
    {
        if(ms_uboFix) glFinish();
        GLsizeiptr l_matrixCount = std::min(f_value.size(), ROC_SHADER_BONES_COUNT);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, l_matrixCount*sizeof(glm::mat4), f_value.data());
    }
}
void ROC::Shader::SetTime(float f_value)
{
    if(m_defaultUniforms[SDU_Time]) m_defaultUniforms[SDU_Time]->SetValue(f_value);
}
void ROC::Shader::SetColor(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_Color]) m_defaultUniforms[SDU_Color]->SetValue(f_value);
}

bool ROC::Shader::Attach(Drawable *f_drawable, const std::string &f_uniform)
{
    bool l_result = false;
    auto l_mapResult = m_uniformMap.find(f_uniform);
    if(l_mapResult != m_uniformMapEnd)
    {
        ShaderUniform *l_shaderUniform = l_mapResult->second;
        bool l_isUsed = false;
        for(const auto &iter : m_drawableBind)
        {
            if(iter.m_uniform == l_shaderUniform)
            {
                l_isUsed = true;
                break;
            }
        }
        if(!l_isUsed)
        {
            if((l_shaderUniform->IsSampler2D() && !f_drawable->IsCubic()) || (l_shaderUniform->IsSamplerCube() && f_drawable->IsCubic()))
            {
                int l_slot = m_bindPool->Allocate();
                if(l_slot != -1)
                {
                    drawableBindData l_bind{ f_drawable, l_slot + 1, l_shaderUniform };
                    m_drawableBind.push_back(l_bind);
                    l_shaderUniform->SetSampler(l_slot + 1);
                    l_result = true;
                }
            }
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
            m_bindPool->Reset(static_cast<size_t>(iter->m_slot - 1));
            iter->m_uniform->SetSampler(0);
            m_drawableBind.erase(iter);
            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::Shader::HasAttached(Drawable *f_drawable) const
{
    bool l_result = false;
    for(const auto &iter : m_drawableBind)
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
    if(ms_bonesUBO == 0U)
    {
        glGenBuffers(1, &ms_bonesUBO);
        glBindBufferBase(GL_UNIFORM_BUFFER, ROC_SHADER_BONES_BINDPOINT, ms_bonesUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * ROC_SHADER_BONES_COUNT, NULL, GL_DYNAMIC_DRAW);
    }
}
void ROC::Shader::DestroyBonesUBO()
{
    if(ms_bonesUBO != 0U)
    {
        glDeleteBuffers(1, &ms_bonesUBO);
        ms_bonesUBO = 0U;
    }
}
void ROC::Shader::EnableUBOFix()
{
    ms_uboFix = true;
}
void ROC::Shader::UpdateDrawableMaxCount()
{
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ms_drawableMaxCount);
    ms_drawableMaxCount--; // Slot 0 is reserved for texture of drawn material
}

void ROC::Shader::Enable()
{
    GLBinder::UseShaderProgram(m_program);
    for(auto iter : m_defaultUniforms)
    {
        if(iter)
        {
            iter->SetActive(true);
            iter->Update();
        }
    }
    for(auto &iter : m_uniformMap)
    {
        ShaderUniform *l_shaderUniform = iter.second;
        l_shaderUniform->SetActive(true);
        l_shaderUniform->Update();
    }
    if(!m_drawableBind.empty())
    {
        unsigned int l_slot = 0U;
        for(auto &iter : m_drawableBind)
        {
            glActiveTexture(GL_TEXTURE1 + l_slot);
            iter.m_element->Bind();
            l_slot++;
        }
        glActiveTexture(GL_TEXTURE0);
    }
}

void ROC::Shader::Disable()
{
    for(auto iter : m_defaultUniforms)
    {
        if(iter) iter->SetActive(false);
    }
    for(auto &iter : m_uniformMap) iter.second->SetActive(false);
}
