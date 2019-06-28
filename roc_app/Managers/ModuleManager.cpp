#include "stdafx.h"

#include "Managers/ModuleManager.h"
#include "Core/Core.h"
#include "Interfaces/IModule.h"

#include "Managers/ConfigManager.h"
#include "Managers/LogManager.h"

ROC::ModuleManager::ModuleManager(Core *f_core)
{
    m_core = f_core;
}
ROC::ModuleManager::~ModuleManager()
{
}

void ROC::ModuleManager::LoadModules()
{
    std::string l_log;

    auto &l_modules = m_core->GetConfigManager()->GetModules();
    for(auto &l_modulePath : l_modules)
    {
        HMODULE l_dll = LoadLibraryA(l_modulePath.c_str());
        if(l_dll)
        {
            ModuleInitFunc l_func = reinterpret_cast<ModuleInitFunc>(GetProcAddress(l_dll, "ModuleInit"));
            if(l_func)
            {
                IModule *l_module = (*l_func)(m_core);
                if(l_module)
                {
                    m_modules.push_back(l_module);
                    m_libraries.push_back(l_dll);

                    l_log.assign("Module '");
                    l_log.append(l_modulePath);
                    l_log.append("' is loaded");
                }
                else
                {
                    FreeLibrary(l_dll);

                    l_log.assign("No module interface in module '");
                    l_log.append(l_modulePath);
                    l_log.push_back('\'');
                }
            }
            else
            {
                FreeLibrary(l_dll);

                l_log.assign("Unable to find entry point in module '");
                l_log.append(l_modulePath);
                l_log.push_back('\'');
            }
        }
        else
        {
            l_log.assign("Unable to load '");
            l_log.append(l_modulePath);
            l_log.append("' module");
        }
        m_core->GetLogManager()->Log(l_log);
    }
}
void ROC::ModuleManager::UnloadModules()
{
    for(auto l_library : m_libraries) FreeLibrary(l_library);
}

void ROC::ModuleManager::SignalGlobalEvent(unsigned char f_event, const CustomArguments &f_args)
{
    for(auto l_module : m_modules) l_module->RecieveGlobalEvent(f_event, f_args);
}

void ROC::ModuleManager::DoPulse()
{
    for(auto l_module : m_modules) l_module->DoPulse();
}
