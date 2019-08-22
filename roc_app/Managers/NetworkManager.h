#pragma once
#include "Interfaces/INetworkManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;

class NetworkManager final : public INetworkManager
{
    Core *m_core;

    RakNet::RakPeerInterface *m_networkInterface;
    RakNet::SocketDescriptor m_socketDescriptor;
    RakNet::SystemAddress m_serverAddress;
    enum NetworkIdentifier : unsigned char
    {
        ID_ROC_DATA_PACKET = ID_USER_PACKET_ENUM + 1
    };

    enum NetworkState : unsigned char 
    { 
        NS_Disconnected = 0U, 
        NS_Connecting, 
        NS_Connected, 
        NS_Disconnecting 
    };
    NetworkState m_networkState;

    CustomArguments m_arguments;

    static unsigned char GetPacketIdentifier(RakNet::Packet *f_packet);

    NetworkManager(const NetworkManager &that) = delete;
    NetworkManager& operator=(const NetworkManager &that) = delete;
public:
    bool Connect(const std::string &f_ip, unsigned short f_port);
    bool Disconnect();
    bool SendData(const std::string &f_data);

    inline unsigned char GetNetworkState() const { return m_networkState; }
    int GetPing() const;
protected:
    explicit NetworkManager(Core *f_core);
    ~NetworkManager();

    void DoPulse();

    friend class Core;
};

}
