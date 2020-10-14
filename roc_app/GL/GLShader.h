#pragma once

class GLShader
{
    GLuint m_program;
    GLuint m_vertex;
    GLuint m_fragment;
    GLuint m_geometry;

    std::string m_log;

    __declspec(thread) static GLuint ms_activeProgram;

    GLShader(const GLShader &that) = delete;
    GLShader& operator=(const GLShader &that) = delete;
public:
    GLShader();
    ~GLShader();

    bool Create(const char *f_vertex, size_t f_vertexSize, const char *f_fragment, size_t f_fragmentSize, const char *f_geometry, size_t f_geometrySize);
    bool Destroy();

    const std::string& GetLog() const;
    GLint GetUniformLocation(const char *f_name);
    bool GetUniformInfo(GLint f_index, GLenum &f_type, std::string &f_name);
    GLint GetUniformsCount();

    bool SetAttribute(GLuint f_index, const char *f_name);
    bool SetUniform(GLint f_index, const int *f_data, size_t f_count);
    bool SetUniform(GLint f_index, const unsigned int *f_data, size_t f_count);
    bool SetUniform(GLint f_index, const float *f_data, size_t f_count);
    bool SetUniform(GLint f_index, const double *f_data, size_t f_count);
    bool SetUniform(GLint f_index, size_t f_dim, const float *f_data, size_t f_count);
    bool SetUniform(GLint f_index, size_t f_dim, const double *f_data, size_t f_count);

    bool Bind();
    static void Reset();
};
