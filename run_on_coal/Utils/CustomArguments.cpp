#include "stdafx.h"

#include "Utils/CustomArguments.h"

ROC::CustomArguments::CustomArguments()
{
}
ROC::CustomArguments::~CustomArguments()
{
    m_arguments.clear();
}

void ROC::CustomArguments::Clear()
{
    m_arguments.clear();
}
