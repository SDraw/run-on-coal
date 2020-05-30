#include "stdafx.h"

#include "GL/GLTexture.h"

GLuint GLTexture::ms_activeSlot = 0U;

GLTexture::GLTexture()
{
    m_name = 0U;
}

GLTexture::~GLTexture()
{
}
