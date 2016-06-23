#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/GlfwManager.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager.h"
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
    m_workingDir.append(l_pathBuf);
    delete[]l_pathBuf;

    m_configManager = new ConfigManager(this);
    m_logManager = new LogManager(this);
    m_glfwManager = new GlfwManager(this);
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
    delete m_glfwManager;
    delete m_logManager;
    delete m_configManager;
}

bool ROC::Core::DoPulse()
{
    bool l_result = m_glfwManager->DoPulse();
    m_preRenderManager->DoPulse_S1();
    m_physicsManager->DoPulse();
    m_preRenderManager->DoPulse_S2();
    m_soundManager->DoPulse();
    m_elementManager->SetLock(true);
    m_renderManager->DoPulse();
    m_elementManager->SetLock(false);
    return l_result;
}

ROC::ConfigManager* ROC::Core::GetConfigManager()
{
    return m_configManager;
}
ROC::ElementManager* ROC::Core::GetElementManager()
{
    return m_elementManager;
}
ROC::GlfwManager* ROC::Core::GetGlfwManager()
{
    return m_glfwManager;
}
ROC::InheritanceManager* ROC::Core::GetInheritManager()
{ 
    return m_inheritManager; 
}
ROC::LogManager* ROC::Core::GetLogManager() 
{ 
    return m_logManager; 
}
ROC::LuaManager* ROC::Core::GetLuaManager() 
{ 
    return m_luaManager;
}
ROC::MemoryManager* ROC::Core::GetMemoryManager() 
{ 
    return m_memoryManager; 
}
ROC::PhysicsManager* ROC::Core::GetPhysicsManager()
{
    return m_physicsManager;
}
ROC::RenderManager* ROC::Core::GetRenderManager()
{ 
    return m_renderManager; 
}
ROC::PreRenderManager* ROC::Core::GetPreRenderManager()
{
    return m_preRenderManager;
}
ROC::SoundManager* ROC::Core::GetSoundManager()
{ 
    return m_soundManager; 
}

void ROC::Core::GetWorkingDirectory(std::string &f_path)
{
    f_path.append(m_workingDir);
}
