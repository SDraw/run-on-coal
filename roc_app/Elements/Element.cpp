#include "stdafx.h"

#include "Elements/Element.h"

namespace ROC
{

const std::string g_ElementTypeName[] = {
    "Geometry",
    "Model",
    "Animation",
    "Scene",
    "Camera",
    "Light",
    "RenderTarget",
    "Shader",
    "Sound",
    "Texture",
    "Font",
    "File",
    "Collision"
};
const std::string g_ElementInvalidName = "Invalid";

const size_t g_ElementKeyMaxLength = 128U;

}

ROC::Element::Element()
{
    m_elementType = ET_Invalid;
    m_customDataMapEnd = m_customDataMap.end();
}
ROC::Element::~Element()
{
    for(auto l_child : m_children) l_child->OnParentLinkDestroyed(this);
    m_children.clear();

    for(auto l_parent : m_parents) l_parent->OnChildLinkDestroyed(this);
    m_parents.clear();

    m_customDataMap.clear();
}

bool ROC::Element::HasChildren() const
{
    return m_children.empty();
}
bool ROC::Element::HasParents() const
{
    return m_parents.empty();
}

bool ROC::Element::SetData(const std::string &f_key, const CustomArgument &f_val)
{
    bool l_result = false;
    if(f_key.length() <= g_ElementKeyMaxLength)
    {
        auto iter = m_customDataMap.find(f_key);
        if(iter == m_customDataMapEnd)
        {
            m_customDataMap.emplace(f_key, f_val);
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

unsigned char ROC::Element::GetElementType() const
{
    return m_elementType;
}
const std::string& ROC::Element::GetElementTypeName() const
{
    return ((m_elementType != ET_Invalid) ? g_ElementTypeName[m_elementType] : g_ElementInvalidName);
}

void ROC::Element::AddChild(Element *f_child)
{
    m_children.push_back(f_child);
}
bool ROC::Element::HasChild(Element *f_child)
{
    bool l_result = (std::find(m_children.begin(), m_children.end(), f_child) != m_children.end());
    return l_result;
}
void ROC::Element::RemoveChild(Element *f_child)
{
    for(auto l_searchIter = m_children.begin(), l_end = m_children.end(); l_searchIter != l_end; ++l_searchIter)
    {
        if(*l_searchIter == f_child)
        {
            m_children.erase(l_searchIter);
            f_child->OnParentLinkDestroyed(this);
            break;
        }
    }
}

void ROC::Element::AddParent(Element *f_parent)
{
    m_parents.push_back(f_parent);
}
bool ROC::Element::HasParent(Element *f_parent)
{
    bool l_result = (std::find(m_parents.begin(), m_parents.end(), f_parent) != m_parents.end());
    return l_result;
}
void ROC::Element::RemoveParent(Element *f_parent)
{
    for(auto l_searchIter = m_parents.begin(), l_end = m_parents.end(); l_searchIter != l_end; ++l_searchIter)
    {
        if(*l_searchIter == f_parent)
        {
            m_parents.erase(l_searchIter);
            f_parent->OnChildLinkDestroyed(this);
            break;
        }
    }
}

void ROC::Element::OnParentLinkDestroyed(Element *f_parent)
{
    for(auto l_searchIter = m_parents.begin(), l_end = m_parents.end(); l_searchIter != l_end; ++l_searchIter)
    {
        if((*l_searchIter) == f_parent)
        {
            m_parents.erase(l_searchIter);
            break;
        }
    }
}
void ROC::Element::OnChildLinkDestroyed(Element *f_child)
{
    for(auto l_searchIter = m_children.begin(), l_end = m_children.end(); l_searchIter != l_end; ++l_searchIter)
    {
        if((*l_searchIter) == f_child)
        {
            m_children.erase(l_searchIter);
            break;
        }
    }
}
