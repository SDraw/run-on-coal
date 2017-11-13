#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Drawable : public Element
{
public:
    enum DrawableFilteringType
    {
        DFT_None = -1,
        DFT_Nearest,
        DFT_Linear
    };

    virtual bool IsTransparent() const = 0;
    virtual bool IsCubic() const = 0;

    virtual const glm::ivec2& GetSize() const = 0;
    inline int GetFiltering() const { return m_filtering; };
protected:
    int m_filtering;

    Drawable();
    virtual ~Drawable();

    virtual void Bind() = 0;

    friend class RenderManager;
    friend class Shader;
};

}
