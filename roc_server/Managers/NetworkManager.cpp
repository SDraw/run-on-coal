#include "stdafx.h"

#include "Managers/NetworkManager.h"
#include "Core/Core.h"
#include "Elements/Client.h"
#include "Lua/LuaArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/LuaManager/EventManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager/LuaManager.h"

#define ROC_NETWORK_MAX_CONNECTIONS 8
#define ROC_NETWORK_DISCONNECT_DURATION 300U

ROC::NetworkManager::NetworkManager(Core *f_core)
{
    m_core = f_core;

    m_networkInterface = RakNet::RakPeerInterface::GetInstance();
    std::string l_string;
    ConfigManager *l_configManager = m_core->GetConfigManager();
    l_configManager->GetBindIP(l_string);
    RakNet::SocketDescriptor l_socketDescriptor(l_configManager->GetBindPort(), l_string.c_str());
    if(m_networkInterface->Startup(l_configManager->GetMaxClients(), &l_socketDescriptor, 1) == RakNet::StartupResult::RAKNET_STARTED)
    {
        std::string l_log("Network interface started at ");
        l_log.append(l_socketDescriptor.hostAddress);
        l_log.push_back(':');
        l_log.append(std::to_string(l_socketDescriptor.port));
        m_core->GetLogManager()->Log(l_log);
        l_log.clear();
        l_log.append("Maximum clients - ");
        l_log.append(std::to_string(l_configManager->GetMaxClients()));
        m_core->GetLogManager()->Log(l_log);

        m_networkInterface->SetMaximumIncomingConnections(l_configManager->GetMaxClients());
        m_networkInterface->SetOccasionalPing(true);
    }
    else
    {
        m_core->GetLogManager()->Log("Unable to start network interface. Check IP and port in configuration file");
        RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
        m_networkInterface = nullptr;
    }

    m_clientVector.assign(l_configManager->GetMaxClients(), nullptr);
    m_luaArguments = new LuaArguments();

    m_networkClientConnectCallback = nullptr;
    m_networkClientDisconnectCallback = nullptr;
    m_networkDataRecieveCallback = nullptr;
}
ROC::NetworkManager::~NetworkManager()
{
    if(m_networkInterface)
    {
        m_networkInterface->Shutdown(ROC_NETWORK_DISCONNECT_DURATION);
        RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
    }
    delete m_luaArguments;
}

unsigned char ROC::NetworkManager::GetPacketIdentifier(RakNet::Packet *f_packet)
{
    unsigned char l_result = 255U;
    if(f_packet)
    {
        if(f_packet->data[0] == ID_TIMESTAMP)
        {
            RakAssert(f_packet->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
            l_result = f_packet->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
        }
        else l_result = f_packet->data[0];
    }
    return l_result;
}

bool ROC::NetworkManager::Disconnect(Client *f_client)
{
    if(m_networkInterface) m_networkInterface->CloseConnection(f_client->GetAddress(), true);
    return (m_networkInterface != nullptr);
}
bool ROC::NetworkManager::SendData(Client *f_client, const std::string &f_data)
{
    if(m_networkInterface)
    {
        RakNet::BitStream l_sendData;
        unsigned int l_dataSize = static_cast<unsigned int>(f_data.size());
        l_sendData.Write(static_cast<unsigned char>(ID_ROC_DATA_PACKET));
        l_sendData.Write(l_dataSize);
        l_sendData.Write(f_data.data(), l_dataSize);
        m_networkInterface->Send(&l_sendData, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, f_client->GetAddress(), false);
    }
    return (m_networkInterface != nullptr);
}
int ROC::NetworkManager::GetPing(Client *f_client) const
{
    return (m_networkInterface->GetLastPing(f_client->GetAddress()));
}

void ROC::NetworkManager::DoPulse()
{
    if(m_networkInterface)
    {
        EventManager *l_eventManager = m_core->GetLuaManager()->GetEventManager();
        for(RakNet::Packet *l_packet = m_networkInterface->Receive(); l_packet; m_networkInterface->DeallocatePacket(l_packet), l_packet = m_networkInterface->Receive())
        {
            switch(GetPacketIdentifier(l_packet))
            {
                case ID_NEW_INCOMING_CONNECTION:
                {
                    std::string l_log("New client connected (");
                    l_log.append(l_packet->systemAddress.ToString(true, ':'));
                    l_log.append(") with ID ");
                    l_log.append(std::to_string(l_packet->guid.systemIndex));

                    Client *l_client = m_core->GetElementManager()->CreateClient(l_packet->systemAddress);
                    m_clientVector[l_packet->guid.systemIndex] = l_client;

                    if(m_networkClientConnectCallback) (*m_networkClientConnectCallback)(l_client);

                    m_luaArguments->PushArgument(l_client);
                    l_eventManager->CallEvent(EventManager::EME_onNetworkClientConnect, m_luaArguments);
                    m_luaArguments->Clear();
                    m_core->GetLogManager()->Log(l_log);
                } break;
                case ID_DISCONNECTION_NOTIFICATION: case ID_CONNECTION_LOST:
                {
                    std::string l_log("Client (");
                    l_log.append(l_packet->systemAddress.ToString(true, ':'));
                    l_log.append(") with ID ");
                    l_log.append(std::to_string(l_packet->guid.systemIndex));
                    l_log.append(" disconnected");

                    Client *l_client = m_clientVector[l_packet->guid.systemIndex];

                    if(m_networkClientDisconnectCallback) (*m_networkClientDisconnectCallback)(l_client);

                    m_luaArguments->PushArgument(l_client);
                    l_eventManager->CallEvent(EventManager::EME_onNetworkClientDisconnect, m_luaArguments);
                    m_luaArguments->Clear();

                    m_core->GetElementManager()->DestroyClient(l_client);
                    m_clientVector[l_packet->guid.systemIndex] = nullptr;
                    m_core->GetLogManager()->Log(l_log);
                } break;
                case ID_ROC_DATA_PACKET:
                {
                    RakNet::BitStream l_dataIn(l_packet->data, l_packet->length, false);
                    unsigned int l_textSize;
                    std::string l_stringData;
                    l_dataIn.IgnoreBytes(sizeof(unsigned char));
                    if(l_dataIn.Read(l_textSize))
                    {
                        l_stringData.resize(static_cast<size_t>(l_textSize));
                        if(l_dataIn.Read(&l_stringData[0], l_textSize))
                        {
                            Client *l_client = m_clientVector[l_packet->guid.systemIndex];

                            if(m_networkDataRecieveCallback) (*m_networkDataRecieveCallback)(l_client, l_stringData);

                            m_luaArguments->PushArgument(l_client);
                            m_luaArguments->PushArgument(l_stringData);
                            l_eventManager->CallEvent(EventManager::EME_onNetworkDataRecieve, m_luaArguments);
                            m_luaArguments->Clear();
                        }
                    }
                } break;
            }
        }
    }
}
