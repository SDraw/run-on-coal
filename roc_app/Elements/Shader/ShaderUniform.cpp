#include "stdafx.h"

#include "Elements/Shader/ShaderUniform.h"

ROC::ShaderUniform::ShaderUniform(GLint f_uniform, unsigned char f_type, size_t f_dataSize, size_t f_count)
{
    m_uniformName = f_uniform;
    m_type = f_type;
    m_data = ((f_dataSize > 0U) ? new unsigned char[f_dataSize] : nullptr);
    m_dataSize = f_dataSize;
    m_count = f_count;
    m_updated = false;
}

ROC::ShaderUniform::~ShaderUniform()
{
    if(m_dataSize > 0U) delete[]m_data;
}

void ROC::ShaderUniform::SetData(const void *f_data, size_t f_dataSize)
{
    if(f_dataSize <= m_dataSize)
    {
        if(std::memcmp(m_data, f_data, f_dataSize) != 0)
        {
            std::memcpy(m_data, f_data, f_dataSize);
            m_updated = true;
        }
    }
}
