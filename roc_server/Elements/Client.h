#pragma once

namespace ROC
{

class Client
{
    RakNet::SystemAddress m_address;
public:
    inline RakNet::SystemIndex GetID() { return m_address.systemIndex; }
protected:
    Client();
    ~Client();

    inline void SetAddress(RakNet::SystemAddress &f_id) { m_address = f_id; }
    inline RakNet::SystemAddress& GetAddress() { return m_address; }

    friend class ElementManager;
    friend class NetworkManager;
};

}
