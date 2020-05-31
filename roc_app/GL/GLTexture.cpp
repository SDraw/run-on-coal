#include "stdafx.h"

#include "GL/GLTexture.h"

GLuint GLTexture::ms_activeSlot = 0U;
GLuint GLTexture::ms_activeName[] = { 0U };
GLenum GLTexture::ms_activeNameType[] = { GL_NONE };

GLTexture::GLTexture()
{
    m_name = 0U;
}

GLTexture::~GLTexture()
{
}
