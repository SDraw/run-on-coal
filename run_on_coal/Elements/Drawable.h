#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Drawable : public Element
{
public:
    virtual bool IsTransparent() const = 0;
protected:
    Drawable();
    virtual ~Drawable();

    virtual GLuint GetTextureID() const = 0;
    virtual void Bind() = 0;

    friend class RenderManager;
    friend class Shader;
};

}
