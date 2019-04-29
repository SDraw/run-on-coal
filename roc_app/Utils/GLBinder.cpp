#include "stdafx.h"
#include "Utils/GLBinder.h"

GLuint ROC::GLBinder::ms_arrayBuffer = 0U;
GLuint ROC::GLBinder::ms_vertexArray = 0U;
GLuint ROC::GLBinder::ms_texture2D = 0U;
GLuint ROC::GLBinder::ms_textureCubemap = 0U;
GLenum ROC::GLBinder::ms_textureType = 0U;
GLuint ROC::GLBinder::ms_frameBuffer = 0U;
GLuint ROC::GLBinder::ms_renderBuffer = 0U;
GLuint ROC::GLBinder::ms_shaderProgram = 0U;
int ROC::GLBinder::ms_viewportX = 0;
int ROC::GLBinder::ms_viewportY = 0;
int ROC::GLBinder::ms_viewportW = 0;
int ROC::GLBinder::ms_viewportH = 0;

void ROC::GLBinder::BindArrayBuffer(GLuint f_buffer)
{
    if((ms_arrayBuffer != f_buffer) && (f_buffer != 0U))
    {
        ms_arrayBuffer = f_buffer;
        glBindBuffer(GL_ARRAY_BUFFER, ms_arrayBuffer);
    }
}
void ROC::GLBinder::ResetArrayBuffer(GLuint f_buffer)
{
    if(f_buffer != 0U)
    {
        if(ms_arrayBuffer == f_buffer) ms_arrayBuffer = 0U;
    }
    else ms_arrayBuffer = 0U;
}

void ROC::GLBinder::BindVertexArray(GLuint f_array)
{
    if((ms_vertexArray != f_array) && (f_array != 0U))
    {
        ms_vertexArray = f_array;
        glBindVertexArray(ms_vertexArray);
    }
}
void ROC::GLBinder::ResetVertexArray(GLuint f_array)
{
    if(f_array != 0U)
    {
        if(ms_vertexArray == f_array) ms_vertexArray = 0U;
    }
    else ms_vertexArray = 0U;
}

void ROC::GLBinder::BindTexture2D(GLuint f_texture)
{
    if((ms_texture2D != f_texture) && (f_texture != 0U))
    {
        ms_texture2D = f_texture;
        glBindTexture(GL_TEXTURE_2D, ms_texture2D);
    }
}
void ROC::GLBinder::ResetTexture2D(GLuint f_texture)
{
    if(f_texture != 0U)
    {
        if(ms_texture2D == f_texture) ms_texture2D = 0U;
    }
    else ms_texture2D = 0U;
}

void ROC::GLBinder::BindTextureCubemap(GLuint f_texture)
{
    if((ms_textureCubemap != f_texture) && (f_texture != 0U))
    {
        ms_textureCubemap = f_texture;
        glBindTexture(GL_TEXTURE_CUBE_MAP, ms_textureCubemap);
    }
}
void ROC::GLBinder::ResetTextureCubemap(GLuint f_texture)
{
    if(f_texture != 0U)
    {
        if(ms_textureCubemap == f_texture) ms_textureCubemap = 0U;
    }
    else ms_textureCubemap = 0U;
}

void ROC::GLBinder::BindFramebuffer(GLuint f_buffer)
{
    if(ms_frameBuffer != f_buffer)
    {
        ms_frameBuffer = f_buffer;
        glBindFramebuffer(GL_FRAMEBUFFER, ms_frameBuffer);
    }
}
void ROC::GLBinder::ResetFramebuffer(GLuint f_buffer)
{
    if(((f_buffer != 0U) && (ms_frameBuffer == f_buffer)) || (ms_frameBuffer != 0U))
    {
        ms_frameBuffer = 0U;
        glBindFramebuffer(GL_FRAMEBUFFER, ms_frameBuffer);
    }
}

void ROC::GLBinder::BindRenderbuffer(GLuint f_buffer)
{
    if(ms_renderBuffer != f_buffer)
    {
        ms_renderBuffer = f_buffer;
        glBindRenderbuffer(GL_RENDERBUFFER, ms_renderBuffer);
    }
}
void ROC::GLBinder::ResetRenderbuffer(GLuint f_buffer)
{
    if(((f_buffer != 0U) && (ms_renderBuffer == f_buffer)) || (ms_renderBuffer != 0U))
    {
        ms_renderBuffer = 0U;
        glBindRenderbuffer(GL_RENDERBUFFER, ms_renderBuffer);
    }
}

void ROC::GLBinder::UseShaderProgram(GLuint f_program)
{
    if((ms_shaderProgram != f_program) && (f_program != 0U))
    {
        ms_shaderProgram = f_program;
        glUseProgram(ms_shaderProgram);
    }
}
void ROC::GLBinder::ResetShaderProgram(GLuint f_program)
{
    if(((f_program != 0U) && (ms_shaderProgram == f_program)) || (ms_shaderProgram != 0U))
    {
        ms_shaderProgram = 0U;
        glUseProgram(ms_shaderProgram);
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
