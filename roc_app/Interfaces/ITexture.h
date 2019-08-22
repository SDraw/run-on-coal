#pragma once
#include "Interfaces/IDrawable.h"

namespace ROC
{

class ITexture : public virtual IDrawable
{
public:
    enum TextureType : unsigned char
    {
        TT_RGB,
        TT_RGBA,
        TT_Cubemap,

        TT_None = 0xFFU
    };

    virtual bool IsCompressed() const = 0;
};

}