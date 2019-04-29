#pragma once
#include "Interfaces/IDrawable.h"

namespace ROC
{

class IRenderTarget : public virtual IDrawable
{
public:
    enum RenderTargetType
    {
        RTT_None = -1,
        RTT_Shadow,
        RTT_RGB,
        RTT_RGBA,
        RTT_RGBF,
        RTT_RGBAF
    };

    virtual bool IsShadowType() const = 0;
};

}