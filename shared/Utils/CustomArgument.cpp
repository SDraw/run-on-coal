#include "stdafx.h"

#include "Utils/CustomArgument.h"

CustomArgument::CustomArgument()
{
    m_type = CAT_Nil;
    m_ptr = nullptr;
}
CustomArgument::CustomArgument(bool f_val)
{
    m_type = CAT_Boolean;
    m_bool = f_val;
}
CustomArgument::CustomArgument(int f_val)
{
    m_type = CAT_Integer;
    m_int = f_val;
}
CustomArgument::CustomArgument(unsigned int f_val)
{
    m_type = CAT_UInteger;
    m_uint = f_val;
}
CustomArgument::CustomArgument(float f_val)
{
    m_type = CAT_Float;
    m_float = f_val;
}
CustomArgument::CustomArgument(double f_val)
{
    m_type = CAT_Double;
    m_double = f_val;
}
CustomArgument::CustomArgument(void *f_val)
{
    m_type = CAT_Pointer;
    m_ptr = f_val;
}
CustomArgument::CustomArgument(const std::string &f_val)
{
    m_string.assign(f_val);
    m_type = CAT_String;
}
CustomArgument::CustomArgument(ROC::IElement *f_element)
{
    m_ptr = f_element;
    m_type = CAT_Element;
}
CustomArgument::CustomArgument(const CustomArgument& f_data)
{
    m_type = f_data.m_type;
    m_ptr = nullptr;
    switch(m_type)
    {
        case CAT_Boolean:
            m_bool = f_data.m_bool;
            break;
        case CAT_Integer:
            m_int = f_data.m_int;
            break;
        case CAT_UInteger:
            m_uint = f_data.m_uint;
            break;
        case CAT_Float:
            m_float = f_data.m_float;
            break;
        case CAT_Double:
            m_double = f_data.m_double;
            break;
        case CAT_Pointer: case CAT_Element:
            m_ptr = f_data.m_ptr;
            break;
        case CAT_String:
            m_string.assign(f_data.m_string);
            break;
    }
}
CustomArgument::~CustomArgument()
{
}

CustomArgument& CustomArgument::operator=(const CustomArgument &f_data)
{
    m_type = f_data.m_type;
    m_ptr = nullptr;
    switch(m_type)
    {
        case CAT_Boolean:
            m_bool = f_data.m_bool;
            break;
        case CAT_Integer:
            m_int = f_data.m_int;
            break;
        case CAT_UInteger:
            m_uint = f_data.m_uint;
            break;
        case CAT_Float:
            m_float = f_data.m_float;
            break;
        case CAT_Double:
            m_double = f_data.m_double;
            break;
        case CAT_Pointer: case CAT_Element:
            m_ptr = f_data.m_ptr;
            break;
        case CAT_String:
            m_string.assign(f_data.m_string);
            break;
    }
    return *this;
}
