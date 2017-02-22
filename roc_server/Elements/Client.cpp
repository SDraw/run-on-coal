#include "stdafx.h"
#include "Elements/Client.h"

ROC::Client::Client()
{
    m_elementType = ElementType::ClientElement;
}
ROC::Client::~Client()
{
}

void ROC::Client::GetAddress(std::string &f_ip, unsigned short &f_port)
{
    f_ip.assign(m_address.ToString(false));
    f_port = m_address.GetPort();
}
