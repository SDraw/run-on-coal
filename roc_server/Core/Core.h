#pragma once
#include "Interfaces/ICore.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class ConfigManager;
class ElementManager;
class LogManager;
class ModuleManager;
class NetworkManager;

class Core final : public ICore
{
    static Core *ms_instance;

    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    LogManager *m_logManager;
    ModuleManager *m_moduleManager;
    NetworkManager *m_networkManager;

    std::chrono::milliseconds m_pulseTick;
    CustomArguments m_arguments;

    Core();
    Core(const Core &that) = delete;
    Core& operator=(const Core &that) = delete;
    ~Core();

    // ROC::ICore
    ROC::IElementManager* GetIElementManager() const;
    ROC::ILogManager* GetILogManager() const;
    ROC::INetworkManager* GetINetworkManager() const;
public:
    static bool Init();
    inline static Core* GetCore() { return ms_instance; }
    static void Terminate();

    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    inline ElementManager* GetElementManager() const { return m_elementManager; }
    inline LogManager* GetLogManager() const { return m_logManager; }
    inline ModuleManager* GetModuleManager() const { return m_moduleManager; }
    inline NetworkManager* GetNetworkManager() const { return m_networkManager; }

    void DoPulse();
};

}
