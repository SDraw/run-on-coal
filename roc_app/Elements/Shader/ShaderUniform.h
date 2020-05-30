#pragma once

namespace ROC
{

class ShaderUniform
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
    inline const void* GetData() const { return m_data; }
    inline bool HasData() const { return (m_data != nullptr); }

    inline GLint GetUniformName() const { return m_uniformName; }
    inline unsigned char GetType() const { return m_type; }
    inline size_t GetCount() const { return m_count; }

    inline bool IsUpdated() const { return m_updated; }
    inline void ResetUpdate() { m_updated = false; }

    friend class Shader;
};

}
