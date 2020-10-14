#pragma once
#include "Interfaces/ICustomArgument.h"

namespace ROC
{

class CustomArgument final : public ICustomArgument
{
    unsigned char m_type;

    union
    {
        bool m_bool;
        int m_int;
        unsigned int m_uint;
        float m_float;
        double m_double;
        void *m_ptr;
    };
    std::string m_string;
public:
    CustomArgument();
    explicit CustomArgument(bool f_val);
    explicit CustomArgument(int f_val);
    explicit CustomArgument(unsigned int f_val);
    explicit CustomArgument(float f_val);
    explicit CustomArgument(double f_val);
    explicit CustomArgument(void *f_val);
    explicit CustomArgument(const std::string &f_val);
    explicit CustomArgument(ROC::IElement *f_element);
    CustomArgument(const CustomArgument &f_data);
    ~CustomArgument();

    unsigned char GetType() const;

    bool GetBoolean() const;
    int GetInteger() const;
    int GetUInteger() const;
    float GetFloat() const;
    double GetDouble() const;
    void* GetPointer() const;
    ROC::IElement* GetElement() const;
    const std::string& GetString() const;

    CustomArgument& operator=(const CustomArgument &f_data);
};

}
