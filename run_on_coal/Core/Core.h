#pragma once

namespace ROC
{

class ConfigManager;
class ElementManager;
class SfmlManager;
class InheritanceManager;
class LogManager;
class LuaManager;
class MemoryManager;
class PhysicsManager;
class PreRenderManager;
class RenderManager;
class SoundManager;
class Core
{
    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    SfmlManager *m_sfmlManager;
    InheritanceManager *m_inheritManager;
    LogManager *m_logManager;
    LuaManager *m_luaManager;
    MemoryManager *m_memoryManager;
    PhysicsManager *m_physicsManager;
    RenderManager *m_renderManager;
    PreRenderManager *m_preRenderManager;
    SoundManager *m_soundManager;
    static Core *m_instance;
    std::string m_workingDir;
    Core();
    ~Core();
public:
    static Core* GetCore();
    static void Terminate();

    bool DoPulse();
    void GetWorkingDirectory(std::string &f_path);

    ConfigManager* GetConfigManager();
    ElementManager* GetElementManager();
    SfmlManager* GetSfmlManager();
    InheritanceManager* GetInheritManager();
    LogManager* GetLogManager();
    LuaManager* GetLuaManager();
    MemoryManager* GetMemoryManager();
    PhysicsManager* GetPhysicsManager();
    RenderManager* GetRenderManager();
    PreRenderManager* GetPreRenderManager();
    SoundManager* GetSoundManager();
};

}
