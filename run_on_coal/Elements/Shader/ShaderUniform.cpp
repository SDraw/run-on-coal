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
    if(m_type == GL_FLOAT)
    {
        m_float[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT);
}
bool ROC::ShaderUniform::SetValue(const glm::vec2 &f_val)
{
    if(m_type == GL_FLOAT_VEC2)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_VEC2);
}
bool ROC::ShaderUniform::SetValue(const glm::vec3 &f_val)
{
    if(m_type == GL_FLOAT_VEC3)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_VEC3);
}
bool ROC::ShaderUniform::SetValue(const glm::vec4 &f_val)
{
    if(m_type == GL_FLOAT_VEC4)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::vec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_VEC4);
}

bool ROC::ShaderUniform::SetValue(int f_val)
{
    if(m_type == GL_INT)
    {
        m_int[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_INT);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec2 &f_val)
{
    if(m_type == GL_INT_VEC2)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_INT_VEC2);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec3 &f_val)
{
    if(m_type == GL_INT_VEC3)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_INT_VEC3);
}
bool ROC::ShaderUniform::SetValue(const glm::ivec4 &f_val)
{
    if(m_type == GL_INT_VEC4)
    {
        std::memcpy(m_int, &f_val, sizeof(glm::ivec4));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_INT_VEC4);
}

bool ROC::ShaderUniform::SetValue(bool f_val)
{
    if(m_type == GL_BOOL)
    {
        m_bool[0] = f_val;
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_BOOL);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec2 &f_val)
{
    if(m_type == GL_BOOL_VEC2)
    {
        for(int i = 0; i < 2; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_BOOL_VEC2);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec3 &f_val)
{
    if(m_type == GL_BOOL_VEC3)
    {
        for(int i = 0; i < 3; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_BOOL_VEC3);
}
bool ROC::ShaderUniform::SetValue(const glm::bvec4 &f_val)
{
    if(m_type == GL_BOOL_VEC4)
    {
        for(int i = 0; i < 4; i++) m_bool[i] = f_val[i];
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_BOOL_VEC4);
}

bool ROC::ShaderUniform::SetValue(const glm::mat2 &f_val)
{
    if(m_type == GL_FLOAT_MAT2)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat2));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_MAT2);
}
bool ROC::ShaderUniform::SetValue(const glm::mat3 &f_val)
{
    if(m_type == GL_FLOAT_MAT3)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat3));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_MAT3);
}
bool ROC::ShaderUniform::SetValue(const glm::mat4 &f_val)
{
    if(m_type == GL_FLOAT_MAT4)
    {
        std::memcpy(m_float, &f_val, sizeof(glm::mat4));
        m_needUpdate = true;
        if(m_isActive) Update();
    }
    return (m_type == GL_FLOAT_MAT4);
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
            case GL_FLOAT:
                glUniform1f(m_uniform, m_float[0]);
                break;
            case GL_FLOAT_VEC2:
                glUniform2f(m_uniform, m_float[0], m_float[1]);
                break;
            case GL_FLOAT_VEC3:
                glUniform3f(m_uniform, m_float[0], m_float[1], m_float[2]);
                break;
            case GL_FLOAT_VEC4:
                glUniform4f(m_uniform, m_float[0], m_float[1], m_float[2], m_float[3]);
                break;

            case GL_INT: case GL_SAMPLER_2D: case GL_SAMPLER_2D_SHADOW: case GL_SAMPLER_CUBE:
                glUniform1i(m_uniform, m_int[0]);
                break;
            case GL_INT_VEC2:
                glUniform2i(m_uniform, m_int[0], m_int[1]);
                break;
            case GL_INT_VEC3:
                glUniform3i(m_uniform, m_int[0], m_int[1], m_int[2]);
                break;
            case GL_INT_VEC4:
                glUniform4i(m_uniform, m_int[0], m_int[1], m_int[2], m_int[3]);
                break;

            case GL_BOOL:
                glUniform1ui(m_uniform, m_bool[0]);
                break;
            case GL_BOOL_VEC2:
                glUniform2ui(m_uniform, m_bool[0], m_bool[1]);
                break;
            case GL_BOOL_VEC3:
                glUniform3ui(m_uniform, m_bool[0], m_bool[1], m_bool[2]);
                break;
            case GL_BOOL_VEC4:
                glUniform4ui(m_uniform, m_bool[0], m_bool[1], m_bool[2], m_bool[3]);
                break;

            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(m_uniform, 1, GL_FALSE, m_float);
                break;
            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(m_uniform, 1, GL_FALSE, m_float);
                break;
            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(m_uniform, 1, GL_FALSE, m_float);
                break;
        }
        m_needUpdate = false;
    }
}
