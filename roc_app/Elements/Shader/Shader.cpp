#include "stdafx.h"

#include "Elements/Shader/Shader.h"
#include "Elements/Shader/ShaderUniform.h"
#include "Elements/Drawable.h"
#include "Elements/Light.h"
#include "GL/GLShader.h"
#include "GL/GLSetting.h"
#include "Utils/Pool.h"

#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_defaultUniforms
{
    "gProjectionMatrix", "gViewMatrix", "gViewProjectionMatrix", "gModelMatrix", "gAnimated", "gBoneMatrix",
    "gCameraPosition", "gCameraDirection",
    "gLightData", "gLightsCount",
    "gMaterialType", "gMaterialParam",
    "gTexture0", "gColor",
    "gTime"
};

const size_t g_shaderMaxBonesCount = 227U;
const size_t g_shaderMaxLightsCount = 4U;

const std::string g_defaultShaderDefines = (std::string() +
    "#version 330 core\n" +
    "#define MAX_BONES " + std::to_string(g_shaderMaxBonesCount) + '\n' +
    "#define MAX_LIGHTS " + std::to_string(g_shaderMaxLightsCount) + '\n' +
    "#define LIGHT_DIRECTIONAL " + std::to_string(ROC::Light::LT_Directional) + '\n' +
    "#define LIGHT_POINT " + std::to_string(ROC::Light::LT_Point) + '\n' +
    "#define LIGHT_SPOTLIGHT " + std::to_string(ROC::Light::LT_Spotlight) + '\n'
    );

}

int ROC::Shader::ms_drawableMaxCount = 0;

ROC::Shader::Shader()
{
    m_elementType = ET_Shader;

    m_shader = nullptr;
    m_active = false;

    for(size_t i = 0U; i < SDU_UniformCount; i++) m_defaultUniforms[i] = nullptr;

    m_uniformMapEnd = m_uniformMap.end();

    m_bindPool = new Pool(static_cast<size_t>(ms_drawableMaxCount));
}

ROC::Shader::~Shader()
{
    if(m_shader)
    {
        m_shader->Destroy();
        delete m_shader;
    }
    for(auto l_uniform : m_defaultUniforms) delete l_uniform;
    for(auto l_pair : m_uniformMap) delete l_pair.second;
    m_uniformMap.clear();
    delete m_bindPool;
    m_drawableBind.clear();
}

bool ROC::Shader::Load(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath)
{
    if(!m_shader)
    {
        std::string l_shaderData[3U];

        std::ifstream l_file;
        l_file.open(f_vpath, std::ios::in);
        if(!l_file.fail())
        {
            l_shaderData[0U].assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            l_shaderData[0U].insert(l_shaderData[0U].begin(), g_defaultShaderDefines.begin(), g_defaultShaderDefines.end());
        }
        else m_error.assign("Unable to load vertex shader");
        l_file.clear();

        l_file.open(f_fpath, std::ios::in);
        if(!l_file.fail())
        {
            l_shaderData[1U].assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
            l_file.close();

            l_shaderData[1U].insert(l_shaderData[1U].begin(), g_defaultShaderDefines.begin(), g_defaultShaderDefines.end());
        }
        else m_error.assign("Unable to load fragment shader");
        l_file.clear();

        if(!f_gpath.empty())
        {
            l_file.open(f_gpath, std::ios::in);
            if(!l_file.fail())
            {
                l_shaderData[2U].assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());
                l_file.close();

                l_shaderData[2U].insert(l_shaderData[2U].begin(), g_defaultShaderDefines.begin(), g_defaultShaderDefines.end());
            }
            l_file.clear();
        }

        if(m_error.empty())
        {
            m_shader = new GLShader();
            if(!m_shader->Create(l_shaderData[0U].data(), l_shaderData[0U].size(), l_shaderData[1U].data(), l_shaderData[1U].size(), l_shaderData[2U].data(), l_shaderData[2U].size()))
            {
                m_error.assign(m_shader->GetLog());
                m_shader->Destroy();
                delete m_shader;
                m_shader = nullptr;
            }
            else SetupUniformsAndLocations();
        }
    }
    return (m_shader != nullptr);
}

