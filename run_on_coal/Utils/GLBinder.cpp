#include "stdafx.h"
#include "Utils/GLBinder.h"

GLuint ROC::GLBinder::ms_arrayBuffer = 0U;
GLuint ROC::GLBinder::ms_vertexArray = 0U;
GLuint ROC::GLBinder::ms_texture = 0U;
GLuint ROC::GLBinder::ms_frameBuffer = 0U;
int ROC::GLBinder::ms_viewportX = 0;
int ROC::GLBinder::ms_viewportY = 0;
int ROC::GLBinder::ms_viewportW = 0;
int ROC::GLBinder::ms_viewportH = 0;

void ROC::GLBinder::BindArrayBuffer(GLuint f_buffer)
{
    if(ms_arrayBuffer != f_buffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, f_buffer);
        ms_arrayBuffer = f_buffer;
    }
}
void ROC::GLBinder::BindVertexArray(GLuint f_array)
{
    if(ms_vertexArray != f_array)
    {
        glBindVertexArray(f_array);
        ms_vertexArray = f_array;
    }
}
void ROC::GLBinder::BindTexture2D(GLuint f_texture)
{
    if(ms_texture != f_texture)
    {
        glBindTexture(GL_TEXTURE_2D, f_texture);
        ms_texture = f_texture;
    }
}
void ROC::GLBinder::BindTextureCube(GLuint f_texture)
{
    if(ms_texture != f_texture)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, f_texture);
        ms_texture = f_texture;
    }
}
void ROC::GLBinder::BindFramebuffer(GLuint f_buffer)
{
    if(ms_frameBuffer != f_buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, f_buffer);
        ms_frameBuffer = f_buffer;
    }
}

void ROC::GLBinder::SetViewport(int f_px, int f_py, int f_width, int f_height)
{
    glm::ivec4 l_size(f_px, f_py, f_width, f_height);
    glm::ivec4 l_current(ms_viewportX, ms_viewportY, ms_viewportW, ms_viewportW);
    if(l_current != l_size)
    {
        ms_viewportX = l_size.x;
        ms_viewportY = l_size.y;
        ms_viewportW = l_size.z;
        ms_viewportH = l_size.w;
        glViewport(ms_viewportX, ms_viewportY, ms_viewportW, ms_viewportH);
    }
}
