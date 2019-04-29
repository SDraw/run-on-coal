#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/ModuleManager.h"

#include "Interfaces/IModule.h"

ROC::Core* ROC::Core::ms_instance = nullptr;

ROC::Core::Core()
{
    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_networkManager = new NetworkManager(this);
    m_elementManager = new ElementManager(this);
    m_moduleManager = new ModuleManager(this);
    m_pulseTick = std::chrono::milliseconds(m_configManager->GetPulseTick());
}
ROC::Core::~Core()
{
    delete m_moduleManager;
    delete m_networkManager;
    delete m_elementManager;
    delete m_logManager;
    delete m_configManager;
}

bool ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();

        ms_instance->m_moduleManager->LoadModules();
        ms_instance->m_moduleManager->SignalGlobalEvent(IModule::ME_OnServerStart, ms_instance->m_arguments);
    }
    return (ms_instance != nullptr);
}
bool ROC::Core::Terminate()
{
    if(ms_instance)
    {
        ms_instance->m_logManager->Log("Server is shutting down ...");

        ms_instance->m_moduleManager->SignalGlobalEvent(IModule::ME_OnServerStop, ms_instance->m_arguments);
        ms_instance->m_moduleManager->UnloadModules();

        delete ms_instance;
        ms_instance = nullptr;
    }
    return (ms_instance == nullptr);
}

ROC::ElementManager* ROC::Core::GetElementManager() const
{
    return m_elementManager;
}
ROC::LogManager* ROC::Core::GetLogManager() const
{
    return m_logManager;
}
ROC::NetworkManager* ROC::Core::GetNetworkManager() const
{
    return m_networkManager;
}


void ROC::Core::DoPulse()
{
    m_networkManager->DoPulse();
    m_moduleManager->DoPulse();

    std::this_thread::sleep_for(m_pulseTick);
}
