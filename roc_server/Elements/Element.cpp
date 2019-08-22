#include "stdafx.h"

#include "Elements/Element.h"

namespace ROC
{

const std::string g_ElementTypeName[] = {
    "Client",
    "File"
};
const std::string g_ElementInvalidName = "Invalid";

const size_t g_ElementKeyMaxLength = 128U;

}

ROC::Element::Element()
{
    m_elementType = ET_Invalid;
}
ROC::Element::~Element()
{
    m_customDataMap.clear();
}

unsigned char ROC::Element::GetElementType() const 
{ 
    return m_elementType; 
}
const std::string& ROC::Element::GetElementTypeName() const 
{ 
    return ((m_elementType != ET_Invalid) ? g_ElementTypeName[m_elementType] : g_ElementInvalidName);
}


bool ROC::Element::SetData(const std::string &f_key, const CustomArgument &f_val)
{
    bool l_result = false;
    if(f_key.size() <= g_ElementKeyMaxLength)
    {
        auto iter = m_customDataMap.find(f_key);
        if(iter == m_customDataMapEnd)
        {
            m_customDataMap.insert(std::make_pair(f_key, f_val));
            m_customDataMapEnd = m_customDataMap.end();
        }
        else iter->second = f_val;
        l_result = true;
    }
    return l_result;
}
bool ROC::Element::GetData(const std::string &f_key, CustomArgument &f_val)
{
    bool l_result = false;
    auto iter = m_customDataMap.find(f_key);
    if(iter != m_customDataMapEnd)
    {
        f_val = iter->second;
        l_result = true;
    }
    return l_result;
}
bool ROC::Element::RemoveData(const std::string &f_key)
{
    bool l_result = false;
    auto iter = m_customDataMap.find(f_key);
    if(iter != m_customDataMapEnd)
    {
        m_customDataMap.erase(iter);
        m_customDataMapEnd = m_customDataMap.end();
        l_result = true;
    }
    return l_result;
}
