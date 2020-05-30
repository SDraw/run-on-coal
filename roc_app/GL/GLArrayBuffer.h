#pragma once
class GLArrayBuffer
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLArrayBuffer(const GLArrayBuffer &that) = delete;
    GLArrayBuffer& operator=(const GLArrayBuffer &that) = delete;
public:
    GLArrayBuffer();
    ~GLArrayBuffer();

    bool Create(size_t f_size, const void *f_data, GLenum f_usage);
    bool Destroy();

    bool Bind();
    bool Update(GLintptr f_offset, GLsizeiptr f_size, const void *f_data);
};

