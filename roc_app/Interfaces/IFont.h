#pragma once
#include "IElement.h"

namespace ROC
{

class IFont : public virtual IElement
{
public:
    enum FontFilteringType : unsigned char
    {
        FFT_Nearest = 0U,
        FFT_Linear,

        FFT_None = 0xFFU
    };

    virtual unsigned char GetFiltering() const = 0;
    virtual float GetGlyphSize() const = 0;
};

}
