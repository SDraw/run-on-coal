#include "stdafx.h"

#include "Elements/Element.h"

namespace ROC
{

extern const std::vector<std::string> g_ElementTypeNames;

}

ROC::Element::Element()
{
    m_elementType = 0xFF;
    m_customDataMapEnd = m_customDataMap.end();
}
ROC::Element::~Element()
{
    m_customDataMap.clear();
}

bool ROC::Element::SetCustomData(const std::string &f_key, CustomData &f_val)
{
    bool l_result = false;
    if(f_key.size() <= ROC_CUSTOM_DATA_LENGTH)
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
void ROC::Element::GetCustomData(const std::string &f_key, CustomData &f_val)
{
    auto iter = m_customDataMap.find(f_key);
    if(iter != m_customDataMapEnd) f_val = iter->second;
}
bool ROC::Element::RemoveCustomData(const std::string &f_key)
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
