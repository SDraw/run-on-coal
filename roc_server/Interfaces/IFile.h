#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class IFile : public virtual IElement
{
public:
    virtual size_t Read(std::string &f_data, size_t f_lenght) = 0;
    virtual size_t Write(const std::string &f_data) = 0;

    virtual size_t GetSize() = 0;
    virtual bool SetPosition(size_t f_pos) = 0;
    virtual size_t GetPosition() = 0;
    virtual bool IsEOF() const = 0;
    virtual void Flush() = 0;

    virtual const std::string& GetPath() const = 0;
};

}
