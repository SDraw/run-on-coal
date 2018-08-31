#pragma once

namespace ROC
{

class GLBinder
{
    __declspec(thread) static GLuint ms_arrayBuffer;
    __declspec(thread) static GLuint ms_vertexArray;
    __declspec(thread) static GLuint ms_texture;
    __declspec(thread) static GLenum ms_textureType;
    __declspec(thread) static GLuint ms_frameBuffer;
    __declspec(thread) static GLuint ms_renderBuffer;
    __declspec(thread) static GLuint ms_shaderProgram;
    __declspec(thread) static int ms_viewportX, ms_viewportY, ms_viewportW, ms_viewportH;
public:
    enum GLBinderTextureType : GLenum
    {
        GLBTT_2D = GL_TEXTURE_2D,
        GLBTT_Cube = GL_TEXTURE_CUBE_MAP
    };

    static void BindArrayBuffer(GLuint f_buffer);
    inline static GLuint GetBindedArrayBuffer() { return ms_arrayBuffer; }
    static void ResetArrayBuffer(GLuint f_buffer = 0U);

    static void BindVertexArray(GLuint f_array);
    inline static GLuint GetBindedVertexArray() { return ms_vertexArray; }
    static void ResetVertexArray(GLuint f_array = 0U);

    static void BindTexture(GLuint f_texture, GLenum f_type);
    static void GetBindedTexture(GLuint &f_texture, GLenum &f_type);
    static void ResetTexture(GLuint f_texture = 0U);

    static void BindFramebuffer(GLuint f_buffer);
    inline static GLuint GetBindedFramebuffer() { return ms_frameBuffer; }
    static void ResetFramebuffer(GLuint f_buffer = 0U);

    static void BindRenderbuffer(GLuint f_buffer);
    inline static GLuint GetBindedRenderbuffer() { return ms_renderBuffer; }
    static void ResetRenderbuffer(GLuint f_buffer = 0U);

    static void UseShaderProgram(GLuint f_program);
    inline static GLuint GetUsedShaderProgram() { return ms_shaderProgram; }
    static void ResetShaderProgram(GLuint f_program = 0U);

    static void SetViewport(int f_px, int f_py, int f_width, int f_height);
};

}
