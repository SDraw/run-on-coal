#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/EventManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Lua/LuaArguments.h"

#define ROC_DEFAULT_SCRIPTS_PATH "server_scripts/"

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
    LuaManager::SetCore(this);

    m_networkManager = new NetworkManager(this);
    m_argument = new LuaArguments();
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
    delete m_argument;
}

ROC::Core* ROC::Core::Init()
{
    if(!ms_instance)
    {
        ms_instance = new Core();

        // Load default scripts
        std::string l_metaPath(ROC_DEFAULT_SCRIPTS_PATH);
        l_metaPath.append("meta.xml");
        pugi::xml_document *l_meta = new pugi::xml_document();
        if(l_meta->load_file(l_metaPath.c_str()))
        {
            pugi::xml_node l_metaRoot = l_meta->child("meta");
            if(l_metaRoot)
            {
                for(pugi::xml_node l_node = l_metaRoot.child("script"); l_node; l_node = l_node.next_sibling("script"))
                {
                    pugi::xml_attribute l_attrib = l_node.attribute("src");
                    if(l_attrib)
                    {
                        std::string l_path(ROC_DEFAULT_SCRIPTS_PATH);
                        l_path.append(l_attrib.as_string());
                        ms_instance->m_luaManager->LoadScript(l_path);
                    }
                }
            }
        }
        delete l_meta;

        if(ms_serverStartCallback) (*ms_serverStartCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent("onServerStart", ms_instance->m_argument);
    }
    return ms_instance;
}
void ROC::Core::Terminate()
{
    if(ms_instance)
    {
        if(ms_instance->m_serverStopCallback) (*ms_instance->m_serverStopCallback)();
        ms_instance->m_luaManager->GetEventManager()->CallEvent("onServerStop", ms_instance->m_argument);

        delete ms_instance;
        ms_instance = nullptr;
    }
}

void ROC::Core::DoPulse()
{
    m_networkManager->DoPulse();

    if(m_serverPulseCallback) (*m_serverPulseCallback)();
    m_luaManager->GetEventManager()->CallEvent("onServerPulse", m_argument);

    std::this_thread::sleep_for(m_pulseTick);
}
