#pragma once
#include "Interfaces/IDrawable.h"

namespace ROC
{

class ITexture : public virtual IDrawable
{
public:
    enum TextureType
    {
        TT_None = -1,
        TT_RGB,
        TT_RGBA,
        TT_Cubemap
    };

    virtual bool IsCompressed() const = 0;
};

}