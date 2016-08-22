#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"

ROC::Core* ROC::Core::m_instance = NULL;
ROC::Core* ROC::Core::GetCore()
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

ROC::Core::Core()
{
    char *l_pathBuf = new char[FILENAME_MAX];
    _getcwd(l_pathBuf,FILENAME_MAX);

    std::string l_appPath(l_pathBuf);
    delete[]l_pathBuf;
    std::regex l_regex("\\\\");
    std::regex_replace(std::back_inserter(m_workingDir),l_appPath.begin(),l_appPath.end(),l_regex,"/");

    m_configManager = new ConfigManager(this);
    m_logManager = new LogManager(this);
    m_sfmlManager = new SfmlManager(this);
    m_luaManager = new LuaManager(this);
    m_memoryManager = new MemoryManager(this);
    m_inheritManager = new InheritanceManager(this);
    m_elementManager = new ElementManager(this);
    m_soundManager = new SoundManager(this);
    m_physicsManager = new PhysicsManager(this);
    m_preRenderManager = new PreRenderManager(this);
    m_renderManager = new RenderManager(this);
}
ROC::Core::~Core()
{
    delete m_renderManager;
    delete m_preRenderManager;
    delete m_physicsManager;
    delete m_soundManager;
    delete m_elementManager;
    delete m_inheritManager;
    delete m_memoryManager;
    delete m_luaManager;
    delete m_sfmlManager;
    delete m_logManager;
    delete m_configManager;
}

bool ROC::Core::DoPulse()
{
    bool l_result = m_sfmlManager->DoPulse();
    m_preRenderManager->DoPulse_S1();
    m_physicsManager->DoPulse();
    m_preRenderManager->DoPulse_S2();
    m_soundManager->DoPulse();
    m_elementManager->SetLock(true);
    m_renderManager->DoPulse();
    m_elementManager->SetLock(false);
    return l_result;
}
