#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IDrawable : public virtual IElement
{
public:
    enum DrawableFilteringType : unsigned char
    {
        DFT_Nearest = 0U,
        DFT_Linear,

        DFT_None = 0xFFU
    };

    virtual bool IsTransparent() const = 0;
    virtual bool IsCubic() const = 0;

    virtual const glm::ivec2& GetSize() const = 0;
    virtual unsigned char GetFiltering() const = 0;
};

}