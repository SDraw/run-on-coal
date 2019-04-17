#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class RenderTarget final : public Drawable
{
    int m_type;

    GLuint m_frameBuffer;
    GLuint m_renderBuffer;
    GLuint m_texture;
    glm::ivec2 m_size;

    std::string m_error;

    RenderTarget(const RenderTarget &that) = delete;
    RenderTarget& operator=(const RenderTarget &that) = delete;

    void Clear();
public:
    enum RenderTargetType
    {
        RTT_None = -1,
        RTT_Shadow,
        RTT_RGB,
        RTT_RGBA,
        RTT_RGBF,
        RTT_RGBAF
    };

    inline const glm::ivec2& GetSize() const { return m_size; }

    inline bool IsTransparent() const { return ((m_type == RTT_RGBA) || (m_type == RTT_RGBAF)); }
    inline bool IsCubic() const { return false; }
    inline bool IsShadowType() const { return (m_type == RTT_Shadow); }
protected:
    RenderTarget();
    ~RenderTarget();
    bool Create(int f_type, const glm::ivec2 &f_size, int f_filter = DFT_Nearest);
    inline const std::string& GetError() const { return m_error; }

    inline GLuint GetTextureID() const { return m_texture; };

    void Bind();
    void Enable();
    void Disable();

    friend class ElementManager;
    friend class RenderManager;
    friend class VRManager;
    friend class Scene;
    friend class Shader;
};

}
