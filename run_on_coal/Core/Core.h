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
class NetworkManager;
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
    NetworkManager *m_networkManager;
    PhysicsManager *m_physicsManager;
    RenderManager *m_renderManager;
    PreRenderManager *m_preRenderManager;
    SoundManager *m_soundManager;
    static Core *m_instance;
    std::string m_workingDir;
    bool m_state;

    Core();
    Core(const Core& that);
    Core &operator =(const Core &that);
    ~Core();
public:
    static Core* Init();
    static void Terminate();

    bool DoPulse();
    inline void GetWorkingDirectory(std::string &f_path) { f_path.assign(m_workingDir); }

    inline ConfigManager* GetConfigManager() { return m_configManager; }
    inline ElementManager* GetElementManager() { return m_elementManager; }
    inline SfmlManager* GetSfmlManager() { return m_sfmlManager; }
    inline InheritanceManager* GetInheritManager() { return m_inheritManager; }
    inline LogManager* GetLogManager() { return m_logManager; }
    inline LuaManager* GetLuaManager() { return m_luaManager; }
    inline MemoryManager* GetMemoryManager() { return m_memoryManager; }
    inline NetworkManager* GetNetworkManager() { return m_networkManager; }
    inline PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
    inline RenderManager* GetRenderManager() { return m_renderManager; }
    inline PreRenderManager* GetPreRenderManager() { return m_preRenderManager; }
    inline SoundManager* GetSoundManager() { return m_soundManager; }
};

}
