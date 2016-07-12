#pragma once
#define RENDERTARGET_TYPE_NONE -1
#define RENDERTARGET_TYPE_DEPTH 0
#define RENDERTARGET_TYPE_RGB 1
#define RENDERTARGET_TYPE_RGBA 2
#define RENDERTARGET_TYPE_RGBF 3

namespace ROC
{

class RenderTarget
{
    int m_type;
    GLuint m_frameBuffer;
    bool m_bFrameBuffer;
    GLuint m_renderBuffer;
    bool m_bRenderBuffer;
    GLuint m_texture;
    bool m_bTexture;
    glm::ivec2 m_size;

    std::string m_error;
    void Clear();
public:
    void BindTexture(unsigned int f_bind);
    void GetSize(glm::ivec2 &f_size);
    bool IsColored();
    bool IsTransparent();
    bool IsDepthable();
protected:
    RenderTarget();
    ~RenderTarget();
    bool Create(unsigned int f_num, glm::ivec2 &f_size, int f_type);
    void Enable();
    GLuint GetTexture();
    void GetError(std::string &f_str);
    friend class ElementManager;
    friend class RenderManager;
};

}
