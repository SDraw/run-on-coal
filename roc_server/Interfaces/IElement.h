#pragma once

namespace ROC
{

class IElement
{
public:
    enum ElementType : unsigned char
    {
        ET_Client = 0U,
        ET_File,

        ET_Invalid = 0xFFU
    };

    virtual unsigned char GetElementType() const = 0;
    virtual const std::string& GetElementTypeName() const = 0;
};

}
