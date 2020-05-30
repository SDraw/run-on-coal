#pragma once

class GLViewport
{
    __declspec(thread) static GLint ms_positionX;
    __declspec(thread) static GLint ms_positionY;
    __declspec(thread) static GLsizei ms_width;
    __declspec(thread) static GLsizei ms_height;
    __declspec(thread) static float ms_clearColorR;
    __declspec(thread) static float ms_clearColorG;
    __declspec(thread) static float ms_clearColorB;
    __declspec(thread) static float ms_clearColorA;
    __declspec(thread) static GLenum ms_blendSource;
    __declspec(thread) static GLenum ms_blendDestination;
    __declspec(thread) static float ms_lineWidth;

    GLViewport() = delete;
    GLViewport(const GLViewport &that) = delete;
    GLViewport& operator=(const GLViewport &that) = delete;
    ~GLViewport() = delete;
public:
    static void SetViewport(GLint f_x, GLint f_y, GLsizei f_width, GLsizei f_height);
    static void Clear(bool f_depth, bool f_color);
    static void SetClearColor(float f_red, float f_green, float f_blue, float f_alpha);
    static void SetBlendFunctions(GLenum f_src, GLenum f_dst);
    static void SetLineWidth(float f_width);
};
