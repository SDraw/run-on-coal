#include "stdafx.h"

#include "GL/GLSetting.h"

std::map<GLenum, bool> *GLSetting::ms_settings = nullptr;
bool GLSetting::ms_depthMask = false;
GLenum GLSetting::ms_fillMode = GL_FILL;
GLint GLSetting::ms_pixelUnpackAlignment = 4;

void GLSetting::Set(GLenum f_setting, bool f_state)
{
    if(!ms_settings) ms_settings = new std::map<GLenum, bool>();
    auto l_setting = ms_settings->find(f_setting);
    if(l_setting != ms_settings->end())
    {
        if(l_setting->second != f_state)
        {
            l_setting->second = f_state;
            f_state ? glEnable(f_setting) : glDisable(f_setting);
        }
    }
    else
    {
        ms_settings->emplace(f_setting, f_state);
        f_state ? glEnable(f_setting) : glDisable(f_setting);
    }
}

void GLSetting::SetDepthMask(bool f_state)
{
    if(ms_depthMask != f_state)
    {
        glDepthMask(f_state ? 1U : 0U);
        ms_depthMask = f_state;
    }
}

void GLSetting::SetFillMode(GLenum f_mode)
{
    if(ms_fillMode != f_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, f_mode);
        ms_fillMode = f_mode;
    }
}

void GLSetting::SetPixelUnpackAlignment(GLint f_value)
{
    if(ms_pixelUnpackAlignment != f_value)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, f_value);
        ms_pixelUnpackAlignment = f_value;
    }
}

const unsigned char* GLSetting::GetString(GLenum f_value)
{
    return glGetString(f_value);
}

void GLSetting::GetInteger(GLenum f_val, GLint *f_int)
{
    glGetIntegerv(f_val, f_int);
}

void GLSetting::MemoryCleanup()
{
    delete ms_settings;
    ms_settings = nullptr;
}
