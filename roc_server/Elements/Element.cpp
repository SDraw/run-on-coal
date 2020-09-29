#include "stdafx.h"

#include "Elements/Element.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

const std::string g_elementTypeNames[]
{
    "Client",
    "File"
};
const std::string g_ElementInvalidName = "Invalid";

}

ROC::Element::Element()
{
    m_elementType = ET_Invalid;
}

ROC::Element::~Element()
{
}

unsigned char ROC::Element::GetElementType() const
{
    return m_elementType;
}

const std::string& ROC::Element::GetElementTypeName() const
{
    return ((m_elementType != ET_Invalid) ? g_elementTypeNames[m_elementType] : g_ElementInvalidName);
}
