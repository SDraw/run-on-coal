#include "stdafx.h"

#include "Utils/CustomArguments.h"

ROC::CustomArguments::CustomArguments()
{
    m_argumentsCount = 0U;
}

ROC::CustomArguments::~CustomArguments()
{
    m_arguments.clear();
}

void ROC::CustomArguments::Clear()
{
    m_arguments.clear();
    m_argumentsCount = 0U;
}

size_t ROC::CustomArguments::GetArgumentsCount() const
{
    return m_arguments.size();
}

const ROC::ICustomArgument* ROC::CustomArguments::GetArgument(size_t f_index) const
{
    return ((f_index < m_argumentsCount) ? &m_arguments[f_index] : nullptr);
}

const std::vector<ROC::CustomArgument>& ROC::CustomArguments::GetArguments() const
{
    return m_arguments;
}
