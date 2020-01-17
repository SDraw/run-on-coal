#pragma once
#include "IDrawable.h"

namespace ROC
{

class IRenderTarget : public virtual IDrawable
{
public:
    enum RenderTargetType : unsigned char
    {
        RTT_Shadow,
        RTT_RGB,
        RTT_RGBA,
        RTT_RGBF,
        RTT_RGBAF,

        RTT_None = 0xFFU
    };

    virtual bool IsShadowType() const = 0;
};

}