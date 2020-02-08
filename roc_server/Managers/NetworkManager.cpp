#include "stdafx.h"

#include "Managers/NetworkManager.h"
#include "Core/Core.h"
#include "Elements/Client.h"
#include "Utils/CustomArguments.h"

#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/ModuleManager.h"
#include "Interfaces/IModule.h"

namespace ROC
{

const size_t g_NetworkDisconnectDuration = 300U;

}

ROC::NetworkManager::NetworkManager(Core *f_core)
{
    m_core = f_core;

    m_networkInterface = RakNet::RakPeerInterface::GetInstance();
    std::string l_string;
    ConfigManager *l_configManager = m_core->GetConfigManager();
    l_configManager->GetBindIP(l_string);
    RakNet::SocketDescriptor l_socketDescriptor(l_configManager->GetBindPort(), l_string.c_str());
    RakNet::StartupResult l_startupResult = m_networkInterface->Startup(l_configManager->GetMaxClients(), &l_socketDescriptor, 1);
    if(l_startupResult == RakNet::RAKNET_STARTED)
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
        std::string l_log("Unable to start network interface. Reason: ");
        switch(l_startupResult)
        {
            case RakNet::RAKNET_ALREADY_STARTED:
                l_log.append("Already started");
                break;
            case RakNet::INVALID_SOCKET_DESCRIPTORS:
                l_log.append("Invaliid socket descriptors");
                break;
            case RakNet::INVALID_MAX_CONNECTIONS:
                l_log.append("Invalid maximal connections");
                break;
            case RakNet::SOCKET_FAMILY_NOT_SUPPORTED:
                l_log.append("Socket family isn't supported");
                break;
            case RakNet::SOCKET_PORT_ALREADY_IN_USE:
                l_log.append("Socket port is already in use");
                break;
            case RakNet::SOCKET_FAILED_TO_BIND:
                l_log.append("Socket failed to bind");
                break;
            case RakNet::SOCKET_FAILED_TEST_SEND:
                l_log.append("Socket failed to perform test send");
                break;
            case RakNet::PORT_CANNOT_BE_ZERO:
                l_log.append("Port is equal zero");
                break;
            case RakNet::FAILED_TO_CREATE_NETWORK_THREAD:
                l_log.append("Failed to create network thread");
                break;
            case RakNet::COULD_NOT_GENERATE_GUID:
                l_log.append("Failed to generate guid");
                break;
            case RakNet::STARTUP_OTHER_FAILURE: default:
                l_log.append("Unknown reason");
                break;
        }
        m_core->GetLogManager()->Log(l_log);
        RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
        m_networkInterface = nullptr;
    }

    m_clients.assign(l_configManager->GetMaxClients(), nullptr);

    m_arguments = new CustomArguments();
}
ROC::NetworkManager::~NetworkManager()
{
    if(m_networkInterface)
    {
        m_networkInterface->Shutdown(g_NetworkDisconnectDuration);
        RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
    }
    delete m_arguments;
}

unsigned char ROC::NetworkManager::GetPacketIdentifier(RakNet::Packet *f_packet)
{
    unsigned char l_result = 255U;
    if(f_packet)
    {
        if(f_packet->data[0] == ID_TIMESTAMP)
        {
            RakAssert(f_packet->length > (sizeof(RakNet::MessageID) + sizeof(RakNet::Time)));
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
        for(RakNet::Packet *l_packet = m_networkInterface->Receive(); l_packet; m_networkInterface->DeallocatePacket(l_packet), l_packet = m_networkInterface->Receive())
        {
            switch(GetPacketIdentifier(l_packet))
            {
                case ID_NEW_INCOMING_CONNECTION:
                {
                    std::string l_log("Client connected (");
                    l_log.append(l_packet->systemAddress.ToString(true, ':'));
                    l_log.push_back(':');
                    l_log.append(std::to_string(l_packet->guid.systemIndex));

                    Client *l_client = m_core->GetElementManager()->CreateClient(l_packet->systemAddress);
                    m_clients[l_packet->guid.systemIndex] = l_client;

                    m_arguments->Push(l_client);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnNetworkClientConnect, m_arguments);
                    m_arguments->Clear();
                    m_core->GetLogManager()->Log(l_log);
                } break;
                case ID_DISCONNECTION_NOTIFICATION: case ID_CONNECTION_LOST:
                {
                    std::string l_log("Client (");
                    l_log.append(l_packet->systemAddress.ToString(true, ':'));
                    l_log.push_back(':');
                    l_log.append(std::to_string(l_packet->guid.systemIndex));
                    l_log.append(" disconnected");

                    Client *l_client = m_clients[l_packet->guid.systemIndex];

                    m_arguments->Push(l_client);
                    m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnNetworkClientDisconnect, m_arguments);
                    m_arguments->Clear();

                    m_core->GetElementManager()->DestroyClient(l_client);
                    m_clients[l_packet->guid.systemIndex] = nullptr;
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
                            Client *l_client = m_clients[l_packet->guid.systemIndex];

                            m_arguments->Push(l_client);
                            m_arguments->Push(l_stringData);
                            m_core->GetModuleManager()->SignalGlobalEvent(IModule::ME_OnNetworkDataRecieve, m_arguments);
                            m_arguments->Clear();
                        }
                    }
                } break;
            }
        }
    }
}

// ROC::INetworkManager
bool ROC::NetworkManager::Disconnect(IClient *f_client)
{
    return Disconnect(dynamic_cast<Client*>(f_client));
}
bool ROC::NetworkManager::SendData(IClient *f_client, const std::string &f_data)
{
    return SendData(dynamic_cast<Client*>(f_client), f_data);
}
int ROC::NetworkManager::GetPing(IClient *f_client) const
{
    return GetPing(dynamic_cast<Client*>(f_client));
}
