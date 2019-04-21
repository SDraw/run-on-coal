#include "stdafx.h"

#include "Elements/Element.h"

#define ROC_ELEMENT_CUSTOMDATA_KEYLENGTH 128U

ROC::Element::Element()
{
    m_elementType = 0xFF;
}
ROC::Element::~Element()
{
    m_customDataMap.clear();
}

bool ROC::Element::SetData(const std::string &f_key, const CustomArgument &f_val)
{
    bool l_result = false;
    if(f_key.size() <= ROC_ELEMENT_CUSTOMDATA_KEYLENGTH)
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
