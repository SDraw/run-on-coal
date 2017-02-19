#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/LuaArguments.h"

ROC::Core* ROC::Core::m_instance = NULL;

ROC::Core::Core()
{
    char *l_pathBuf = new char[_MAX_PATH];
    std::string l_appPath;
    std::regex l_regex("\\\\");
    _getcwd(l_pathBuf, _MAX_PATH);
    l_appPath.append(l_pathBuf);
    std::regex_replace(std::back_inserter(m_workingDir), l_appPath.begin(), l_appPath.end(), l_regex, "/");
    delete[]l_pathBuf;

    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_memoryManager = new MemoryManager();
    m_elementManager = new ElementManager(this);
    m_luaManager = new LuaManager(this);
    m_sfmlManager = new SfmlManager();
    m_networkManager = new NetworkManager(this);
    m_argument = new LuaArguments();
    m_pulseTick = std::chrono::milliseconds(m_configManager->GetPulseTick());
}
ROC::Core::~Core()
{
    delete m_networkManager;
    delete m_memoryManager;
    delete m_elementManager;
    delete m_luaManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;
    delete m_argument;
}

ROC::Core* ROC::Core::Init()
{
    if(!m_instance) m_instance = new Core();
    return m_instance;
}
void ROC::Core::Terminate()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

void ROC::Core::DoPulse()
{
    m_sfmlManager->DoPulse();
    m_networkManager->DoPulse();
    m_luaManager->GetEventManager()->CallEvent(EventType::ServerPulse, m_argument);
    std::this_thread::sleep_for(m_pulseTick);
}
