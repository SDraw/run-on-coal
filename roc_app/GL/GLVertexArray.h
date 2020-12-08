#pragma once

class GLVertexArray final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLVertexArray(const GLVertexArray &that) = delete;
    GLVertexArray& operator=(const GLVertexArray &that) = delete;
public:
    GLVertexArray();
    ~GLVertexArray();

    bool Create();
    bool Destroy();

    bool EnableAttribute(GLuint f_index, GLsizei f_size, GLenum f_type);

    bool Bind();
    bool Draw(GLenum f_mode, GLsizei f_count);
};
