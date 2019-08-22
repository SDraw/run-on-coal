#pragma once
#include "Interfaces/IRenderTarget.h"
#include "Elements/Drawable.h"

namespace ROC
{

class RenderTarget final : public Drawable, public virtual IRenderTarget
{
    unsigned char m_type;

    GLuint m_frameBuffer;
    GLuint m_renderBuffer;
    GLuint m_texture;
    glm::ivec2 m_size;

    std::string m_error;

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
protected:
    RenderTarget();
    ~RenderTarget();

    bool Create(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter);
    inline const std::string& GetError() const { return m_error; }

    inline GLuint GetTextureID() const { return m_texture; };

    void Bind();
    void Enable();
    void Disable();
    
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
