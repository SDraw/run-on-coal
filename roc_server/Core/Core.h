#pragma once

namespace ROC
{

class ConfigManager;
class ElementManager;
class LogManager;
class LuaManager;
class NetworkManager;
class LuaArguments;

typedef void(*OnServerStartCallback)(void);
typedef void(*OnServerPulseCallback)(void);
typedef void(*OnServerStopCallback)(void);

class Core final
{
    static Core *ms_instance;

    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    LogManager *m_logManager;
    LuaManager *m_luaManager;
    NetworkManager *m_networkManager;

    std::string m_workingDir;
    std::chrono::milliseconds m_pulseTick;
    LuaArguments *m_luaArguments;

    static OnServerStartCallback ms_serverStartCallback;
    OnServerPulseCallback m_serverPulseCallback;
    OnServerStopCallback m_serverStopCallback;

    Core();
    Core(const Core &that) = delete;
    Core& operator=(const Core &that) = delete;
    ~Core();
public:
    static Core* Init();
    inline static Core* GetCore() { return ms_instance; }
    static void Terminate();

    inline const std::string& GetWorkingDirectory() const { return m_workingDir; }
    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    inline ElementManager* GetElementManager() const { return m_elementManager; }
    inline LogManager* GetLogManager() const { return m_logManager; }
    inline LuaManager* GetLuaManager() const { return m_luaManager; }
    inline NetworkManager* GetNetworkManager() const { return m_networkManager; }

    static inline void SetServerStartCallback(OnServerStartCallback f_callback) { ms_serverStartCallback = f_callback; }
    inline void SetServerPulseCallback(OnServerPulseCallback f_callback) { m_serverPulseCallback = f_callback; }
    inline void SetServerStopCallback(OnServerStopCallback f_callback) { m_serverStopCallback = f_callback; }

    void DoPulse();
};

}
