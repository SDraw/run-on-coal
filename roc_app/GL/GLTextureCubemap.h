#pragma once
#include "GL/GLTexture.h"

class GLTextureCubemap : public GLTexture
{
    GLTextureCubemap(const GLTextureCubemap &that) = delete;
    GLTextureCubemap& operator=(const GLTextureCubemap &that) = delete;
public:
    GLTextureCubemap();
    ~GLTextureCubemap();

    bool Create(GLsizei f_width, GLsizei f_height, GLint f_format, GLenum f_dataFormat, const std::vector<const void*> &f_data, GLenum f_filter = GL_NEAREST);
    bool Destroy();

    bool Bind(GLenum f_slot = 0U);
};
