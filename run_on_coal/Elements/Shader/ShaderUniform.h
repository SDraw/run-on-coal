#pragma once

namespace ROC
{

class ShaderUniform final
{
    unsigned int m_type;
    int m_uniform;
    union
    {
        float m_float[16];
        int m_int[4];
        unsigned int m_bool[4];
    };
    bool m_isActive;
    bool m_needUpdate;
public:
    inline unsigned int GetType() const { return m_type; }

    bool SetValue(float f_val);
    bool SetValue(const glm::vec2 &f_val);
    bool SetValue(const glm::vec3 &f_val);
    bool SetValue(const glm::vec4 &f_val);

    bool SetValue(int f_val);
    bool SetValue(const glm::ivec2 &f_val);
    bool SetValue(const glm::ivec3 &f_val);
    bool SetValue(const glm::ivec4 &f_val);

    bool SetValue(bool f_val);
    bool SetValue(const glm::bvec2 &f_val);
    bool SetValue(const glm::bvec3 &f_val);
    bool SetValue(const glm::bvec4 &f_val);

    bool SetValue(const glm::mat2 &f_val);
    bool SetValue(const glm::mat3 &f_val);
    bool SetValue(const glm::mat4 &f_val);
protected:
    ShaderUniform(unsigned int f_type, int f_uniform);
    ~ShaderUniform();

    inline void SetActive(bool f_active) { m_isActive = f_active; }

    bool SetSampler(int f_sampler);

    void Update();

    friend class Shader;
};

}
