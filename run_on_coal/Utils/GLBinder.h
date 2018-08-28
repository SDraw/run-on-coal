#pragma once

namespace ROC
{

class GLBinder
{
    __declspec(thread) static GLuint ms_arrayBuffer;
    __declspec(thread) static GLuint ms_vertexArray;
    __declspec(thread) static GLuint ms_texture;
    __declspec(thread) static GLuint ms_frameBuffer;
    __declspec(thread) static int ms_viewportX, ms_viewportY, ms_viewportW, ms_viewportH;
public:
    static void BindArrayBuffer(GLuint f_buffer);
    inline static bool IsArrayBufferBinded(GLuint f_buffer) { return (f_buffer == ms_arrayBuffer); }
    inline static void ResetArrayBuffer() { ms_arrayBuffer = 0U; }

    static void BindVertexArray(GLuint f_array);
    inline static bool IsVertexArrayBinded(GLuint f_array) { return (f_array == ms_vertexArray); }
    inline static void ResetVertexArray() { ms_vertexArray = 0U; }

    static void BindTexture2D(GLuint f_texture);
    static void BindTextureCube(GLuint f_texture);
    inline static bool IsTextureBinded(GLuint f_texture) { return (f_texture == ms_texture); }
    inline static void ResetTexture() { ms_texture = 0U; }

    static void BindFramebuffer(GLuint f_buffer);
    inline static bool IsFramebufferBinded(GLuint f_buffer) { return (f_buffer == ms_frameBuffer); }
    inline static void ResetFramebuffer() { ms_frameBuffer = 0U; }

    static void SetViewport(int f_px, int f_py, int f_width, int f_height);
};

}
