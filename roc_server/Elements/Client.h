#pragma once

namespace ROC
{

class Client
{
    RakNet::RakNetGUID m_guid;
    int m_id;
public:
    inline int GetID() { return m_id; }
protected:
    Client();
    ~Client();

    inline void SetGUID(RakNet::RakNetGUID &f_guid) { m_guid = f_guid; }
    inline RakNet::RakNetGUID& GetGUID() { return m_guid; }
    inline void SetID(int f_id) { m_id = f_id; }

    friend class ElementManager;
    friend class NetworkManager;
};

}
