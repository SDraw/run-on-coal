#include "stdafx.h"

#include "GL/GLVertexArray.h"

GLuint GLVertexArray::ms_activeName = 0U;

GLVertexArray::GLVertexArray()
{
    m_name = 0U;
}

GLVertexArray::~GLVertexArray()
{
}

bool GLVertexArray::Create()
{
    if(m_name == 0U) glGenVertexArrays(1, &m_name);

    return (m_name != 0U);
}

bool GLVertexArray::Destroy()
{
    if(m_name != 0U)
    {
        if(ms_activeName == m_name) ms_activeName = 0U;
        glDeleteVertexArrays(1, &m_name);
        m_name = 0U;
    }
    return (m_name == 0U);
}

bool GLVertexArray::EnableAttribute(GLuint f_index, GLsizei f_size, GLenum f_type)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindVertexArray(m_name);

        glEnableVertexAttribArray(f_index);
        switch(f_type)
        {
            case GL_FLOAT:
                glVertexAttribPointer(f_index, f_size, f_type, GL_FALSE, 0, NULL);
                break;
            case GL_INT:
                glVertexAttribIPointer(f_index, f_size, f_type, 0, NULL);
                break;
        }

        if(ms_activeName != m_name) glBindVertexArray(ms_activeName);
    }

    return (m_name != 0U); 
}

bool GLVertexArray::Bind()
{
    if(m_name)
    {
        if(ms_activeName != m_name)
        {
            glBindVertexArray(m_name);
            ms_activeName = m_name;
        }
    }

    return (m_name != 0U);
}

bool GLVertexArray::Draw(GLenum f_mode, GLsizei f_count)
{
    if(m_name != 0U)
    {
        if(ms_activeName != m_name) glBindVertexArray(m_name);
        glDrawArrays(f_mode, 0, f_count);
        if(ms_activeName != m_name) glBindVertexArray(ms_activeName);
    }
    return (m_name != 0U);
}
