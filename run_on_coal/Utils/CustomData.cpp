#include "stdafx.h"

#include "Utils/CustomData.h"

ROC::CustomData::CustomData()
{
    m_type = DataType::None;
}
ROC::CustomData::CustomData(const CustomData& f_data)
{
    m_type = f_data.m_type;
    switch(m_type)
    {
        case DataType::Boolean:
            m_bool = f_data.m_bool;
            break;
        case DataType::Integer:
            m_int = f_data.m_int;
            break;
        case DataType::Double:
            m_double = f_data.m_double;
            break;
        case DataType::Float:
            m_float = f_data.m_float;
            break;
        case DataType::Element:
        {
            m_ptr = f_data.m_ptr;
            m_string.assign(f_data.m_string);
        } break;
        case DataType::String:
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
    m_type = CustomData::Boolean;
}
void ROC::CustomData::SetInteger(int f_val)
{
    m_int = f_val;
    m_type = CustomData::Integer;
}
void ROC::CustomData::SetDouble(double f_val)
{
    m_double = f_val;
    m_type = DataType::Double;
}
void ROC::CustomData::SetFloat(float f_val)
{
    m_float = f_val;
    m_type = DataType::Float;
}
void ROC::CustomData::GetElement(void *&f_ptr, std::string &f_name) const
{
    f_ptr = m_ptr;
    f_name.assign(m_string);
}
void ROC::CustomData::SetElement(void *f_ptr, const std::string &f_name)
{
    m_ptr = f_ptr;
    m_string.assign(f_name);
    m_type = DataType::Element;
}
void ROC::CustomData::SetString(const std::string &f_val)
{
    m_string.assign(f_val);
    m_type = DataType::String;
}
void ROC::CustomData::SetString(const char *f_val, size_t f_size)
{
    m_string.assign(f_val, f_size);
    m_type = DataType::String;
}

ROC::CustomData& ROC::CustomData::operator=(CustomData &f_data)
{
    m_type = f_data.m_type;
    switch(m_type)
    {
        case DataType::Boolean:
            m_bool = f_data.m_bool;
            break;
        case DataType::Integer:
            m_int = f_data.m_int;
            break;
        case DataType::Double:
            m_double = f_data.m_double;
            break;
        case DataType::Float:
            m_float = f_data.m_float;
            break;
        case DataType::Element:
        {
            m_ptr = f_data.m_ptr;
            m_string.assign(f_data.m_string);
        } break;
        case DataType::String:
            m_string.assign(f_data.m_string);
            break;
    }
    return *this;
}
