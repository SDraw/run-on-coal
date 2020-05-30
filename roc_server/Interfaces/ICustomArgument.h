#pragma once

namespace ROC
{

class IElement;

class ICustomArgument
{
public:
    enum CustomArgumentType : unsigned char
    {
        CAT_None = 0U,
        CAT_Boolean,
        CAT_Integer,
        CAT_UInteger,
        CAT_Float,
        CAT_Double,
        CAT_Pointer,
        CAT_String,
        CAT_Element
    };

    virtual unsigned char GetType() const = 0;
    virtual bool GetBoolean() const = 0;
    virtual int GetInteger() const = 0;
    virtual int GetUInteger() const = 0;
    virtual float GetFloat() const = 0;
    virtual double GetDouble() const = 0;
    virtual void* GetPointer() const = 0;
    virtual IElement* GetElement() const = 0;
    virtual const std::string& GetString() const = 0;
};

}