void ROC::Shader::SetupUniformsAndLocations()
{
    m_shader->SetAttribute(0U, "gVertexPosition");
    m_shader->SetAttribute(1U, "gVertexNormal");
    m_shader->SetAttribute(2U, "gVertexUV");
    m_shader->SetAttribute(3U, "gVertexWeight");
    m_shader->SetAttribute(4U, "gVertexIndex");

    // Matrices
    FindDefaultUniform(SDU_Projection, SUT_FloatMat4, "gProjectionMatrix", sizeof(glm::mat4));
    FindDefaultUniform(SDU_View, SUT_FloatMat4, "gViewMatrix", sizeof(glm::mat4));
    FindDefaultUniform(SDU_ViewProjection, SUT_FloatMat4, "gViewProjectionMatrix", sizeof(glm::mat4));
    FindDefaultUniform(SDU_Model, SUT_FloatMat4, "gModelMatrix", sizeof(glm::mat4));
    FindDefaultUniform(SDU_BoneMatrices, SUT_FloatMat4, "gBoneMatrix", 0U); // Only direct update

    // Camera
    FindDefaultUniform(SDU_CameraPosition, SUT_Float3, "gCameraPosition", sizeof(glm::vec3));
    FindDefaultUniform(SDU_CameraDirection, SUT_Float3, "gCameraDirection", sizeof(glm::vec3));
    // Light
    FindDefaultUniform(SDU_LightData, SUT_FloatMat4, "gLightData", sizeof(glm::mat4) * g_shaderMaxLightsCount, g_shaderMaxLightsCount); // Array
    FindDefaultUniform(SDU_LightsCount, SUT_Int, "gLightsCount", sizeof(int));
    // Material
    FindDefaultUniform(SDU_MaterialParam, SUT_Float4, "gMaterialParam", sizeof(glm::vec4));
    FindDefaultUniform(SDU_MaterialType, SUT_Int, "gMaterialType", sizeof(int));
    // Animation
    FindDefaultUniform(SDU_Animated, SUT_UInt, "gAnimated", sizeof(unsigned int));
    // Samplers
    FindDefaultUniform(SDU_DiffuseTexture, SUT_Int, "gTexture0", sizeof(int));
    // Time
    FindDefaultUniform(SDU_Time, SUT_Float, "gTime", sizeof(float));
    // Color (RenderTarget, Texture, Font)
    FindDefaultUniform(SDU_Color, SUT_Float4, "gColor", sizeof(glm::vec4));

    // Get list of custom uniforms
    for(int i = 0, j = m_shader->GetUniformsCount(); i < j; i++)
    {
        std::string l_uniformName;
        GLenum l_uniformType;
        m_shader->GetUniformInfo(i, l_uniformType, l_uniformName);

        const size_t l_dataSize = GetSizeFromGLType(l_uniformType);
        if(l_dataSize > 0U)
        {
            if(EnumUtils::ReadEnumVector(l_uniformName, g_defaultUniforms) == std::numeric_limits<size_t>::max())
            {
                const unsigned char l_internalType = Shader::GetTypeFromGLType(l_uniformType);
                GLint l_uniformLocation = m_shader->GetUniformLocation(l_uniformName.c_str());
                m_uniformMap.insert(std::make_pair(l_uniformName, new ShaderUniform(l_uniformLocation, l_internalType, l_dataSize)));
            }
        }
    }
    m_uniformMapEnd = m_uniformMap.end();

    // Initialize few uniforms
    if(m_defaultUniforms[SDU_DiffuseTexture])
    {
        const int l_index = 0;
        m_defaultUniforms[SDU_DiffuseTexture]->SetData(&l_index, sizeof(int));
    }
}

void ROC::Shader::FindDefaultUniform(ShaderDefaultUniform f_sud, unsigned char f_type, const char *f_name, size_t f_dataSize, size_t f_count)
{
    int l_uniform = m_shader->GetUniformLocation(f_name);
    if(l_uniform != -1) m_defaultUniforms[f_sud] = new ShaderUniform(l_uniform, f_type, f_dataSize, f_count);
}

const std::string& ROC::Shader::GetError() const
{
    return m_error;
}

ROC::ShaderUniform* ROC::Shader::GetUniform(const std::string &f_uniform)
{
    ShaderUniform *l_uniform = nullptr;
    auto l_uniformPair = m_uniformMap.find(f_uniform);
    if(l_uniformPair != m_uniformMapEnd) l_uniform = l_uniformPair->second;
    return l_uniform;
}

