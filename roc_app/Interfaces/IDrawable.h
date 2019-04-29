#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IDrawable : public virtual IElement
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
    virtual int GetFiltering() const = 0;
};

}