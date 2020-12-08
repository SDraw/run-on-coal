#pragma once

class GLSetting final
{
    __declspec(thread) static std::map<GLenum, bool> *ms_settings;
    __declspec(thread) static bool ms_depthMask;
    __declspec(thread) static GLenum ms_fillMode;
    __declspec(thread) static GLint ms_pixelUnpackAlignment;

    GLSetting() = delete;
    GLSetting(const GLSetting &that) = delete;
    GLSetting& operator=(const GLSetting &that) = delete;
    ~GLSetting() = delete;
public:
    static void Set(GLenum f_setting, bool f_state);
    static void SetDepthMask(bool f_state);
    static void SetFillMode(GLenum f_mode);
    static void SetPixelUnpackAlignment(GLint f_value);
    static const unsigned char* GetString(GLenum f_value);
    static void GetInteger(GLenum f_val, GLint *f_int);

    static void MemoryCleanup();
};
