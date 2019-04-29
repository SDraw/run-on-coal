#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IShaderUniform;

class IShader : public virtual IElement
{
public:
    virtual IShaderUniform* GetUniform(const std::string &f_uniform) = 0;
};

}
