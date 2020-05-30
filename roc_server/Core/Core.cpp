#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/NetworkManager.h"
#include "Utils/CustomArguments.h"

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
    m_arguments = new CustomArguments();
}

ROC::Core::~Core()
{
    delete m_moduleManager;
    delete m_networkManager;
    delete m_elementManager;
    delete m_logManager;
    delete m_configManager;
    delete m_arguments;
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

void ROC::Core::Terminate()
{
    if(ms_instance)
    {
        ms_instance->m_logManager->Log("Server is shutting down ...");

        ms_instance->m_moduleManager->SignalGlobalEvent(IModule::ME_OnServerStop, ms_instance->m_arguments);
        ms_instance->m_moduleManager->UnloadModules();

        delete ms_instance;
        ms_instance = nullptr;
    }
}

void ROC::Core::DoPulse()
{
    m_networkManager->DoPulse();
    m_moduleManager->DoPulse();

    std::this_thread::sleep_for(m_pulseTick);
}

// ROC::ICore
ROC::IElementManager* ROC::Core::GetIElementManager() const
{
    return m_elementManager;
}

ROC::ILogManager* ROC::Core::GetILogManager() const
{
    return m_logManager;
}

ROC::INetworkManager* ROC::Core::GetINetworkManager() const
{
    return m_networkManager;
}
