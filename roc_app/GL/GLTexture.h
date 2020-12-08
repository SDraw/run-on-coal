#pragma once

class GLTexture
{
    GLTexture(const GLTexture &that) = delete;
    GLTexture& operator=(const GLTexture &that) = delete;
public:
    virtual ~GLTexture();

    GLuint GetName() const;

    bool Destroy();
    bool Bind(GLenum f_slot = 0U);

    static void Rebind();
protected:
    GLuint m_name;
    GLenum m_internalType;

    __declspec(thread) static GLuint ms_activeSlot;
    __declspec(thread) static GLuint ms_activeName[16U];
    __declspec(thread) static GLenum ms_activeNameType[16U];

    GLTexture();
};
