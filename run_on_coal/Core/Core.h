#pragma once

namespace ROC
{

class ConfigManager;
class ElementManager;
class SfmlManager;
class AsyncManager;
class InheritanceManager;
class LogManager;
class LuaManager;
class NetworkManager;
class PhysicsManager;
class PreRenderManager;
class RenderManager;
class SoundManager;
class VRManager;
class LuaArguments;

typedef void(*OnEngineStartCallback)(void);
typedef void(*OnEngineStopCallback)(void);

class Core final
{
    static Core *ms_instance;

    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    SfmlManager *m_sfmlManager;
    AsyncManager *m_asyncManager;
    InheritanceManager *m_inheritManager;
    LogManager *m_logManager;
    LuaManager *m_luaManager;
    NetworkManager *m_networkManager;
    PhysicsManager *m_physicsManager;
    RenderManager *m_renderManager;
    PreRenderManager *m_preRenderManager;
    SoundManager *m_soundManager;
    VRManager *m_vrManager;

    std::string m_workingDir;
    bool m_state;
    LuaArguments *m_luaArguments;

    static OnEngineStartCallback ms_engineStartCallback;
    OnEngineStopCallback m_engineStopCallback;

    Core();
    Core(const Core &that);
    Core& operator=(const Core &that);
    ~Core();
public:
    static Core* Init();
    inline static Core* GetCore() { return ms_instance; }
    static void Terminate();

    static inline void SetEngineStartCallback(OnEngineStartCallback f_callback) { ms_engineStartCallback = f_callback; }
    inline void SetEngineStopCallback(OnEngineStopCallback f_callback) { m_engineStopCallback = f_callback; }

    inline const std::string& GetWorkingDirectory() const { return m_workingDir; }

    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    inline ElementManager* GetElementManager() const { return m_elementManager; }
    inline SfmlManager* GetSfmlManager() const { return m_sfmlManager; }
    inline AsyncManager* GetAsyncManager() const { return m_asyncManager; }
    inline InheritanceManager* GetInheritManager() { return m_inheritManager; }
    inline LogManager* GetLogManager() const { return m_logManager; }
    inline LuaManager* GetLuaManager() const { return m_luaManager; }
    inline NetworkManager* GetNetworkManager() const { return m_networkManager; }
    inline PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
    inline RenderManager* GetRenderManager() const { return m_renderManager; }
    inline PreRenderManager* GetPreRenderManager() const { return m_preRenderManager; }
    inline SoundManager* GetSoundManager() const { return m_soundManager; }
    inline VRManager* GetVRManager() const { return m_vrManager; }

    bool DoPulse();
};

}
