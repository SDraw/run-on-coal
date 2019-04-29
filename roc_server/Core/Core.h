#pragma once
#include "Interfaces/ICore.h"
#include "Managers/ElementManager.h"
#include "Managers/LogManager.h"
#include "Managers/NetworkManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class ConfigManager;
class ModuleManager;

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
public:
    static bool Init();
    inline static Core* GetCore() { return ms_instance; }
    static bool Terminate();

    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    ElementManager* GetElementManager() const;
    LogManager* GetLogManager() const;
    inline ModuleManager* GetModuleManager() const { return m_moduleManager; }
    NetworkManager* GetNetworkManager() const;

    void DoPulse();
};

}