void ROC::Shader::UpdateShaderFromUniform(ShaderUniform *l_uniform)
{
    switch(l_uniform->GetType())
    {
        case SUT_Float:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const float*>(l_uniform->GetData()), 1U);
            break;
        case SUT_Float2:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const float*>(l_uniform->GetData()), 2U);
            break;
        case SUT_Float3:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const float*>(l_uniform->GetData()), 3U);
            break;
        case SUT_Float4:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const float*>(l_uniform->GetData()), 4U);
            break;

        case SUT_Int:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const int*>(l_uniform->GetData()), 1U);
            break;
        case SUT_Int2:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const int*>(l_uniform->GetData()), 2U);
            break;
        case SUT_Int3:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const int*>(l_uniform->GetData()), 3U);
            break;
        case SUT_Int4:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const int*>(l_uniform->GetData()), 4U);
            break;

        case SUT_UInt:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const unsigned int*>(l_uniform->GetData()), 1U);
            break;
        case SUT_UInt2:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const unsigned int*>(l_uniform->GetData()), 2U);
            break;
        case SUT_UInt3:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const unsigned int*>(l_uniform->GetData()), 3U);
            break;
        case SUT_UInt4:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const unsigned int*>(l_uniform->GetData()), 4U);
            break;

        case SUT_Double:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const double*>(l_uniform->GetData()), 1U);
            break;
        case SUT_Double2:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const double*>(l_uniform->GetData()), 2U);
            break;
        case SUT_Double3:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const double*>(l_uniform->GetData()), 3U);
            break;
        case SUT_Double4:
            m_shader->SetUniform(l_uniform->GetUniformName(), reinterpret_cast<const double*>(l_uniform->GetData()), 4U);
            break;

        case SUT_FloatMat2:
            m_shader->SetUniform(l_uniform->GetUniformName(), 2U, reinterpret_cast<const float*>(l_uniform->GetData()), l_uniform->GetCount());
            break;
        case SUT_FloatMat3:
            m_shader->SetUniform(l_uniform->GetUniformName(), 3U, reinterpret_cast<const float*>(l_uniform->GetData()), l_uniform->GetCount());
            break;
        case SUT_FloatMat4:
            m_shader->SetUniform(l_uniform->GetUniformName(), 4U, reinterpret_cast<const float*>(l_uniform->GetData()), l_uniform->GetCount());
            break;

        case SUT_DoubleMat2:
            m_shader->SetUniform(l_uniform->GetUniformName(), 2U, reinterpret_cast<const double*>(l_uniform->GetData()), l_uniform->GetCount());
            break;
        case SUT_DoubleMat3:
            m_shader->SetUniform(l_uniform->GetUniformName(), 3U, reinterpret_cast<const double*>(l_uniform->GetData()), l_uniform->GetCount());
            break;
        case SUT_DoubleMat4:
            m_shader->SetUniform(l_uniform->GetUniformName(), 4U, reinterpret_cast<const double*>(l_uniform->GetData()), l_uniform->GetCount());
            break;
    }
}

