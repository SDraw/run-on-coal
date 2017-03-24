#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Drawable : public Element
{
public:
    virtual bool IsTransparent() const = NULL;
protected:
    Drawable();
    virtual ~Drawable();

    virtual GLuint GetTextureID() const = NULL;
    virtual void Bind() = NULL;

    friend class RenderManager;
    friend class Shader;
};

}
