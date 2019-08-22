#pragma once
#include "Interfaces/IDrawable.h"
#include "Elements/Element.h"

namespace ROC
{

class Drawable : public Element, public virtual IDrawable
{
    Drawable(const Drawable &that) = delete;
    Drawable& operator=(const Drawable &that) = delete;
public:
    virtual bool IsTransparent() const = 0;
    virtual bool IsCubic() const = 0;

    virtual const glm::ivec2& GetSize() const = 0;
    unsigned char GetFiltering() const;
protected:
    unsigned char m_filtering;

    Drawable();
    virtual ~Drawable();

    virtual void Bind() = 0;

    friend class RenderManager;
    friend class VRManager;
    friend class Shader;
};

}
