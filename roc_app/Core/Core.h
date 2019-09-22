#pragma once
#include "Interfaces/ICore.h"
#include "Utils/CustomArguments.h"

namespace ROC
{

class AsyncManager;
class ConfigManager;
class ElementManager;
class LogManager;
class ModuleManager;
class NetworkManager;
class PhysicsManager;
class PreRenderManager;
class RenderManager;
class SfmlManager;
class SoundManager;
class VRManager;

class Core final : public ICore
{
    static Core *ms_instance;

    AsyncManager *m_asyncManager;
    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
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

    // ROC::ICore
    IAsyncManager* GetIAsyncManager() const;
    IElementManager* GetIElementManager() const;
    ILogManager* GetILogManager() const;
    INetworkManager* GetINetworkManager() const;
    IPhysicsManager* GetIPhysicsManager() const;
    IRenderManager* GetIRenderManager() const;
    ISfmlManager* GetISfmlManager() const;
    ISoundManager* GetISoundManager() const;
    IVRManager* GetIVRManager() const;
public:
    static bool Init();
    static Core* GetCore();
    static void Terminate();

    inline ConfigManager* GetConfigManager() const { return m_configManager; }
    inline ElementManager* GetElementManager() const { return m_elementManager; }
    inline AsyncManager* GetAsyncManager() const { return m_asyncManager; }
    inline LogManager* GetLogManager() const { return m_logManager; }
    inline ModuleManager* GetModuleManager() const { return m_moduleManager; }
    inline NetworkManager* GetNetworkManager() const { return m_networkManager; }
    inline PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
    inline RenderManager* GetRenderManager() const { return m_renderManager; }
    inline PreRenderManager* GetPreRenderManager() const { return m_preRenderManager; }
    inline SfmlManager* GetSfmlManager() const { return m_sfmlManager; }
    inline SoundManager* GetSoundManager() const { return m_soundManager; }
    inline VRManager* GetVRManager() const { return m_vrManager; }

    bool DoPulse();
};

}
