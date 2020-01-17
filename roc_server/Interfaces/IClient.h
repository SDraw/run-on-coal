#pragma once
#include "IElement.h"

namespace ROC
{

class IClient : public virtual IElement
{
public:
    virtual unsigned int GetID() const = 0;
    virtual void GetAddress(std::string &f_ip, unsigned short &f_port) const = 0;
};

}
