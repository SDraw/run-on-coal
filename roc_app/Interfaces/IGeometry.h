#pragma once
#include "IElement.h"

namespace ROC
{

class IGeometry : public virtual IElement
{
public:
    virtual float GetBoundSphereRadius() const = 0;
    virtual size_t GetMaterialsCount() const = 0;
};

}
