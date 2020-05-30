#include "stdafx.h"

#include "GL/GLViewport.h"

GLint GLViewport::ms_positionX = 0;
GLint GLViewport::ms_positionY = 0;
GLsizei GLViewport::ms_width = 0;
GLsizei GLViewport::ms_height = 0;
float GLViewport::ms_clearColorR = 0.f;
float GLViewport::ms_clearColorG = 0.f;
float GLViewport::ms_clearColorB = 0.f;
float GLViewport::ms_clearColorA = 0.f;
GLenum GLViewport::ms_blendSource = GL_ONE;
GLenum GLViewport::ms_blendDestination = GL_ZERO;
float GLViewport::ms_lineWidth = 1.f;

void GLViewport::SetViewport(GLint f_x, GLint f_y, GLsizei f_width, GLsizei f_height)
{
    bool l_update = false;
    if(ms_positionX != f_x)
    {
        ms_positionX = f_x;
        l_update = true;
    }
    if(ms_positionY != f_y)
    {
        ms_positionY = f_y;
        l_update = true;
    }
    if(ms_width != f_width)
    {
        ms_width = f_width;
        l_update = true;
    }
    if(ms_height != f_height)
    {
        ms_height = f_height;
        l_update = true;
    }

    if(l_update) glViewport(ms_positionX, ms_positionY, ms_width, ms_height);
}

void GLViewport::Clear(bool f_depth, bool f_color)
{
    GLenum l_mode = 0;
    if(f_depth) l_mode |= GL_DEPTH_BUFFER_BIT;
    if(f_color) l_mode |= GL_COLOR_BUFFER_BIT;
    if(l_mode) glClear(l_mode);
}

void GLViewport::SetClearColor(float f_red, float f_green, float f_blue, float f_alpha)
{
    bool l_update = false;
    if(ms_clearColorR != f_red)
    {
        ms_clearColorR = f_red;
        l_update = true;
    }
    if(ms_clearColorG != f_green)
    {
        ms_clearColorG = f_green;
        l_update = true;
    }
    if(ms_clearColorB != f_blue)
    {
        ms_clearColorB = f_blue;
        l_update = true;
    }
    if(ms_clearColorA != f_alpha)
    {
        ms_clearColorA = f_alpha;
        l_update = true;
    }
    if(l_update) glClearColor(ms_clearColorR, ms_clearColorG, ms_clearColorB, ms_clearColorA);
}

void GLViewport::SetBlendFunctions(GLenum f_src, GLenum f_dst)
{
    bool l_update = false;
    if(ms_blendSource != f_src)
    {
        ms_blendSource = f_src;
        l_update = true;
    }
    if(ms_blendDestination != f_dst)
    {
        ms_blendDestination = f_dst;
        l_update = true;
    }
    if(l_update) glBlendFunc(ms_blendSource, ms_blendDestination);
}

void GLViewport::SetLineWidth(float f_width)
{
    if(ms_lineWidth != f_width)
    {
        ms_lineWidth = f_width;
        glLineWidth(ms_lineWidth);
    }
}