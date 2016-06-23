#pragma once

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
    enum RenderTargetType {
        Depth = 0U, RGB, RGBA, RGBF
    };
protected:
    RenderTarget();
    ~RenderTarget();
    bool Create(unsigned int f_num,glm::ivec2 &f_size,unsigned int f_type);
    void Enable();
    GLuint GetTexture();
    void GetError(std::string &f_str);
    friend class ElementManager;
    friend class RenderManager;
};

}
