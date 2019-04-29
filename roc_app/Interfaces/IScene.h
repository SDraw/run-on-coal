#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class ICamera;
class IRenderTarget;
class IShader;

class IScene : public virtual IElement
{
public:
    virtual ICamera* GetCamera() const = 0;
    virtual IRenderTarget* GetRenderTarget() const = 0;
    virtual IShader* GetShader() const = 0;

    virtual size_t GetLightsCount() const = 0;
    virtual bool IsActive() const = 0;
};

}