// Set for default uniforms
void ROC::Shader::SetProjectionMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_Projection])
    {
        m_defaultUniforms[SDU_Projection]->SetData(&f_value, sizeof(glm::mat4));
        if(m_active && m_defaultUniforms[SDU_Projection]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_Projection]->GetUniformName(), 4U, glm::value_ptr(f_value), 1U);
            m_defaultUniforms[SDU_Projection]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetViewMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_View])
    {
        m_defaultUniforms[SDU_View]->SetData(&f_value, sizeof(glm::mat4));
        if(m_active && m_defaultUniforms[SDU_View]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_View]->GetUniformName(), 4U, glm::value_ptr(f_value), 1U);
            m_defaultUniforms[SDU_View]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetViewProjectionMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_ViewProjection])
    {
        m_defaultUniforms[SDU_ViewProjection]->SetData(&f_value, sizeof(glm::mat4));
        if(m_active && m_defaultUniforms[SDU_ViewProjection]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_ViewProjection]->GetUniformName(), 4U, glm::value_ptr(f_value), 1U);
            m_defaultUniforms[SDU_ViewProjection]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetModelMatrix(const glm::mat4 &f_value)
{
    if(m_defaultUniforms[SDU_Model])
    {
        m_defaultUniforms[SDU_Model]->SetData(&f_value, sizeof(glm::mat4));
        if(m_active && m_defaultUniforms[SDU_Model]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_Model]->GetUniformName(), 4U, glm::value_ptr(f_value), 1U);
            m_defaultUniforms[SDU_Model]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetCameraPosition(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_CameraPosition])
    {
        m_defaultUniforms[SDU_CameraPosition]->SetData(&f_value, sizeof(glm::vec3));
        if(m_active && m_defaultUniforms[SDU_CameraPosition]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_CameraPosition]->GetUniformName(), glm::value_ptr(f_value), 3U);
            m_defaultUniforms[SDU_CameraPosition]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetCameraDirection(const glm::vec3 &f_value)
{
    if(m_defaultUniforms[SDU_CameraDirection])
    {
        m_defaultUniforms[SDU_CameraDirection]->SetData(&f_value, sizeof(glm::vec3));
        if(m_active && m_defaultUniforms[SDU_CameraDirection]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_CameraDirection]->GetUniformName(), glm::value_ptr(f_value), 3U);
            m_defaultUniforms[SDU_CameraDirection]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetLightsData(const std::vector<Light*> &f_data)
{
    const int l_count = static_cast<int>(std::min(f_data.size(), g_shaderMaxLightsCount));
    if(m_defaultUniforms[SDU_LightsCount])
    {
        m_defaultUniforms[SDU_LightsCount]->SetData(&l_count, sizeof(int));
        if(m_active && m_defaultUniforms[SDU_LightsCount]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_LightsCount]->GetUniformName(), &l_count, 1U);
            m_defaultUniforms[SDU_LightsCount]->ResetUpdate();
        }
    }
    if(m_defaultUniforms[SDU_LightData])
    {
        glm::mat4 l_data[g_shaderMaxLightsCount];
        for(size_t i = 0U, j = std::min(f_data.size(), g_shaderMaxLightsCount); i < j; i++)
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

        m_defaultUniforms[SDU_LightData]->SetData(&l_data, sizeof(glm::mat4)*g_shaderMaxLightsCount);
        if(m_active && m_defaultUniforms[SDU_LightData]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_LightData]->GetUniformName(), 4U, glm::value_ptr(l_data[0]), l_count);
            m_defaultUniforms[SDU_LightData]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetMaterialParam(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_MaterialParam])
    {
        m_defaultUniforms[SDU_MaterialParam]->SetData(&f_value, sizeof(glm::vec4));
        if(m_active && m_defaultUniforms[SDU_MaterialParam]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_MaterialParam]->GetUniformName(), glm::value_ptr(f_value), 4U);
            m_defaultUniforms[SDU_MaterialParam]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetMaterialType(int f_value)
{
    if(m_defaultUniforms[SDU_MaterialType])
    {
        m_defaultUniforms[SDU_MaterialType]->SetData(&f_value, sizeof(int));
        if(m_active && m_defaultUniforms[SDU_MaterialType]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_MaterialType]->GetUniformName(), &f_value, 1U);
            m_defaultUniforms[SDU_MaterialType]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetAnimated(bool f_value)
{
    if(m_defaultUniforms[SDU_Animated])
    {
        const unsigned int l_value = (f_value ? 1U : 0U);
        m_defaultUniforms[SDU_Animated]->SetData(&l_value, sizeof(unsigned int));
        if(m_active && m_defaultUniforms[SDU_Animated]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_Animated]->GetUniformName(), &l_value, 1U);
            m_defaultUniforms[SDU_Animated]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetBoneMatrices(const std::vector<glm::mat4> &f_value)
{
    if(m_defaultUniforms[SDU_BoneMatrices] && m_active)
    {
        size_t l_size = std::min(f_value.size(), g_shaderMaxBonesCount);
        m_shader->SetUniform(m_defaultUniforms[SDU_BoneMatrices]->GetUniformName(), 4U, glm::value_ptr(f_value[0U]), l_size);
    }
}

void ROC::Shader::SetTime(float f_value)
{
    if(m_defaultUniforms[SDU_Time])
    {
        m_defaultUniforms[SDU_Time]->SetData(&f_value, sizeof(float));
        if(m_active && m_defaultUniforms[SDU_Time]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_Time]->GetUniformName(), &f_value, 1U);
            m_defaultUniforms[SDU_Time]->ResetUpdate();
        }
    }
}

