#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IFont : public virtual IElement
{
public:
    enum FontFilteringType
    {
        FFT_None = -1,
        FFT_Nearest,
        FFT_Linear
    };

    virtual int GetFiltering() const = 0;
    virtual float GetGlyphSize() const = 0;
};

}