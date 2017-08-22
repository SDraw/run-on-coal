#pragma once
#define ROC_DEFAULT_SCRIPTS_PATH "server_scripts/"

namespace ROC
{

class ConfigManager;
class ElementManager;
class LogManager;
class LuaManager;
class MemoryManager;
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
    MemoryManager *m_memoryManager;
    NetworkManager *m_networkManager;

    std::string m_workingDir;
    std::chrono::milliseconds m_pulseTick;
    LuaArguments *m_argument;

    static OnServerStartCallback ms_serverStartCallback;
    OnServerPulseCallback m_serverPulseCallback;
    OnServerStopCallback m_serverStopCallback;

    Core();
    Core(const Core& that);
    Core &operator =(const Core &that);
    ~Core();
public:
    static Core* Init();
    static void Terminate();

    inline const std::string& GetWorkingDirectory() { return m_workingDir; }
    inline ConfigManager* GetConfigManager() { return m_configManager; }
    inline ElementManager* GetElementManager() { return m_elementManager; }
    inline LogManager* GetLogManager() { return m_logManager; }
    inline LuaManager* GetLuaManager() { return m_luaManager; }
    inline MemoryManager* GetMemoryManager() { return m_memoryManager; }
    inline NetworkManager* GetNetworkManager() { return m_networkManager; }

    static inline void SetServerStartCallback(OnServerStartCallback f_callback) { ms_serverStartCallback = f_callback; }
    inline void SetServerPulseCallback(OnServerPulseCallback f_callback) { m_serverPulseCallback = f_callback; }
    inline void SetServerStopCallback(OnServerStopCallback f_callback) { m_serverStopCallback = f_callback; }

    void DoPulse();
};

}
