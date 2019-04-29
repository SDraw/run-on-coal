#pragma once

namespace ROC
{

class IShaderUniform
{
public:
    enum ShaderUniformType
    {
        SUT_Float = 0x1406, // GL_FLOAT
        SUT_Float2 = 0x8B50, // GL_FLOAT_VEC2
        SUT_Float3 = 0x8B51, // GL_FLOAT_VEC3
        SUT_Float4 = 0x8B52, // GL_FLOAT_VEC4
        SUT_Int = 0x1404, // GL_INT
        SUT_Int2 = 0x8B53, // GL_INT_VEC2
        SUT_Int3 = 0x8B54, // GL_INT_VEC3
        SUT_Int4 = 0x8B55, // GL_INT_VEC4
        SUT_Bool = 0x8B56, // GL_BOOL
        SUT_Bool2 = 0x8B57, // GL_BOOL_VEC2,
        SUT_Bool3 = 0x8B58, // GL_BOOL_VEC3,
        SUT_Bool4 = 0x8B59, // GL_BOOL_VEC4,
        SUT_Mat2 = 0x8B5A, // GL_FLOAT_MAT2,
        SUT_Mat3 = 0x8B5B, // GL_FLOAT_MAT3,
        SUT_Mat4 = 0x8B5C, // GL_FLOAT_MAT4,
        SUT_Sampler = 0x8B5E, // GL_SAMPLER_2D,
        SUT_ShadowSampler = 0x8B62, // GL_SAMPLER_2D_SHADOW,
        SUT_CubeSampler = 0x8B60, // GL_SAMPLER_CUBE
    };

    virtual unsigned int GetType() const = 0;
    virtual bool IsSampler2D() const = 0;
    virtual bool IsSamplerCube() const = 0;

    virtual bool SetValue(float f_val) = 0;
    virtual bool SetValue(const glm::vec2 &f_val) = 0;
    virtual bool SetValue(const glm::vec3 &f_val) = 0;
    virtual bool SetValue(const glm::vec4 &f_val) = 0;

    virtual bool SetValue(int f_val) = 0;
    virtual bool SetValue(const glm::ivec2 &f_val) = 0;
    virtual bool SetValue(const glm::ivec3 &f_val) = 0;
    virtual bool SetValue(const glm::ivec4 &f_val) = 0;

    virtual bool SetValue(bool f_val) = 0;
    virtual bool SetValue(const glm::bvec2 &f_val) = 0;
    virtual bool SetValue(const glm::bvec3 &f_val) = 0;
    virtual bool SetValue(const glm::bvec4 &f_val) = 0;

    virtual bool SetValue(const glm::mat2 &f_val) = 0;
    virtual bool SetValue(const glm::mat3 &f_val) = 0;
    virtual bool SetValue(const glm::mat4 &f_val) = 0;

    virtual bool IsActive() const = 0;
};

}
