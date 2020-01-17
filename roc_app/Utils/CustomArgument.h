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

    inline unsigned char GetType() const { return m_type; }

    inline bool GetBoolean() const { return m_bool; }
    inline int GetInteger() const { return m_int; }
    inline int GetUInteger() const { return m_uint; }
    inline float GetFloat() const { return m_float; }
    inline double GetDouble() const { return m_double; }
    inline void* GetPointer() const { return m_ptr; }
    inline ROC::IElement* GetElement() const { return reinterpret_cast<ROC::IElement*>(m_ptr); };
    inline const std::string& GetString() const { return m_string; }

    CustomArgument& operator=(const CustomArgument &f_data);
};

}
