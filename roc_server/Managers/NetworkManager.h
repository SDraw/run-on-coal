#pragma once
#define ROC_NETWORK_MAX_CONNECTIONS 8
#define ROC_NETWORK_DISCONNECT_DURATION 300U

namespace ROC
{

class Core;
class Client;
class LuaArguments;
typedef void(*OnNetworkClientConnectCallback)(Client*);
typedef void(*OnNetworkClientDisconnectCallback)(Client*);
typedef void(*OnNetworkDataRecieveCallback)(Client*, const std::string&);

class NetworkManager final
{
    Core *m_core;

    RakNet::RakPeerInterface *m_networkInterface;
    enum NetworkIdentifier : unsigned char { ID_ROC_DATA_PACKET = ID_USER_PACKET_ENUM + 1 };

    std::vector<Client*> m_clientVector;

    LuaArguments *m_argument;

    OnNetworkClientConnectCallback m_networkClientConnectCallback;
    OnNetworkClientDisconnectCallback m_networkClientDisconnectCallback;
    OnNetworkDataRecieveCallback m_networkDataRecieveCallback;

    NetworkManager(const NetworkManager& that);
    NetworkManager &operator =(const NetworkManager &that);
public:
    bool Disconnect(Client *f_client);
    bool SendData(Client *f_client, const std::string &f_data);
    int GetPing(Client *f_client);

    inline void SetNetworkClientConnectCallback(OnNetworkClientConnectCallback f_callback) { m_networkClientConnectCallback = f_callback; }
    inline void SetNetworkClientDisconnectCallback(OnNetworkClientDisconnectCallback f_callback) { m_networkClientDisconnectCallback = f_callback; }
    inline void SetNetworkDataRecieveCallback(OnNetworkDataRecieveCallback f_callback) { m_networkDataRecieveCallback = f_callback; }
protected:
    explicit NetworkManager(Core *f_core);
    ~NetworkManager();

    void DoPulse();
    friend Core;
};

}
