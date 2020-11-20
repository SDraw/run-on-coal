#include "stdafx.h"

#include "Core/Core.h"
#include "Core/EventHandler.h"
#include "Lua/LuaVM.h"
#include "Core/TaskHandler.h"

Core* Core::ms_instance = nullptr;

Core::Core(ROC::ICore *f_core)
{
    m_engineCore = f_core;

    m_luaVM = new LuaVM(this);
    m_eventHandler = new EventHandler(this);
    m_taskHandler = new TaskHandler(this);
}

Core::~Core()
{
    delete m_taskHandler;
    delete m_eventHandler;
    delete m_luaVM;
}

bool Core::Init(ROC::ICore *f_core)
{
    if(!ms_instance)
    {
        ms_instance = new Core(f_core);

        pugi::xml_document *l_configFile = new pugi::xml_document();
        if(l_configFile->load_file("module_lua.xml"))
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

void Core::Terminate()
{
    if(ms_instance)
    {
        delete ms_instance;
        ms_instance = nullptr;
    }
}

Core* Core::GetInstance()
{
    return ms_instance;
}

ROC::ICore* Core::GetEngineICore() const
{
    return m_engineCore;
}

LuaVM* Core::GetLuaVM() const
{
    return m_luaVM;
}

EventHandler* Core::GetEventHandler() const
{
    return m_eventHandler;
}

TaskHandler* Core::GetTaskHandler() const
{
    return m_taskHandler;
}

// ROC::IModule
void Core::RecieveGlobalEvent(unsigned char f_event, const ROC::ICustomArguments *f_args)
{
    switch(f_event)
    {
        case ME_OnEngineStart:
            m_eventHandler->CallEvent(EventHandler::EHE_onEngineStart, f_args);
            break;
        case ME_OnEngineStop:
            m_eventHandler->CallEvent(EventHandler::EHE_onEngineStop, f_args);
            break;
        case ME_OnRender:
            m_eventHandler->CallEvent(EventHandler::EHE_onRender, f_args);
            break;
        case ME_OnVRRender:
            m_eventHandler->CallEvent(EventHandler::EHE_onVRRender, f_args);
            break;
        case ME_OnPreRender:
            m_eventHandler->CallEvent(EventHandler::EHE_onPreRender, f_args);
            break;
        case ME_OnWindowClose:
            m_eventHandler->CallEvent(EventHandler::EHE_onWindowClose, f_args);
            break;
        case ME_OnWindowResize:
            m_eventHandler->CallEvent(EventHandler::EHE_onWindowResize, f_args);
            break;
        case ME_OnWindowFocus:
            m_eventHandler->CallEvent(EventHandler::EHE_onWindowFocus, f_args);
            break;
        case ME_OnKeyPress:
            m_eventHandler->CallEvent(EventHandler::EHE_onKeyPress, f_args);
            break;
        case ME_OnMouseKeyPress:
            m_eventHandler->CallEvent(EventHandler::EHE_onMouseKeyPress, f_args);
            break;
        case ME_OnMouseScroll:
            m_eventHandler->CallEvent(EventHandler::EHE_onMouseScroll, f_args);
            break;
        case ME_OnCursorMove:
            m_eventHandler->CallEvent(EventHandler::EHE_onCursorMove, f_args);
            break;
        case ME_OnCursorEnter:
            m_eventHandler->CallEvent(EventHandler::EHE_onCursorEnter, f_args);
            break;
        case ME_OnJoypadStateChange:
            m_eventHandler->CallEvent(EventHandler::EHE_onJoypadStateChange, f_args);
            break;
        case ME_OnJoypadButton:
            m_eventHandler->CallEvent(EventHandler::EHE_onJoypadButton, f_args);
            break;
        case ME_OnJoypadAxis:
            m_eventHandler->CallEvent(EventHandler::EHE_onJoypadAxis, f_args);
            break;
        case ME_OnVRDeviceActivated:
            m_eventHandler->CallEvent(EventHandler::EHE_onVRDeviceActivated, f_args);
            break;
        case ME_OnVRDeviceDeactivated:
            m_eventHandler->CallEvent(EventHandler::EHE_onVRDeviceDeactivated, f_args);
            break;
        case ME_OnVRDeviceKeyPress:
            m_eventHandler->CallEvent(EventHandler::EHE_onVRDeviceKeyPress, f_args);
            break;
        case ME_OnVRDeviceKeyTouch:
            m_eventHandler->CallEvent(EventHandler::EHE_onVRDeviceKeyTouch, f_args);
            break;
        case ME_OnTextInput:
            m_eventHandler->CallEvent(EventHandler::EHE_onTextInput, f_args);
            break;
        case ME_OnAsyncTask:
            m_taskHandler->ProcessTask(f_args);
            break;
    }
}

void Core::DoPulse()
{
    m_luaVM->DoPulse();
}
