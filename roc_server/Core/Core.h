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
class Core
{
    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    LogManager *m_logManager;
    LuaManager *m_luaManager;
    MemoryManager *m_memoryManager;
    NetworkManager *m_networkManager;
    static Core *m_instance;
    std::string m_workingDir;
    std::chrono::milliseconds m_pulseTick;
    LuaArguments *m_argument;

    Core();
    Core(const Core& that);
    Core &operator =(const Core &that);
    ~Core();
public:
    static Core* Init();
    static void Terminate();

    inline void GetWorkingDirectory(std::string &f_path) { f_path.assign(m_workingDir); }
    inline ConfigManager* GetConfigManager() { return m_configManager; }
    inline ElementManager* GetElementManager() { return m_elementManager; }
    inline LogManager* GetLogManager() { return m_logManager; }
    inline LuaManager* GetLuaManager() { return m_luaManager; }
    inline MemoryManager* GetMemoryManager() { return m_memoryManager; }
    inline NetworkManager* GetNetworkManager() { return m_networkManager; }

    void DoPulse();
};

}
