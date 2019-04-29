#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IAnimation : public virtual IElement
{
public:
    virtual size_t GetBonesCount() const = 0;
    virtual unsigned int GetDuration() const = 0;
};

}
