#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class ICollidable : public virtual IElement
{
public:
    virtual bool SetCollidableWith(ICollidable *f_col, bool f_state) = 0;
};

}
