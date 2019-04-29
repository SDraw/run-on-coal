#pragma once

namespace ROC
{

class ILogManager
{
public:
    virtual void Log(const std::string &f_text) = 0;
};

}
