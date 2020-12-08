#pragma once
#include "GL/GLTexture.h"

class GLTexture2D final : public GLTexture
{
    GLTexture2D(const GLTexture2D &that) = delete;
    GLTexture2D& operator=(const GLTexture2D &that) = delete;
public:
    GLTexture2D();
    ~GLTexture2D();

    bool Create(GLsizei f_width, GLsizei f_height, GLint f_format, GLenum f_dataFormat, const void *f_data, GLenum f_filter = GL_NEAREST);

    bool SetSwizzle(GLenum f_swizzle, const GLint *f_order);
    bool SetCompareFunc(GLenum f_func);
    bool SetCompareMode(GLenum f_mode);
    bool SetWrap(GLenum f_wrap);
    bool Update(GLint f_posX, GLint f_posY, GLsizei f_width, GLsizei f_height, GLenum f_dataFormat, const void *f_data);

};
