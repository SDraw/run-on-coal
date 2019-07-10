#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/ModuleManager.h"
#include "Managers/PreRenderManager.h"

#include "Interfaces/IModule.h"
#include "Utils/SystemTick.h"

ROC::Core *ROC::Core::ms_instance = nullptr;

ROC::Core::Core()
{
    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager();
    m_physicsManager = new PhysicsManager(this);
    m_sfmlManager = new SfmlManager(this);
    m_asyncManager = new AsyncManager(this);
    m_preRenderManager = new PreRenderManager(this);
    m_vrManager = new VRManager(this);
    m_renderManager = new RenderManager(this);
    m_networkManager = new NetworkManager(this);
    m_moduleManager = new ModuleManager(this);

    m_state = true;
}
ROC::Core::~Core()
{
    delete m_networkManager;
    delete m_soundManager;
    delete m_physicsManager;
    delete m_asyncManager;
    delete m_elementManager;
    delete m_renderManager;
    delete m_preRenderManager;
    delete m_vrManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;
}

bool ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();
        SystemTick::Init();

        ms_instance->m_moduleManager->LoadModules();
        ms_instance->m_moduleManager->SignalGlobalEvent(IModule::ME_OnEngineStart, ms_instance->m_arguments);
    }
    return (ms_instance != nullptr);
}
bool ROC::Core::Terminate()
{
    if(ms_instance)
    {
        ms_instance->m_moduleManager->SignalGlobalEvent(IModule::ME_OnEngineStop, ms_instance->m_arguments);
        ms_instance->m_moduleManager->UnloadModules();

        delete ms_instance;
        ms_instance = nullptr;
    }
    return (ms_instance == nullptr);
}
ROC::Core* ROC::Core::GetCore()
{
    return ms_instance;
}

// Interfaces
ROC::ElementManager* ROC::Core::GetElementManager() const
{
    return m_elementManager;
}
ROC::SfmlManager* ROC::Core::GetSfmlManager() const
{
    return m_sfmlManager;
}
ROC::AsyncManager* ROC::Core::GetAsyncManager() const
{
    return m_asyncManager;
}
ROC::LogManager* ROC::Core::GetLogManager() const
{
    return m_logManager;
}
ROC::NetworkManager* ROC::Core::GetNetworkManager() const
{
    return m_networkManager;
}
ROC::PhysicsManager* ROC::Core::GetPhysicsManager() const
{
    return m_physicsManager;
}
ROC::RenderManager* ROC::Core::GetRenderManager() const
{
    return m_renderManager;
}
ROC::SoundManager* ROC::Core::GetSoundManager() const
{
    return m_soundManager;
}
ROC::VRManager* ROC::Core::GetVRManager() const
{
    return m_vrManager;
}

bool ROC::Core::DoPulse()
{
    SystemTick::UpdateTick();
    m_networkManager->DoPulse();
    m_asyncManager->DoPulse();
    m_state = (m_state && m_vrManager->DoPulse());
    m_state = (m_state && m_sfmlManager->DoPulse());
    m_preRenderManager->DoPulse_S1();
    m_physicsManager->DoPulse();
    m_preRenderManager->DoPulse_S2();
    m_renderManager->DoPulse();
    m_moduleManager->DoPulse();
    return m_state;
}
