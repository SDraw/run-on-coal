#include "stdafx.h"

#include "GLShader.h"

GLuint GLShader::ms_activeProgram = 0U;

GLShader::GLShader()
{
    m_program = 0U;
    m_vertex = 0U;
    m_fragment = 0U;
    m_geometry = 0U;
}

GLShader::~GLShader()
{
}

bool GLShader::Create(const char *f_vertex, size_t f_vertexSize, const char *f_fragment, size_t f_fragmentSize, const char *f_geometry, size_t f_geometrySize)
{
    if(m_program == 0U)
    {
        if(f_vertex && (f_vertexSize > 0U))
        {
            m_vertex = glCreateShader(GL_VERTEX_SHADER);
            if(m_vertex != 0U)
            {
                int l_sourceLength = static_cast<int>(f_vertexSize);
                glShaderSource(m_vertex, 1, &f_vertex, &l_sourceLength);
                glCompileShader(m_vertex);

                GLint l_state;
                glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_vertex, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_vertex, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Vertex shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_vertex);
                    m_vertex = 0U;
                }
            }
        }

        if(f_fragment && (f_fragmentSize > 0U))
        {
            m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
            if(m_fragment != 0U)
            {
                int l_sourceLength = static_cast<int>(f_fragmentSize);
                glShaderSource(m_fragment, 1, &f_fragment, &l_sourceLength);
                glCompileShader(m_fragment);

                GLint l_state;
                glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_fragment, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_fragment, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Fragment shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_fragment);
                    m_fragment = 0U;
                }

            }
        }

        if(f_geometry && (f_geometrySize > 0U))
        {
            m_geometry = glCreateShader(GL_GEOMETRY_SHADER);
            if(m_geometry != 0U)
            {
                int l_sourceLength = static_cast<int>(f_geometrySize);
                glShaderSource(m_geometry, 1, &f_geometry, &l_sourceLength);
                glCompileShader(m_geometry);

                GLint l_state;
                glGetShaderiv(m_geometry, GL_COMPILE_STATUS, &l_state);
                if(!l_state)
                {
                    GLint l_logSize = 0;
                    std::string l_log;
                    glGetShaderiv(m_geometry, GL_INFO_LOG_LENGTH, &l_logSize);
                    l_log.resize(static_cast<size_t>(l_logSize));
                    glGetShaderInfoLog(m_geometry, l_logSize, &l_logSize, &l_log[0]);
                    l_log.insert(0U, "Fragment shader error: ");
                    m_log.append(l_log);

                    glDeleteShader(m_geometry);
                    m_geometry = 0U;
                }
            }
        }

        if(m_vertex != 0U || m_fragment != 0U || m_geometry != 0U)
        {
            m_program = glCreateProgram();
            if(m_program != 0U)
            {
                if(m_vertex != 0U) glAttachShader(m_program, m_vertex);
                if(m_fragment != 0U) glAttachShader(m_program, m_fragment);
                if(m_geometry != 0U) glAttachShader(m_program, m_geometry);

                GLint l_link = 0;
                glLinkProgram(m_program);
                glGetProgramiv(m_program, GL_LINK_STATUS, &l_link);
                if(!l_link)
                {
                    GLint l_logLength = 0;
                    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &l_logLength);
                    m_log.resize(static_cast<size_t>(l_logLength));
                    glGetProgramInfoLog(m_program, l_logLength, &l_logLength, &m_log[0]);
                    glDeleteProgram(m_program);
                    m_program = 0U;
                }
                else
                {
                    if(m_vertex != 0U) glDetachShader(m_program, m_vertex);
                    if(m_fragment != 0U) glDetachShader(m_program, m_fragment);
                    if(m_geometry != 0U) glDetachShader(m_program, m_geometry);
                }
            }

            if(m_vertex != 0U)
            {
                glDeleteShader(m_vertex);
                m_vertex = 0U;
            }
            if(m_fragment != 0U)
            {
                glDeleteShader(m_fragment);
                m_fragment = 0U;
            }
            if(m_geometry != 0U)
            {
                glDeleteShader(m_geometry);
                m_geometry = 0U;
            }

        }
    }

    return (m_program != 0U);
}

