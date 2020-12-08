#pragma once

class GLFramebuffer final
{
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeName;

    GLFramebuffer(const GLFramebuffer &that) = delete;
    GLFramebuffer& operator=(const GLFramebuffer &that) = delete;
public:
    GLFramebuffer();
    ~GLFramebuffer();

    bool Create();
    bool Destroy();

    bool SetRenderbuffer(GLenum f_attachment, GLuint f_name);
    bool SetTexture2D(GLenum f_attachment, GLuint f_name);
    bool SetDrawBuffer(GLenum f_buffer);
    bool SetReadBuffer(GLenum f_buffer);

    bool Bind();
    static void Reset();

    static GLenum CheckStatus();
};

