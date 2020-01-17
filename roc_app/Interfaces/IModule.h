#pragma once

namespace ROC
{

class ICustomArguments;

class IModule
{
public:
    enum ModuleEvent : unsigned char
    {
        ME_OnEngineStart = 0U,
        ME_OnEngineStop,
        ME_OnRender,
        ME_OnVRRender,
        ME_OnPreRender,
        ME_OnWindowClose,
        ME_OnWindowResize,
        ME_OnWindowFocus,
        ME_OnKeyPress,
        ME_OnMouseKeyPress,
        ME_OnMouseScroll,
        ME_OnCursorMove,
        ME_OnCursorEnter,
        ME_OnJoypadStateChange,
        ME_OnJoypadButton,
        ME_OnJoypadAxis,
        ME_OnVRControllerKeyPress,
        ME_OnVRControllerKeyTouch,
        ME_OnVRControllerAxis,
        ME_OnTextInput,
        ME_OnNetworkStateChange,
        ME_OnNetworkDataRecieve,
        ME_OnAsyncTask
    };

    virtual void RecieveGlobalEvent(unsigned char f_event, const ICustomArguments *f_args) = 0;
    virtual void DoPulse() = 0;
};

}
