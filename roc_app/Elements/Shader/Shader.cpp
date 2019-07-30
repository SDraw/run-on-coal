#include "stdafx.h"

#include "Elements/Shader/Shader.h"
#include "Elements/Shader/ShaderUniform.h"
#include "Elements/Drawable.h"
#include "Elements/Light.h"
#include "Utils/Pool.h"

#include "Utils/EnumUtils.h"
#include "Utils/GLBinder.h"

#define ROC_SHADER_BONES_COUNT 227
#define ROC_SHADER_LIGHTS_COUNT 4

namespace ROC
{

const std::vector<std::string> g_DefaultUniforms
{
    "gProjectionMatrix", "gViewMatrix", "gViewProjectionMatrix", "gModelMatrix", "gAnimated", "gBoneMatrix",
    "gCameraPosition", "gCameraDirection",
    "gLightData", "gLightsCount",
    "gMaterialType", "gMaterialParam",
    "gTexture0", "gColor",
    "gTime"
};

const std::string g_DefaultShaderDefines = (std::string() +
    "#version 330 core\n" +
    "#define MAX_BONES " + std::to_string(ROC_SHADER_BONES_COUNT) + '\n' +
    "#define MAX_LIGHTS " + std::to_string(ROC_SHADER_LIGHTS_COUNT) + '\n' +
    "#define LIGHT_DIRECTIONAL " + std::to_string(ROC::Light::LT_Directional) + '\n' +
    "#define LIGHT_POINT " + std::to_string(ROC::Light::LT_Point) + '\n' +
    "#define LIGHT_SPOTLIGHT " + std::to_string(ROC::Light::LT_Spotlight) + '\n'
    );

}

int ROC::Shader::ms_drawableMaxCount = 0;

