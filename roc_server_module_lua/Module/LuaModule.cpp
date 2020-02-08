#include "stdafx.h"

#include "Module/LuaModule.h"
#include "Module/EventHandler.h"
#include "Lua/LuaVM.h"

LuaModule* LuaModule::ms_instance = nullptr;

LuaModule::LuaModule(ROC::ICore *f_core)
{
    m_core = f_core;

    m_luaVM = new LuaVM(this);
    m_eventHandler = new EventHandler(this);
}
LuaModule::~LuaModule()
{
    delete m_eventHandler;
    delete m_luaVM;
}

bool LuaModule::Init(ROC::ICore *f_core)
{
    if(!ms_instance)
    {
        ms_instance = new LuaModule(f_core);

        pugi::xml_document *l_configFile = new pugi::xml_document();
        if(l_configFile->load_file("server_module_lua.xml"))
        {
            pugi::xml_node l_root = l_configFile->child("scripts");
            if(l_root)
            {
                for(pugi::xml_node l_node = l_root.child("script"); l_node; l_node = l_node.next_sibling("script"))
                {
                    pugi::xml_attribute l_attrib = l_node.attribute("src");
                    if(l_attrib) ms_instance->m_luaVM->LoadScript(l_attrib.as_string());
                }
            }
        }
        delete l_configFile;
    }
    return (ms_instance != nullptr);
}
void LuaModule::Terminate()
{
    if(ms_instance)
    {
        delete ms_instance;
        ms_instance = nullptr;
    }
}

void LuaModule::RecieveGlobalEvent(unsigned char f_event, const ROC::ICustomArguments *f_args)
{
    switch(f_event)
    {
        case ME_OnServerStart:
            m_eventHandler->CallEvent(EventHandler::EHE_OnServerStart, f_args);
            break;
        case ME_OnServerStop:
            m_eventHandler->CallEvent(EventHandler::EHE_OnServerStop, f_args);
            break;
        case ME_OnServerPulse:
            m_eventHandler->CallEvent(EventHandler::EHE_OnServerPulse, f_args);
            break;
        case ME_OnNetworkClientConnect:
            m_eventHandler->CallEvent(EventHandler::EHE_onNetworkClientConnect, f_args);
            break;
        case ME_OnNetworkClientDisconnect:
            m_eventHandler->CallEvent(EventHandler::EHE_onNetworkClientDisconnect, f_args);
            break;
        case ME_OnNetworkDataRecieve:
            m_eventHandler->CallEvent(EventHandler::EHE_onNetworkDataRecieve, f_args);
            break;
    }
}

void LuaModule::DoPulse()
{
    m_luaVM->DoPulse();
}
