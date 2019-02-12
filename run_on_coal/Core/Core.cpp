#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/AsyncManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/EventManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"
#include "Managers/VRManager.h"
#include "Lua/LuaArguments.h"

#include "Utils/SystemTick.h"

ROC::Core* ROC::Core::ms_instance = nullptr;
ROC::OnEngineStartCallback ROC::Core::ms_engineStartCallback = nullptr;

ROC::Core::Core()
{
    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_inheritManager = new InheritanceManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager();
    m_physicsManager = new PhysicsManager(this);
    m_luaManager = new LuaManager(this);
    m_sfmlManager = new SfmlManager(this);
    m_asyncManager = new AsyncManager(this);
    m_preRenderManager = new PreRenderManager(this);
    m_vrManager = new VRManager(this);
    m_renderManager = new RenderManager(this);
    m_networkManager = new NetworkManager(this);

    m_engineStopCallback = nullptr;

    m_state = true;
    m_luaArguments = new LuaArguments();
}
ROC::Core::~Core()
{
    delete m_networkManager;
    delete m_soundManager;
    delete m_physicsManager;
    delete m_inheritManager;
    delete m_asyncManager;
    delete m_elementManager;
    delete m_luaManager;
    delete m_renderManager;
    delete m_preRenderManager;
    delete m_vrManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;

    delete m_luaArguments;
}

ROC::Core* ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();
        SystemTick::UpdateTick();

        ms_instance->GetLuaManager()->LoadDefaultScripts();

        if(ms_engineStartCallback) (*ms_engineStartCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent(EventManager::EME_onEngineStart, ms_instance->m_luaArguments);
    }
    return ms_instance;
}
void ROC::Core::Terminate()
{
    if(ms_instance)
    {
        if(ms_instance->m_engineStopCallback) (*ms_instance->m_engineStopCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent(EventManager::EME_onEngineStop, ms_instance->m_luaArguments);

        delete ms_instance;
        ms_instance = nullptr;
    }
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
    m_luaManager->DoPulse();
    return m_state;
}
