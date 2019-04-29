#pragma once
#include "Interfaces/ICore.h"
#include "Managers/ElementManager.h"
#include "Managers/AsyncManager/AsyncManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"
#include "Managers/VRManager.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class ConfigManager;
class ModuleManager;
class PreRenderManager;

class Core final : public ICore
{
    static Core *ms_instance;

    AsyncManager *m_asyncManager;
    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    InheritanceManager *m_inheritManager;
    LogManager *m_logManager;
    ModuleManager *m_moduleManager;
    NetworkManager *m_networkManager;
    PhysicsManager *m_physicsManager;
    RenderManager *m_renderManager;
    PreRenderManager *m_preRenderManager;
    SfmlManager *m_sfmlManager;
    SoundManager *m_soundManager;
    VRManager *m_vrManager;

    bool m_state;
    CustomArguments m_arguments;

    Core();
    Core(const Core &that) = delete;
    Core& operator=(const Core &that) = delete;
    ~Core();
public:
    static bool Init();
    static Core* GetCore();
    static bool Terminate();

    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    ElementManager* GetElementManager() const;
    AsyncManager* GetAsyncManager() const;
    InheritanceManager* GetInheritManager() const;
    LogManager* GetLogManager() const;
    inline ModuleManager* GetModuleManager() const { return m_moduleManager; }
    NetworkManager* GetNetworkManager() const;
    PhysicsManager* GetPhysicsManager() const;
    RenderManager* GetRenderManager() const;
    inline PreRenderManager* GetPreRenderManager() const { return m_preRenderManager; }
    SfmlManager* GetSfmlManager() const;
    SoundManager* GetSoundManager() const;
    VRManager* GetVRManager() const;

    bool DoPulse();
};

}
