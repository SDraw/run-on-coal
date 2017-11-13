#pragma once

namespace ROC
{

class GLBinder
{
    __declspec(thread) static GLuint ms_arrayBuffer;
    __declspec(thread) static GLuint ms_vertexArray;
    __declspec(thread) static GLuint ms_texture;
    __declspec(thread) static GLuint ms_frameBuffer;
public:
    static void BindArrayBuffer(GLuint buffer);
    inline static bool IsArrayBufferBinded(GLuint f_buffer) { return (f_buffer == ms_arrayBuffer); }

    static void BindVertexArray(GLuint f_array);
    inline static bool IsVertexArrayBinded(GLuint f_array) { return (f_array == ms_vertexArray); }

    static void BindTexture2D(GLuint f_texture);
    static void BindTextureCube(GLuint f_texture);
    inline static bool IsTextureBinded(GLuint f_texture) { return (f_texture == ms_texture); }

    static void BindFramebuffer(GLuint f_buffer);
    inline static bool IsFramebufferBinded(GLuint f_buffer) { return (f_buffer == ms_frameBuffer); }
};

}
