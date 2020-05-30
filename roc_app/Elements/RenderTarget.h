#pragma once
#include "Interfaces/IRenderTarget.h"
#include "Elements/Drawable.h"

class GLTexture2D;
class GLFramebuffer;
class GLRenderbuffer;

namespace ROC
{

class RenderTarget final : public Drawable, public virtual IRenderTarget
{
    unsigned char m_type;

    GLFramebuffer *m_framebuffer;
    GLRenderbuffer *m_renderbuffer;
    GLTexture2D *m_texture;
    glm::ivec2 m_size;

    glm::vec4 m_clearColor;
    glm::bvec2 m_clearBuffer; // Depth, color

    static RenderTarget *ms_fallbackRT;
    static glm::ivec2 ms_fallbackSize;

    RenderTarget(const RenderTarget &that) = delete;
    RenderTarget& operator=(const RenderTarget &that) = delete;

    void Clear();
public:
    const glm::ivec2& GetSize() const;

    bool IsTransparent() const;
    bool IsCubic() const;
    bool IsShadowType() const;

    bool SetProperty(RenderTargetProperty f_prop, const void *f_val);
protected:
    RenderTarget();
    ~RenderTarget();

    bool Create(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter);

    inline GLTexture2D* GetGLTexture() { return m_texture; }

    void Bind(size_t f_slot);
    void Enable(bool f_clear = true);
    static void Disable();

    static inline void SetFallbackRenderTarget(RenderTarget *f_rt) { ms_fallbackRT = f_rt; }
    static inline void SetFallbackSize(const glm::ivec2 &f_size) { std::memcpy(&ms_fallbackSize, &f_size, sizeof(glm::ivec2)); }
    static void Fallback();

    friend class ElementManager;
    friend class RenderManager;
    friend class VRManager;
    friend class Scene;
    friend class Shader;
};

}
