#pragma once

namespace ROC
{

class Client
{
    RakNet::SystemAddress m_address;
    int m_id;
public:
    inline int GetID() { return m_id; }
protected:
    Client();
    ~Client();

    inline void SetAddress(RakNet::SystemAddress &f_address) { m_address = f_address; }
    inline RakNet::SystemAddress& GetAddress() { return m_address; }
    inline void SetID(int f_id) { m_id = f_id; }

    friend class ElementManager;
    friend class NetworkManager;
};

}
