#pragma once
#include "Interfaces/IRenderTarget.h"
#include "Elements/Drawable.h"

namespace ROC
{

class RenderTarget final : public Drawable, public virtual IRenderTarget
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
    const glm::ivec2& GetSize() const;

    bool IsTransparent() const;
    bool IsCubic() const;
    bool IsShadowType() const;
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
