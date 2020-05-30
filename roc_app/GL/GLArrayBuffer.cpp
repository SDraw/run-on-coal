#include "stdafx.h"

#include "GLArrayBuffer.h"

GLuint GLArrayBuffer::ms_activeName = 0U;

GLArrayBuffer::GLArrayBuffer()
{
    m_name = 0U;
}

GLArrayBuffer::~GLArrayBuffer()
{
}

bool GLArrayBuffer::Create(size_t f_size, const void *f_data, GLenum f_usage)
{
    if(m_name == 0U)
    {
        glGenBuffers(1, &m_name);
        if(m_name != 0U)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_name);
            glBufferData(GL_ARRAY_BUFFER, f_size, f_data, f_usage);

            if(ms_activeName != 0U) glBindBuffer(GL_ARRAY_BUFFER, ms_activeName);
        }
    }

    return (m_name != 0U);
}

bool GLArrayBuffer::Destroy()
{
    if(m_name != 0U)
    {
        glDeleteBuffers(1, &m_name);

        if(ms_activeName == m_name) ms_activeName = 0U;
        m_name = 0U;
    }

    return (m_name == 0U);
}

bool GLArrayBuffer::Bind()
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_name);
            ms_activeName = m_name;
        }
    }

    return (m_name != 0U);
}

bool GLArrayBuffer::Update(GLintptr f_offset, GLsizeiptr f_size, const void *f_data)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindBuffer(GL_ARRAY_BUFFER, m_name);
        glBufferSubData(GL_ARRAY_BUFFER, f_offset, f_size, f_data);
        if(ms_activeName != m_name) glBindBuffer(GL_ARRAY_BUFFER, ms_activeName);
    }

    return (m_name != 0U);
}
