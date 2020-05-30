#include "stdafx.h"

#include "GLRenderbuffer.h"

GLuint GLRenderbuffer::ms_activeName = 0U;

GLRenderbuffer::GLRenderbuffer()
{
    m_name = 0U;
}

GLRenderbuffer::~GLRenderbuffer()
{
}

bool GLRenderbuffer::Create(GLenum f_format, GLsizei f_width, GLsizei f_height)
{
    if(m_name == 0U)
    {
        glGenRenderbuffers(1, &m_name);
        if(m_name != 0U)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_name);
            glRenderbufferStorage(GL_RENDERBUFFER, f_format, f_width, f_height);

            glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
        }
    }

    return (m_name != 0U);
}

bool GLRenderbuffer::Destroy()
{
    if(m_name != 0U)
    {
        if(ms_activeName == m_name)
        {
            ms_activeName = 0U;
            glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
        }

        glDeleteRenderbuffers(1, &m_name);
        m_name = 0U;
    }

    return (m_name == 0U);
}

bool GLRenderbuffer::Bind()
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_name);
            ms_activeName = m_name;
        }
    }

    return (m_name != 0U);
}

void GLRenderbuffer::Reset()
{
    if(ms_activeName != 0U)
    {
        ms_activeName = 0U;
        glBindRenderbuffer(GL_RENDERBUFFER, ms_activeName);
    }
}
