#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Client final : public Element
{
    RakNet::SystemAddress m_address;

    Client(const Client &that);
    Client& operator=(const Client &that);
public:
    inline RakNet::SystemIndex GetID() const { return m_address.systemIndex; }
    void GetAddress(std::string &f_ip, unsigned short &f_port) const;
protected:
    explicit Client(const RakNet::SystemAddress &f_address);
    ~Client();

    inline RakNet::SystemAddress& GetAddress() { return m_address; }

    friend class ElementManager;
    friend class NetworkManager;
};

}
