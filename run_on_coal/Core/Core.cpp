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
    std::string l_appPath(_MAX_PATH, '\0');
    _getcwd(&l_appPath[0], _MAX_PATH);
    l_appPath.erase(std::remove(l_appPath.begin(), l_appPath.end(), '\0'), l_appPath.end());

    std::regex l_regex("\\\\");
    std::regex_replace(std::back_inserter(m_workingDir), l_appPath.begin(), l_appPath.end(), l_regex, "/");
    m_workingDir.push_back('/');

    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_inheritManager = new InheritanceManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager();
    m_physicsManager = new PhysicsManager(this);

    m_luaManager = new LuaManager(this);
    LuaManager::SetCore(this);

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

        // Load default scripts
        std::string l_metaPath(ms_instance->GetConfigManager()->GetScriptsDirectory());
        l_metaPath.append("/meta.xml");
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
                        std::string l_path(ms_instance->GetConfigManager()->GetScriptsDirectory());
                        l_path.push_back('/');
                        l_path.append(l_attrib.as_string());
                        ms_instance->m_luaManager->LoadScript(l_path);
                    }
                }
            }
        }
        else
        {
            std::string l_error("Unable to find '");
            l_error.append(l_metaPath);
            l_error.push_back('\'');
            ms_instance->GetLogManager()->Log(l_error);
        }
        delete l_meta;

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
