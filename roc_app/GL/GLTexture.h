#pragma once

class GLTexture
{
    GLTexture(const GLTexture &that) = delete;
    GLTexture& operator=(const GLTexture &that) = delete;
public:
    virtual ~GLTexture();

    inline GLuint GetName() const { return m_name; }

    virtual bool Destroy() = 0;
    virtual bool Bind(GLenum f_slot = 0U) = 0;
protected:
    GLuint m_name;

    __declspec(thread) static GLuint ms_activeSlot;
    __declspec(thread) static GLuint ms_activeName[16U];
    __declspec(thread) static GLenum ms_activeNameType[16U];

    GLTexture();
};