ROC::Shader::Shader()
{
    m_elementType = ET_Shader;

    m_program = 0U;
    m_active = false;

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
    for(auto l_uniform : m_defaultUniforms) delete l_uniform;
    for(auto &l_uniformPair : m_uniformMap) delete l_uniformPair.second;
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

            l_shaderData.insert(l_shaderData.begin(), g_DefaultShaderDefines.begin(), g_DefaultShaderDefines.end());

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

            l_shaderData.insert(l_shaderData.begin(), g_DefaultShaderDefines.begin(), g_DefaultShaderDefines.end());

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

                l_shaderData.insert(l_shaderData.begin(), g_DefaultShaderDefines.begin(), g_DefaultShaderDefines.end());

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
    FindDefaultUniform(SDU_BoneMatrices, "gBoneMatrix", ShaderUniform::SUT_Mat4);

    // Camera
    FindDefaultUniform(SDU_CameraPosition, "gCameraPosition", ShaderUniform::SUT_Float3);
    FindDefaultUniform(SDU_CameraDirection, "gCameraDirection", ShaderUniform::SUT_Float3);
    // Light
    FindDefaultUniform(SDU_LightData, "gLightData", ShaderUniform::SUT_Mat4); // Array
    FindDefaultUniform(SDU_LightsCount, "gLightsCount", ShaderUniform::SUT_Int);
    // Material
    FindDefaultUniform(SDU_MaterialParam, "gMaterialParam", ShaderUniform::SUT_Float4);
    FindDefaultUniform(SDU_MaterialType, "gMaterialType", ShaderUniform::SUT_Int);
    // Animation
    FindDefaultUniform(SDU_Animated, "gAnimated", ShaderUniform::SUT_Bool);
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
        if(EnumUtils::ReadEnumVector(l_uniformNameString, g_DefaultUniforms) == -1)
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
    auto l_uniformPair = m_uniformMap.find(f_uniform);
    if(l_uniformPair != m_uniformMapEnd) l_shaderUniform = l_uniformPair->second;
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
void ROC::Shader::SetLightsData(const std::vector<Light*> &f_data)
{
    size_t l_count = std::min(f_data.size(), static_cast<size_t>(ROC_SHADER_LIGHTS_COUNT));
    if(m_defaultUniforms[SDU_LightsCount]) m_defaultUniforms[SDU_LightsCount]->SetValue(static_cast<int>(l_count));
    if(m_defaultUniforms[SDU_LightData])
    {
        // Forced data fill
        glm::mat4 l_data[ROC_SHADER_LIGHTS_COUNT];
        for(size_t i = 0U; i < l_count; i++)
        {
            Light *l_light = f_data[i];
            const glm::vec2 &l_cutoff = l_light->GetCutoff();

            std::memcpy(&l_data[i][0], &l_light->GetPosition(), sizeof(glm::vec3));
            std::memcpy(&l_data[i][1], &l_light->GetDirection(), sizeof(glm::vec3));
            std::memcpy(&l_data[i][2], &l_light->GetColor(), sizeof(glm::vec4));
            l_data[i][0][3] = l_cutoff.x;
            l_data[i][1][3] = l_cutoff.y;
            std::memcpy(&l_data[i][3], &l_light->GetFalloff(), sizeof(glm::vec3));
            l_data[i][3][3] = static_cast<float>(l_light->GetType());
        }
        glUniformMatrix4fv(m_defaultUniforms[SDU_LightData]->GetUniform(), static_cast<int>(l_count), GL_FALSE, reinterpret_cast<const float*>(&l_data));
    }
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
    if(m_defaultUniforms[SDU_BoneMatrices])
    {
        if(m_defaultUniforms[SDU_BoneMatrices]->IsActive())
        {
            // Forced set of uniform value
            size_t l_size = std::min(f_value.size(), static_cast<size_t>(ROC_SHADER_BONES_COUNT));
            glUniformMatrix4fv(m_defaultUniforms[SDU_BoneMatrices]->GetUniform(), static_cast<int>(l_size), GL_FALSE, reinterpret_cast<const float*>(f_value.data()));
        }
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
    if(!HasAttached(f_drawable))
    {
        ShaderUniform *l_uniform = GetUniform(f_uniform);
        if(l_uniform)
        {
            bool l_used = false;
            for(const auto &l_bindData : m_drawableBind)
            {
                if(l_bindData.m_uniform == l_uniform)
                {
                    l_used = true;
                    break;
                }
            }
            if(!l_used)
            {
                if((l_uniform->IsSampler2D() && !f_drawable->IsCubic()) || (l_uniform->IsSamplerCube() && f_drawable->IsCubic()))
                {
                    size_t l_slot = m_bindPool->Allocate();
                    if(l_slot != Pool::ms_invalid)
                    {
                        DrawableBindData l_bind{ f_drawable, static_cast<int>(l_slot) + 1, l_uniform };
                        m_drawableBind.push_back(l_bind);
                        l_uniform->SetSampler(l_bind.m_slot);

                        Element::AddChild(f_drawable);
                        f_drawable->AddParent(this);

                        l_result = true;
                    }
                }
            }
        }
    }
    return l_result;
}
bool ROC::Shader::Detach(Drawable *f_drawable)
{
    bool l_result = false;
    for(auto l_bindData = m_drawableBind.begin(), l_end = m_drawableBind.end(); l_bindData != l_end; ++l_bindData)
    {
        if(l_bindData->m_element == f_drawable)
        {
            m_bindPool->Reset(static_cast<size_t>(l_bindData->m_slot - 1));
            l_bindData->m_uniform->SetSampler(0);
            m_drawableBind.erase(l_bindData);

            Element::RemoveChild(f_drawable);

            l_result = true;
            break;
        }
    }
    return l_result;
}
bool ROC::Shader::HasAttached(Drawable *f_drawable) const
{
    bool l_result = false;
    for(const auto &l_bindData : m_drawableBind)
    {
        if(l_bindData.m_element == f_drawable)
        {
            l_result = true;
            break;
        }
    }
    return l_result;
}

void ROC::Shader::UpdateDrawableMaxCount()
{
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ms_drawableMaxCount);
    ms_drawableMaxCount--; // Slot 0 is reserved for texture of drawn material
}

void ROC::Shader::Enable()
{
    if(!m_active)
    {
        GLBinder::UseShaderProgram(m_program);
        for(auto l_uniform : m_defaultUniforms)
        {
            if(l_uniform)
            {
                l_uniform->SetActive(true);
                l_uniform->Update();
            }
        }
        for(auto &l_uniformPair : m_uniformMap)
        {
            ShaderUniform *l_uniform = l_uniformPair.second;
            l_uniform->SetActive(true);
            l_uniform->Update();
        }
        if(!m_drawableBind.empty())
        {
            unsigned int l_slot = 0U;
            for(auto &l_bindData : m_drawableBind)
            {
                glActiveTexture(GL_TEXTURE1 + l_slot);
                l_bindData.m_element->Bind();
                l_slot++;
            }
            glActiveTexture(GL_TEXTURE0);
        }

        m_active = true;
    }
}

void ROC::Shader::Disable()
{
    if(m_active)
    {
        for(auto l_uniform : m_defaultUniforms)
        {
            if(l_uniform) l_uniform->SetActive(false);
        }
        for(auto &l_uniformPair : m_uniformMap) l_uniformPair.second->SetActive(false);

        m_active = false;
    }
}

void ROC::Shader::OnParentLinkDestroyed(Element *f_parent)
{
    switch(f_parent->GetElementType())
    {
        case ET_Scene:
            Disable();
            break;
    }

    Element::OnParentLinkDestroyed(f_parent);
}
void ROC::Shader::OnChildLinkDestroyed(Element *f_child)
{
    switch(f_child->GetElementType())
    {
        case ET_Texture: case ET_RenderTarget:
        {
            for(auto l_bindData = m_drawableBind.begin(), l_end = m_drawableBind.end(); l_bindData != l_end; ++l_bindData)
            {
                if(l_bindData->m_element == f_child)
                {
                    m_bindPool->Reset(static_cast<size_t>(l_bindData->m_slot - 1));
                    l_bindData->m_uniform->SetSampler(0);
                    m_drawableBind.erase(l_bindData);
                    break;
                }
            }
        } break;
    }

    Element::OnChildLinkDestroyed(f_child);
}

// Interfaces reroute
bool ROC::Shader::Attach(IDrawable *f_drawable, const std::string &f_uniform)
{
    return Attach(dynamic_cast<Drawable*>(f_drawable), f_uniform);
}
bool ROC::Shader::Detach(IDrawable *f_drawable)
{
    return Detach(dynamic_cast<Drawable*>(f_drawable));
}
bool ROC::Shader::HasAttached(IDrawable *f_drawable) const
{
    return HasAttached(dynamic_cast<Drawable*>(f_drawable));
}
