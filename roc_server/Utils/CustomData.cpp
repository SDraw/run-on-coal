#include "stdafx.h"

#include "Utils/CustomData.h"

ROC::CustomData::CustomData()
{
    m_type = CDT_None;
    m_ptr = nullptr;
}
ROC::CustomData::CustomData(const CustomData& f_data)
{
    m_type = f_data.m_type;
    m_ptr = nullptr;
    switch(m_type)
    {
        case CDT_Boolean:
            m_bool = f_data.m_bool;
            break;
        case CDT_Integer:
            m_int = f_data.m_int;
            break;
        case CDT_Double:
            m_double = f_data.m_double;
            break;
        case CDT_Float:
            m_float = f_data.m_float;
            break;
        case CDT_Element:
        {
            m_ptr = f_data.m_ptr;
            m_string.assign(f_data.m_string);
        } break;
        case CDT_String:
            m_string.assign(f_data.m_string);
            break;
    }
}
ROC::CustomData::~CustomData()
{
}

void ROC::CustomData::SetBoolean(bool f_val)
{
    m_bool = f_val;
    m_type = CDT_Boolean;
}
void ROC::CustomData::SetInteger(int f_val)
{
    m_int = f_val;
    m_type = CDT_Integer;
}
void ROC::CustomData::SetDouble(double f_val)
{
    m_double = f_val;
    m_type = CDT_Double;
}
void ROC::CustomData::SetFloat(float f_val)
{
    m_float = f_val;
    m_type = CDT_Float;
}
ROC::Element* ROC::CustomData::GetElement() const
{
    return reinterpret_cast<Element*>(m_ptr);
}
void ROC::CustomData::SetElement(Element *f_element)
{
    m_ptr = f_element;
    m_type = CDT_Element;
}
void ROC::CustomData::SetString(const std::string &f_val)
{
    m_string.assign(f_val);
    m_type = CDT_String;
}
void ROC::CustomData::SetString(const char *f_val, size_t f_size)
{
    m_string.assign(f_val, f_size);
    m_type = CDT_String;
}

ROC::CustomData& ROC::CustomData::operator=(const CustomData &f_data)
{
    m_type = f_data.m_type;
    switch(m_type)
    {
        case CDT_Boolean:
            m_bool = f_data.m_bool;
            break;
        case CDT_Integer:
            m_int = f_data.m_int;
            break;
        case CDT_Double:
            m_double = f_data.m_double;
            break;
        case CDT_Float:
            m_float = f_data.m_float;
            break;
        case CDT_Element:
        {
            m_ptr = f_data.m_ptr;
            m_string.assign(f_data.m_string);
        } break;
        case CDT_String:
            m_string.assign(f_data.m_string);
            break;
    }
    return *this;
}
