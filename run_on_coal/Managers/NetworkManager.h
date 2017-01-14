#pragma once
#define ROC_NETWORK_CONNECTION_TRIES 5
#define ROC_NETWORK_CONNECTION_PER_TRY_TIME 500
#define ROC_NETWORK_MAX_CONNECTIONS 8
#define ROC_NETWORK_DISCONNECT_DURATION 300U

namespace ROC
{

class Core;
class LuaArguments;
class NetworkManager
{
    Core *m_core;

    RakNet::RakPeerInterface *m_networkInterface;
    RakNet::SocketDescriptor m_socketDescriptor;
    enum NetworkIdentifier : unsigned char { ID_ROC_DATA_PACKET = ID_USER_PACKET_ENUM + 1 };

    enum NetworkState : unsigned char { Disconnected = 0U, Connecting, Connected, Disconnecting };
    NetworkState m_networkState;

    LuaArguments *m_argument;
public:
    bool Connect(std::string &f_ip, unsigned short f_port);
    bool Disconnect();
    bool SendData(std::string &f_data);
protected:
    explicit NetworkManager(Core *f_core);
    ~NetworkManager();

    void DoPulse();
    friend Core;
};

}
