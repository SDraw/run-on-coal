#pragma once
#include "IElement.h"

namespace ROC
{

class ICamera;
class ILight;
class IModel;
class IRenderTarget;
class IShader;

class IScene : public virtual IElement
{
public:
    virtual bool IsActive() const = 0;
    virtual bool SetICamera(ICamera *f_cam) = 0;
    virtual bool RemoveCamera() = 0;
    virtual ICamera* GetICamera() const = 0;
    virtual bool SetIRenderTarget(IRenderTarget *f_rt) = 0;
    virtual bool RemoveRenderTarget() = 0;
    virtual IRenderTarget* GetIRenderTarget() const = 0;
    virtual bool SetIShader(IShader *f_shader) = 0;
    virtual bool RemoveIShader() = 0;
    virtual IShader* GetIShader() const = 0;
    virtual bool AddILight(ILight *f_light) = 0;
    virtual bool RemoveILight(ILight *f_light) = 0;
    virtual bool HasILight(ILight *f_light) const = 0;
    virtual bool AddIModel(IModel *f_model) = 0;
    virtual bool RemoveIModel(IModel *f_model) = 0;
    virtual bool HasIModel(IModel *f_model) const = 0;
};

}
