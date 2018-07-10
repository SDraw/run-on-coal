#pragma once

namespace ROC
{

class Core;
class LuaArguments;
typedef void(*OnNetworkStateChangeCallback)(const std::string&);
typedef void(*OnNetworkDataRecieveCallback)(const std::string&);

class NetworkManager final
{
    Core *m_core;

    RakNet::RakPeerInterface *m_networkInterface;
    RakNet::SocketDescriptor m_socketDescriptor;
    RakNet::SystemAddress m_serverAddress;
    enum NetworkIdentifier : unsigned char { ID_ROC_DATA_PACKET = ID_USER_PACKET_ENUM + 1 };

    enum NetworkState : unsigned char 
    { 
        NS_Disconnected = 0U, 
        NS_Connecting, 
        NS_Connected, 
        NS_Disconnecting 
    };
    NetworkState m_networkState;

    LuaArguments *m_luaArguments;
    OnNetworkStateChangeCallback m_stateCallback;
    OnNetworkDataRecieveCallback m_dataCallback;

    static unsigned char GetPacketIdentifier(RakNet::Packet *f_packet);

    NetworkManager(const NetworkManager& that);
    NetworkManager &operator =(const NetworkManager &that);
public:
    bool Connect(const std::string &f_ip, unsigned short f_port);
    bool Disconnect();
    bool SendData(const std::string &f_data);

    inline unsigned char GetNetworkState() const { return m_networkState; }
    int GetPing();

    inline void SetStateCallback(OnNetworkStateChangeCallback f_callback) { m_stateCallback = f_callback; }
    inline void SetDataCallback(OnNetworkDataRecieveCallback f_callback) { m_dataCallback = f_callback; }
protected:
    explicit NetworkManager(Core *f_core);
    ~NetworkManager();

    void DoPulse();

    friend class Core;
};

}
