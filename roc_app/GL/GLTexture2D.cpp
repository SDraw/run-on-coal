#include "stdafx.h"

#include "GL/GLTexture2D.h"

GLTexture2D::GLTexture2D()
{
    m_internalType = GL_TEXTURE_2D;
}

GLTexture2D::~GLTexture2D()
{
}

bool GLTexture2D::Create(GLsizei f_width, GLsizei f_height, GLint f_format, GLenum f_dataFormat, const void *f_data, GLenum f_filter)
{
    if(m_name == 0U)
    {
        glGenTextures(1, &m_name);
        if(m_name != 0U)
        {
            glBindTexture(GL_TEXTURE_2D, m_name);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f_filter);
            glTexImage2D(GL_TEXTURE_2D, 0, f_format, f_width, f_height, 0, f_dataFormat, GL_UNSIGNED_BYTE, f_data);

            if(ms_activeName[ms_activeSlot] != 0U) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
        }
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetSwizzle(GLenum f_swizzle, const GLint *f_order)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteriv(GL_TEXTURE_2D, f_swizzle, f_order);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetCompareFunc(GLenum f_func)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, f_func);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetCompareMode(GLenum f_mode)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, f_mode);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::SetWrap(GLenum f_wrap)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f_wrap);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }
    return (m_name != 0U);
}

bool GLTexture2D::Update(GLint f_posX, GLint f_posY, GLsizei f_width, GLsizei f_height, GLenum f_dataFormat, const void *f_data)
{
    if(m_name != 0U)
    {
        if(ms_activeName[ms_activeSlot] != m_name) glBindTexture(GL_TEXTURE_2D, m_name);
        glTexSubImage2D(GL_TEXTURE_2D, 0, f_posX, f_posY, f_width, f_height, f_dataFormat, GL_UNSIGNED_BYTE, f_data);
        if((ms_activeName[ms_activeSlot] != m_name) && (ms_activeName[ms_activeSlot] != 0U)) glBindTexture(ms_activeNameType[ms_activeSlot], ms_activeName[ms_activeSlot]);
    }

    return (m_name != 0U);
}
