#include "stdafx.h"

#include "Elements/Client.h"

ROC::Client::Client(const RakNet::SystemAddress &f_address) : m_address(f_address)
{
    m_elementType = ET_Client;
    m_elementTypeName.assign("Client");
}
ROC::Client::~Client()
{
}

void ROC::Client::GetAddress(std::string &f_ip, unsigned short &f_port)
{
    f_ip.assign(m_address.ToString(false));
    f_port = m_address.GetPort();
}
