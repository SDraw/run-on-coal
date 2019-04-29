#pragma once
#include "Interfaces/IClient.h"
#include "Elements/Element.h"

namespace ROC
{

class Client final : public Element, public virtual IClient
{
    RakNet::SystemAddress m_address;

    Client(const Client &that) = delete;
    Client& operator=(const Client &that) = delete;
public:
    unsigned int GetID() const;
    void GetAddress(std::string &f_ip, unsigned short &f_port) const;
protected:
    explicit Client(const RakNet::SystemAddress &f_address);
    ~Client();

    inline RakNet::SystemAddress& GetAddress() { return m_address; }

    friend class ElementManager;
    friend class NetworkManager;
};

}
