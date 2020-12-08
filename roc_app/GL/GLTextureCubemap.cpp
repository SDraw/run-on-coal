#include "stdafx.h"

#include "GL/GLTextureCubemap.h"

GLTextureCubemap::GLTextureCubemap()
{
    m_internalType = GL_TEXTURE_CUBE_MAP;
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

            if(ms_activeName[ms_activeSlot] != 0U) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
        }
    }
    return (m_name != 0U);
}
