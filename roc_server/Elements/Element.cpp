#include "stdafx.h"

#include "Elements/Element.h"
#include "Utils/CustomArguments.h"

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
}

unsigned char ROC::Element::GetElementType() const
{
    return m_elementType;
}

const std::string& ROC::Element::GetElementTypeName() const
{
    return ((m_elementType != ET_Invalid) ? g_ElementTypeName[m_elementType] : g_ElementInvalidName);
}