void ROC::Shader::SetColor(const glm::vec4 &f_value)
{
    if(m_defaultUniforms[SDU_Color])
    {
        m_defaultUniforms[SDU_Color]->SetData(&f_value, sizeof(glm::vec4));
        if(m_active && m_defaultUniforms[SDU_Color]->IsUpdated())
        {
            m_shader->SetUniform(m_defaultUniforms[SDU_Color]->GetUniformName(), glm::value_ptr(f_value), 4U);
            m_defaultUniforms[SDU_Color]->ResetUpdate();
        }
    }
}

// Non-default uniforms update
bool ROC::Shader::SetUniformValue(const std::string &f_name, ShaderUniformType f_type, const void *f_data, size_t f_size)
{
    bool l_result = false;
    ShaderUniform *l_uniform = GetUniform(f_name);
    if(l_uniform)
    {
        if(l_uniform->GetType() == f_type)
        {
            l_uniform->SetData(f_data, f_size);
            if(m_active && l_uniform->IsUpdated())
            {
                UpdateShaderFromUniform(l_uniform);
                l_uniform->ResetUpdate();
            }
            l_result = true;
        }
    }
    return l_result;
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
                if(l_bindData.m_uniform == l_uniform->GetUniformName())
                {
                    l_used = true;
                    break;
                }
            }
            if(!l_used)
            {
                size_t l_slot = m_bindPool->Allocate();
                if(l_slot != Pool::ms_invalid)
                {
                    DrawableBindData l_bind{ f_drawable, static_cast<int>(l_slot)+1, l_uniform->GetUniformName() };
                    m_drawableBind.push_back(l_bind);
                    l_uniform->SetData(&l_bind.m_slot, sizeof(int));

                    if(m_active)
                    {
                        m_shader->SetUniform(l_uniform->GetUniformName(), &l_bind.m_slot, 1U);
                        l_uniform->ResetUpdate();
                    }

                    Element::AddChild(f_drawable);
                    f_drawable->AddParent(this);

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
    for(auto l_bindData = m_drawableBind.begin(), l_end = m_drawableBind.end(); l_bindData != l_end; ++l_bindData)
    {
        if(l_bindData->m_element == f_drawable)
        {
            m_bindPool->Reset(static_cast<size_t>(l_bindData->m_slot - 1));
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

void ROC::Shader::Enable()
{
    if(!m_active && m_shader)
    {
        m_shader->Bind();
        for(auto l_uniform : m_defaultUniforms)
        {
            if(l_uniform)
            {
                if(l_uniform->IsUpdated() && l_uniform->GetData())
                {
                    UpdateShaderFromUniform(l_uniform);
                    l_uniform->ResetUpdate();
                }
            }
        }
        for(auto &l_uniformPair : m_uniformMap)
        {
            ShaderUniform *l_uniform = l_uniformPair.second;
            if(l_uniform->IsUpdated() && l_uniform->GetData())
            {
                UpdateShaderFromUniform(l_uniform);
                l_uniform->ResetUpdate();
            }
        }
        if(!m_drawableBind.empty())
        {
            unsigned int l_slot = 1U;
            for(auto &l_bindData : m_drawableBind)
            {
                l_bindData.m_element->Bind(l_slot);
                l_slot++;
            }
        }

        m_active = true;
    }
}

void ROC::Shader::Disable()
{
    if(m_active && m_shader) m_active = false;
}

bool ROC::Shader::IsActive() const
{
    return m_active;
}

void ROC::Shader::InitStaticResources()
{
    GLSetting::GetInteger(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ms_drawableMaxCount);
    ms_drawableMaxCount--; // Slot 0 is reserved for texture of drawn material
}

size_t ROC::Shader::GetSizeFromGLType(GLenum f_type)
{
    size_t l_result = 0U;
    switch(f_type)
    {
        case GL_FLOAT: case GL_INT: case GL_UNSIGNED_INT: case GL_BOOL:
            l_result = sizeof(float);
            break;

        case GL_FLOAT_VEC2: case GL_INT_VEC2: case GL_UNSIGNED_INT_VEC2: case GL_BOOL_VEC2:
            l_result = sizeof(float) * 2U;
            break;

        case GL_FLOAT_VEC3: case GL_INT_VEC3: case GL_UNSIGNED_INT_VEC3: case GL_BOOL_VEC3:
            l_result = sizeof(float) * 3U;
            break;

        case GL_FLOAT_VEC4: case GL_INT_VEC4: case GL_UNSIGNED_INT_VEC4: case GL_BOOL_VEC4:
            l_result = sizeof(float) * 4U;
            break;

        case GL_SAMPLER_1D: case GL_SAMPLER_2D: case GL_SAMPLER_3D: case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW: case GL_SAMPLER_2D_SHADOW: case GL_SAMPLER_2D_MULTISAMPLE:
            l_result = sizeof(int);
            break;

        case GL_FLOAT_MAT2:
            l_result = sizeof(float) * 4U;
            break;
        case GL_FLOAT_MAT3:
            l_result = sizeof(float) * 9U;
            break;
        case GL_FLOAT_MAT4:
            l_result = sizeof(float) * 16U;
            break;

        case GL_DOUBLE_MAT2:
            l_result = sizeof(double) * 4U;
            break;
        case GL_DOUBLE_MAT3:
            l_result = sizeof(double) * 9U;
            break;
        case GL_DOUBLE_MAT4:
            l_result = sizeof(double) * 16U;
            break;
    }
    return l_result;
}

unsigned char ROC::Shader::GetTypeFromGLType(GLenum f_type)
{
    unsigned char l_result = 255U;
    switch(f_type)
    {
        case GL_FLOAT:
            l_result = SUT_Float;
            break;
        case GL_FLOAT_VEC2:
            l_result = SUT_Float2;
            break;
        case GL_FLOAT_VEC3:
            l_result = SUT_Float3;
            break;
        case GL_FLOAT_VEC4:
            l_result = SUT_Float4;
            break;

        case GL_INT: case GL_SAMPLER_1D: case GL_SAMPLER_2D: case GL_SAMPLER_3D: case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW: case GL_SAMPLER_2D_SHADOW: case GL_SAMPLER_2D_MULTISAMPLE:
            l_result = SUT_Int;
            break;
        case GL_INT_VEC2:
            l_result = SUT_Int2;
            break;
        case GL_INT_VEC3:
            l_result = SUT_Int3;
            break;
        case GL_INT_VEC4:
            l_result = SUT_Int4;
            break;

        case GL_UNSIGNED_INT:
            l_result = SUT_UInt;
            break;
        case GL_UNSIGNED_INT_VEC2:
            l_result = SUT_UInt2;
            break;
        case GL_UNSIGNED_INT_VEC3:
            l_result = SUT_UInt3;
            break;
        case GL_UNSIGNED_INT_VEC4:
            l_result = SUT_UInt4;
            break;

        case GL_DOUBLE:
            l_result = SUT_Double;
            break;
        case GL_DOUBLE_VEC2:
            l_result = SUT_Double2;
            break;
        case GL_DOUBLE_VEC3:
            l_result = SUT_Double3;
            break;
        case GL_DOUBLE_VEC4:
            l_result = SUT_Double4;
            break;

        case GL_FLOAT_MAT2:
            l_result = SUT_FloatMat2;
            break;
        case GL_FLOAT_MAT3:
            l_result = SUT_FloatMat3;
            break;
        case GL_FLOAT_MAT4:
            l_result = SUT_FloatMat4;
            break;

        case GL_DOUBLE_MAT2:
            l_result = SUT_DoubleMat2;
            break;
        case GL_DOUBLE_MAT3:
            l_result = SUT_DoubleMat3;
            break;
        case GL_DOUBLE_MAT4:
            l_result = SUT_DoubleMat4;
            break;
    }
    return l_result;
}


// ROC::Element
void ROC::Shader::OnParentRemoved(Element *f_parent)
{
    switch(f_parent->GetElementType())
    {
        case ET_Scene:
            Disable();
            break;
    }

    Element::OnParentRemoved(f_parent);
}
void ROC::Shader::OnChildRemoved(Element *f_child)
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
                    //l_bindData->m_uniform->SetSampler(0);
                    m_drawableBind.erase(l_bindData);
                    break;
                }
            }
        } break;
    }

    Element::OnChildRemoved(f_child);
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
