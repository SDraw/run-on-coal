#pragma once

namespace ROC
{

class ICore; class Core;
class IModule;
class CustomArguments;

typedef IModule*(*ServerModuleInitFunc)(ICore*);

class ModuleManager
{
    Core *m_core;

    std::vector<IModule*> m_modules;
    std::vector<HMODULE> m_libraries;
protected:
    explicit ModuleManager(Core *f_core);
    ~ModuleManager();

    void LoadModules();
    void UnloadModules();

    void SignalGlobalEvent(unsigned char f_event, const CustomArguments *f_args);

    void DoPulse();

    friend Core;
    friend class NetworkManager;
};

}
