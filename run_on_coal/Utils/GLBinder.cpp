#include "stdafx.h"
#include "Utils/GLBinder.h"

GLuint ROC::GLBinder::ms_arrayBuffer = 0U;
GLuint ROC::GLBinder::ms_vertexArray = 0U;
GLuint ROC::GLBinder::ms_texture = 0U;
GLuint ROC::GLBinder::ms_frameBuffer = 0U;

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
