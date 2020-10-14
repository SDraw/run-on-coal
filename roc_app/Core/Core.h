#pragma once
#include "Interfaces/ICore.h"

namespace ROC
{

class AsyncManager;
class ConfigManager;
class ElementManager;
class LogManager;
class ModuleManager;
class PhysicsManager;
class PreRenderManager;
class RenderManager;
class SfmlManager;
class SoundManager;
class VRManager;
class CustomArguments;

class Core final : public ICore
{
    static Core *ms_instance;

    AsyncManager *m_asyncManager;
    ConfigManager *m_configManager;
    ElementManager *m_elementManager;
    LogManager *m_logManager;
    ModuleManager *m_moduleManager;
    PhysicsManager *m_physicsManager;
    RenderManager *m_renderManager;
    PreRenderManager *m_preRenderManager;
    SfmlManager *m_sfmlManager;
    SoundManager *m_soundManager;
    VRManager *m_vrManager;

    bool m_state;
    CustomArguments *m_arguments;

    Core();
    Core(const Core &that) = delete;
    Core& operator=(const Core &that) = delete;
    ~Core();

    // ROC::ICore
    IAsyncManager* GetIAsyncManager() const;
    IElementManager* GetIElementManager() const;
    ILogManager* GetILogManager() const;
    IPhysicsManager* GetIPhysicsManager() const;
    IRenderManager* GetIRenderManager() const;
    ISfmlManager* GetISfmlManager() const;
    ISoundManager* GetISoundManager() const;
    IVRManager* GetIVRManager() const;
public:
    static bool Init();
    static Core* GetInstance();
    static void Terminate();

    ConfigManager* GetConfigManager() const;
    ElementManager* GetElementManager() const;
    AsyncManager* GetAsyncManager() const;
    LogManager* GetLogManager() const;
    ModuleManager* GetModuleManager() const;
    PhysicsManager* GetPhysicsManager() const;
    RenderManager* GetRenderManager() const;
    PreRenderManager* GetPreRenderManager() const;
    SfmlManager* GetSfmlManager() const;
    SoundManager* GetSoundManager() const;
    VRManager* GetVRManager() const;

    bool DoPulse();
};

}
