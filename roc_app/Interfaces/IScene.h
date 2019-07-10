#pragma once
#include "Interfaces/IElement.h"

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
    virtual bool SetCamera(ICamera *f_cam) = 0;
    virtual bool RemoveCamera() = 0;
    virtual ICamera* GetCamera() const = 0;

    virtual bool SetRenderTarget(IRenderTarget *f_rt) = 0;
    virtual bool RemoveRenderTarget() = 0;
    virtual IRenderTarget* GetRenderTarget() const = 0;

    virtual bool SetShader(IShader *f_shader) = 0;
    virtual bool RemoveShader() = 0;
    virtual IShader* GetShader() const = 0;

    virtual bool AddLight(ILight *f_light) = 0;
    virtual bool RemoveLight(ILight *f_light) = 0;
    virtual bool HasLight(ILight *f_light) const = 0;

    virtual bool AddModel(IModel *f_model) = 0;
    virtual bool RemoveModel(IModel *f_model) = 0;
    virtual bool HasModel(IModel *f_model) const = 0;

    virtual bool IsActive() const = 0;
};

}
