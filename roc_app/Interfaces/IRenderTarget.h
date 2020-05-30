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
    enum RenderTargetProperty : unsigned char
    {
        RTP_ClearColor = 0U,
        RTP_Depth,
        RTP_Color
    };

    virtual bool IsShadowType() const = 0;
    virtual bool SetProperty(RenderTargetProperty f_prop, const void *f_val) = 0;
};

}
