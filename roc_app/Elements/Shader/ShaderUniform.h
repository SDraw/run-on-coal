#pragma once

namespace ROC
{

class ShaderUniform final
{
    GLint m_uniformName;
    unsigned char m_type;
    unsigned char *m_data;
    size_t m_dataSize;
    size_t m_count;
    bool m_updated;
protected:
    ShaderUniform(GLint f_uniform, unsigned char f_type, size_t f_dataSize, size_t f_count = 1U);
    ~ShaderUniform();

    void SetData(const void *f_data, size_t f_dataSize);
    const void* GetData() const;

    GLint GetUniformName() const;
    unsigned char GetType() const;
    size_t GetCount() const;

    bool IsUpdated() const;
    void ResetUpdate();

    friend class Shader;
};

}
