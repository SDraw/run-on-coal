#include "stdafx.h"

#include "GL/GLTextureCubemap.h"

GLuint GLTextureCubemap::ms_active[] = { 0U };

GLTextureCubemap::GLTextureCubemap()
{
}

GLTextureCubemap::~GLTextureCubemap()
{
}

bool GLTextureCubemap::Create(GLsizei f_width, GLsizei f_height, GLint f_format, GLenum f_dataFormat, const std::vector<const void*> &f_data, GLenum f_filter)
{
    if((m_name == 0U) && (f_data.size() >= 6U))
    {
        glGenTextures(1, &m_name);
        if(m_name != 0U)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, f_filter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, f_filter);

            for(size_t i = 0U; i < 6U; i++) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0, f_format, f_width, f_height, 0, f_dataFormat, GL_UNSIGNED_BYTE, f_data[i]);

            if(ms_active[ms_activeSlot] != 0U) glBindTexture(GL_TEXTURE_CUBE_MAP, ms_active[ms_activeSlot]);
        }
    }
    return (m_name != 0U);
}

bool GLTextureCubemap::Destroy()
{
    if(m_name != 0U)
    {
        for(size_t i = 0U; i < 16U; i++)
        {
            if(ms_active[i] == m_name)
            {
                ms_active[i] = 0U;
                break;
            }
        }

        glDeleteTextures(1, &m_name);
        m_name = 0U;
    }
    return (m_name == 0U);
}

bool GLTextureCubemap::Bind(GLenum f_slot)
{
    if(m_name != 0U)
    {
        if(ms_activeSlot != f_slot)
        {
            glActiveTexture(f_slot);
            ms_activeSlot = f_slot;
        }

        if(ms_active[ms_activeSlot] != m_name)
        {
            ms_active[ms_activeSlot] = m_name;
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
        }
    }

    return (m_name != 0U);
}
