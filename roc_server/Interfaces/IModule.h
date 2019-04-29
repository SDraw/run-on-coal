#pragma once

class CustomArguments;

namespace ROC
{

class ICore;

class IModule
{
public:
    enum ModuleEvent : unsigned char
    {
        ME_OnServerStart = 0U,
        ME_OnServerStop,
        ME_OnServerPulse,
        ME_OnNetworkClientConnect,
        ME_OnNetworkClientDisconnect,
        ME_OnNetworkDataRecieve
    };

    virtual void RecieveGlobalEvent(unsigned char f_event, const CustomArguments &f_args) = 0;
    virtual void DoPulse() = 0;
};

}
