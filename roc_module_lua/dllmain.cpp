#include "stdafx.h"

#include "Core/Core.h"

Core *g_core = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    BOOL l_result = FALSE;
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            l_result = TRUE;
            break;
        case DLL_PROCESS_DETACH:
        {
            if(g_core)
            {
                Core::Terminate();
                g_core = nullptr;
            }
            l_result = TRUE;
        } break;
    }
    return l_result;
}

extern "C" __declspec(dllexport) ROC::IModule* ModuleInit(ROC::ICore *f_core)
{
    if(!g_core)
    {
        if(Core::Init(f_core)) g_core = Core::GetInstance();
    }
    return g_core;
}
