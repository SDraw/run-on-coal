#include "stdafx.h"

#include "Elements/Shader/ShaderUniform.h"

#include "Utils/GLUtils.hpp"

ROC::ShaderUniform::ShaderUniform(unsigned int f_type, int f_uniform)
{
    m_type = f_type;
    m_uniform = f_uniform;
    m_isActive = false;
    m_needUpdate = false;
}
ROC::ShaderUniform::~ShaderUniform()
{
}

bool ROC::ShaderUniform::SetValue(float f_val)
{
    if(m_type == SUT_Float)
    {
        m_float[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Float);
}
bool ROC::ShaderUniform::SetValue(const glm::vec2 &f_val)
{
    if(m_type == SUT_Float2)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Float2);
}
bool ROC::ShaderUniform::SetValue(const glm::vec3 &f_val)
{
    if(m_type == SUT_Float3)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Float3);
}
bool ROC::ShaderUniform::SetValue(const glm::vec4 &f_val)
{
    if(m_type == SUT_Float4)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Float4);
}

bool ROC::ShaderUniform::SetValue(int f_val)
{
    if(m_type == SUT_Int)
    {
        m_int[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Int);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec2 &f_val)
{
    if(m_type == SUT_Int2)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Int2);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec3 &f_val)
{
    if(m_type == SUT_Int3)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Int3);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec4 &f_val)
{
    if(m_type == SUT_Int4)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec4));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Int4);
}

bool ROC::ShaderUniform::SetValue(bool f_val)
{
    if(m_type == SUT_Bool)
    {
        m_bool[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Bool);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec2 &f_val)
{
    if(m_type == SUT_Bool2)
    {
        for(int i = 0; i < 2; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Bool2);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec3 &f_val)
{
    if(m_type == SUT_Bool3)
    {
        for(int i = 0; i < 3; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Bool3);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec4 &f_val)
{
    if(m_type == SUT_Bool4)
    {
        for(int i = 0; i < 4; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Bool4);
}

bool ROC::ShaderUniform::SetValue(const glm::mat2 &f_val)
{
    if(m_type == SUT_Mat2)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Mat2);
}
bool ROC::ShaderUniform::SetValue(const glm::mat3 &f_val)
{
    if(m_type == SUT_Mat3)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Mat3);
}
bool ROC::ShaderUniform::SetValue(const glm::mat4 &f_val)
{
    if(m_type == SUT_Mat4)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat4));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == SUT_Mat4);
}

bool ROC::ShaderUniform::SetSampler(int f_sampler)
{
    bool l_isSampler = GLUtils::IsSampler(m_type);
    if(l_isSampler)
    {
        m_int[0] = f_sampler;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return l_isSampler;
}

void ROC::ShaderUniform::Update()
{
    if(m_needUpdate)
    {
        switch(m_type)
        {
            case SUT_Float:
                glUniform1f(m_uniform, m_float[0]);
                break;
            case SUT_Float2:
                glUniform2f(m_uniform, m_float[0], m_float[1]);
                break;
            case SUT_Float3:
                glUniform3f(m_uniform, m_float[0], m_float[1], m_float[2]);
                break;
            case SUT_Float4:
                glUniform4f(m_uniform, m_float[0], m_float[1], m_float[2], m_float[3]);
                break;

            case SUT_Int: case SUT_Sampler: case SUT_ShadowSampler: case SUT_CubeSampler:
                glUniform1i(m_uniform, m_int[0]);
                break;
            case SUT_Int2:
                glUniform2i(m_uniform, m_int[0], m_int[1]);
                break;
            case SUT_Int3:
                glUniform3i(m_uniform, m_int[0], m_int[1], m_int[2]);
                break;
            case SUT_Int4:
                glUniform4i(m_uniform, m_int[0], m_int[1], m_int[2], m_int[3]);
                break;

            case SUT_Bool:
                glUniform1ui(m_uniform, m_bool[0]);
                break;
            case SUT_Bool2:
                glUniform2ui(m_uniform, m_bool[0], m_bool[1]);
                break;
            case SUT_Bool3:
                glUniform3ui(m_uniform, m_bool[0], m_bool[1], m_bool[2]);
                break;
            case SUT_Bool4:
                glUniform4ui(m_uniform, m_bool[0], m_bool[1], m_bool[2], m_bool[3]);
                break;

            case SUT_Mat2:
                glUniformMatrix2fv(m_uniform, 1, GL_FALSE, m_float);
                break;
            case SUT_Mat3:
                glUniformMatrix3fv(m_uniform, 1, GL_FALSE, m_float);
                break;
            case SUT_Mat4:
                glUniformMatrix4fv(m_uniform, 1, GL_FALSE, m_float);
                break;
        }
        m_needUpdate = false;
    }
}
