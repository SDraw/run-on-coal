#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/LuaManager/EventManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Managers/NetworkManager.h"

ROC::Core* ROC::Core::ms_instance = nullptr;
ROC::OnServerStartCallback ROC::Core::ms_serverStartCallback = nullptr;

ROC::Core::Core()
{
    std::string l_appPath(_MAX_PATH, '\0');
    _getcwd(&l_appPath[0], _MAX_PATH);
    l_appPath.erase(std::remove(l_appPath.begin(), l_appPath.end(), '\0'), l_appPath.end());

    std::regex l_regex("\\\\");
    std::regex_replace(std::back_inserter(m_workingDir), l_appPath.begin(), l_appPath.end(), l_regex, "/");
    m_workingDir.push_back('/');

    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_elementManager = new ElementManager(this);
    m_luaManager = new LuaManager(this);
    m_networkManager = new NetworkManager(this);
    m_pulseTick = std::chrono::milliseconds(m_configManager->GetPulseTick());

    m_serverPulseCallback = nullptr;
    m_serverStopCallback = nullptr;
}
ROC::Core::~Core()
{
    delete m_networkManager;
    delete m_elementManager;
    delete m_luaManager;
    delete m_logManager;
    delete m_configManager;
}

ROC::Core* ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();

        ms_instance->GetLuaManager()->LoadDefaultScripts();

        if(ms_serverStartCallback) (*ms_serverStartCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent(EventManager::EME_onServerStart, ms_instance->m_luaArguments);
    }
    return ms_instance;
}
void ROC::Core::Terminate()
{
    if(ms_instance)
    {
        if(ms_instance->m_serverStopCallback) (*ms_instance->m_serverStopCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent(EventManager::EME_onServerStop, ms_instance->m_luaArguments);

        delete ms_instance;
        ms_instance = nullptr;
    }
}

void ROC::Core::DoPulse()
{
    m_networkManager->DoPulse();

    if(m_serverPulseCallback) (*m_serverPulseCallback)();
    m_luaManager->GetEventManager()->CallEvent(EventManager::EME_onServerPulse, m_luaArguments);

    m_luaManager->DoPulse();

    std::this_thread::sleep_for(m_pulseTick);
}
