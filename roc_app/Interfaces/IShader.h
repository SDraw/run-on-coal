#pragma once
#include "IElement.h"

namespace ROC
{

class IShaderUniform;
class IDrawable;

class IShader : public virtual IElement
{
public:
    virtual IShaderUniform* GetIUniform(const std::string &f_uniform) = 0;
    virtual bool Attach(IDrawable *f_drawable, const std::string &f_uniform) = 0;
    virtual bool Detach(IDrawable *f_drawable) = 0;
    virtual bool HasAttached(IDrawable *f_drawable) const = 0;
};

}
