#include "stdafx.h"

#include "Core/Core.h"
#include "Managers/AsyncManager.h"
#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/EventManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"
#include "Lua/LuaArguments.h"

#include "Utils/SystemTick.h"

#define CORE_DEFAULT_SCIPTS_PATH "scripts/"

ROC::Core* ROC::Core::s_instance = nullptr;

ROC::Core::Core()
{
    std::string l_appPath(_MAX_PATH, '\0');
    _getcwd(const_cast<char*>(l_appPath.data()), _MAX_PATH);
    l_appPath.erase(std::remove(l_appPath.begin(), l_appPath.end(), '\0'), l_appPath.end());

    std::regex l_regex("\\\\");
    std::regex_replace(std::back_inserter(m_workingDir), l_appPath.begin(), l_appPath.end(), l_regex, "/");
    m_workingDir.push_back('/');

    m_configManager = new ConfigManager();
    m_logManager = new LogManager(this);
    m_memoryManager = new MemoryManager();
    m_inheritManager = new InheritanceManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager();
    m_physicsManager = new PhysicsManager(this);

    m_luaManager = new LuaManager(this);
    LuaManager::SetCore(this);

    m_sfmlManager = new SfmlManager(this);
    m_asyncManager = new AsyncManager(this);
    m_preRenderManager = new PreRenderManager(this);
    m_renderManager = new RenderManager(this);
    m_networkManager = new NetworkManager(this);

    m_state = true;
    m_argument = new LuaArguments();
}
ROC::Core::~Core()
{
    delete m_networkManager;
    delete m_soundManager;
    delete m_physicsManager;
    delete m_inheritManager;
    delete m_asyncManager;
    delete m_memoryManager;
    delete m_elementManager;
    delete m_luaManager;
    delete m_renderManager;
    delete m_preRenderManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;

    delete m_argument;
}

ROC::Core* ROC::Core::Init()
{
    if(!s_instance)
    {
        s_instance = new Core();
        SystemTick::UpdateTick();

        // Load default scripts
        std::string l_metaPath(CORE_DEFAULT_SCIPTS_PATH);
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
                        std::string l_path(CORE_DEFAULT_SCIPTS_PATH);
                        l_path.append(l_attrib.as_string());
                        s_instance->m_luaManager->OpenFile(l_path);
                    }
                }
            }
        }
        delete l_meta;

        s_instance->m_luaManager->GetEventManager()->CallEvent("onEngineStart", s_instance->m_argument);
    }
    return s_instance;
}
void ROC::Core::Terminate()
{
    if(s_instance)
    {
        s_instance->m_luaManager->GetEventManager()->CallEvent("onEngineStop", s_instance->m_argument);

        delete s_instance;
        s_instance = nullptr;
    }
}

bool ROC::Core::DoPulse()
{
    SystemTick::UpdateTick();
    m_networkManager->DoPulse();
    m_asyncManager->DoPulse();
    m_state = m_sfmlManager->DoPulse();
    m_preRenderManager->DoPulse_S1();
    m_physicsManager->DoPulse();
    m_preRenderManager->DoPulse_S2();
    m_elementManager->SetLock(true);
    m_renderManager->DoPulse();
    m_elementManager->SetLock(false);
    return m_state;
}
