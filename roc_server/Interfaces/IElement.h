#pragma once

class CustomArgument;

namespace ROC
{

class IElement
{
public:
    enum ElementType : unsigned char
    { 
        ET_Client = 0U, 
        ET_File
    };

    virtual bool SetData(const std::string &f_key, const CustomArgument &f_val) = 0;
    virtual bool GetData(const std::string &f_key, CustomArgument &f_val) = 0;
    virtual bool RemoveData(const std::string &f_key) = 0;

    virtual unsigned char GetElementType() const = 0;
    virtual const std::string& GetElementTypeName() const = 0;
};

}
