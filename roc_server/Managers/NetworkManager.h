#pragma once
#include "Interfaces/INetworkManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class IClient; class Client;

class NetworkManager final : public INetworkManager
{
    Core *m_core;

    RakNet::RakPeerInterface *m_networkInterface;
    enum NetworkIdentifier : unsigned char { ID_ROC_DATA_PACKET = ID_USER_PACKET_ENUM + 1 };

    std::vector<Client*> m_clients;

    CustomArguments m_arguments;

    static unsigned char GetPacketIdentifier(RakNet::Packet *f_packet);

    NetworkManager(const NetworkManager &that) = delete;
    NetworkManager& operator=(const NetworkManager &that) = delete;

    // Interfaces reroute
    bool Disconnect(IClient *f_client);
    bool SendData(IClient *f_client, const std::string &f_data);
    int GetPing(IClient *f_client) const;
public:
    bool Disconnect(Client *f_client);
    bool SendData(Client *f_client, const std::string &f_data);
    int GetPing(Client *f_client) const;
protected:
    explicit NetworkManager(Core *f_core);
    ~NetworkManager();

    void DoPulse();
    friend class Core;
};

}