bool GLShader::Destroy()
{
    if(m_program != 0U)
    {
        if(ms_activeProgram == m_program) ms_activeProgram = 0U;

        glDeleteProgram(m_program);
        m_program = 0U;
    }

    return (m_program == 0U);
}

GLint GLShader::GetUniformLocation(const char *f_name)
{
    int l_result = -1;
    if(m_program != 0U) l_result = glGetUniformLocation(m_program, f_name);
    return l_result;
}

bool GLShader::GetUniformInfo(GLint f_index, GLenum &f_type, std::string &f_name)
{
    if(m_program != 0U)
    {
        std::vector<GLchar> l_uniformName(256U, '\0');
        GLsizei l_uniformNameSize, l_uniformSize;
        glGetActiveUniform(m_program, static_cast<GLuint>(f_index), 256, &l_uniformNameSize, &l_uniformSize, &f_type, l_uniformName.data());
        f_name.assign(l_uniformName.data(), l_uniformNameSize);
    }
    return (m_program != 0U);
}

GLint GLShader::GetUniformsCount()
{
    int l_result = 0;
    if(m_program != 0U) glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &l_result);
    return l_result;
}

bool GLShader::SetAttribute(GLuint f_index, const char *f_name)
{
    if(m_program != 0U) glBindAttribLocation(m_program, f_index, f_name);
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, const int *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_count)
        {
            case 1U:
                glUniform1i(f_index, f_data[0U]);
                break;
            case 2U:
                glUniform2i(f_index, f_data[0U], f_data[1U]);
                break;
            case 3U:
                glUniform3i(f_index, f_data[0U], f_data[1U], f_data[2U]);
                break;
            case 4U:
                glUniform4i(f_index, f_data[0U], f_data[1U], f_data[2U], f_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, const unsigned int *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_count)
        {
            case 1U:
                glUniform1ui(f_index, f_data[0U]);
                break;
            case 2U:
                glUniform2ui(f_index, f_data[0U], f_data[1U]);
                break;
            case 3U:
                glUniform3ui(f_index, f_data[0U], f_data[1U], f_data[2U]);
                break;
            case 4U:
                glUniform4ui(f_index, f_data[0U], f_data[1U], f_data[2U], f_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, const float *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_count)
        {
            case 1U:
                glUniform1f(f_index, f_data[0U]);
                break;
            case 2U:
                glUniform2f(f_index, f_data[0U], f_data[1U]);
                break;
            case 3U:
                glUniform3f(f_index, f_data[0U], f_data[1U], f_data[2U]);
                break;
            case 4U:
                glUniform4f(f_index, f_data[0U], f_data[1U], f_data[2U], f_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, const double *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_count)
        {
            case 1U:
                glUniform1d(f_index, f_data[0U]);
                break;
            case 2U:
                glUniform2d(f_index, f_data[0U], f_data[1U]);
                break;
            case 3U:
                glUniform3d(f_index, f_data[0U], f_data[1U], f_data[2U]);
                break;
            case 4U:
                glUniform4d(f_index, f_data[0U], f_data[1U], f_data[2U], f_data[3U]);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, size_t f_dim, const float *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_dim)
        {
            case 2U:
                glUniformMatrix2fv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
            case 3U:
                glUniformMatrix3fv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
            case 4U:
                glUniformMatrix4fv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::SetUniform(GLint f_index, size_t f_dim, const double *f_data, size_t f_count)
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program) glUseProgram(m_program);
        switch(f_dim)
        {
            case 2U:
                glUniformMatrix2dv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
            case 3U:
                glUniformMatrix3dv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
            case 4U:
                glUniformMatrix4dv(f_index, static_cast<GLsizei>(f_count), GL_FALSE, f_data);
                break;
        }
        if((ms_activeProgram != m_program) && (ms_activeProgram != 0U)) glUseProgram(ms_activeProgram);
    }
    return (m_program != 0U);
}

bool GLShader::Bind()
{
    if(m_program != 0U)
    {
        if(ms_activeProgram != m_program)
        {
            glUseProgram(m_program);
            ms_activeProgram = m_program;
        }
    }
    return (m_program != 0U);
}
