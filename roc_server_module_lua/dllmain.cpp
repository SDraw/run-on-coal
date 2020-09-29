#include "stdafx.h"

#include "Module/LuaModule.h"

LuaModule *g_module = nullptr;

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
            if(g_module)
            {
                LuaModule::Terminate();
                g_module = nullptr;
            }
            l_result = TRUE;
        } break;
    }
    return l_result;
}

extern "C" __declspec(dllexport) ROC::IModule* ServerModuleInit(ROC::ICore *f_core)
{
    if(!g_module)
    {
        if(LuaModule::Init(f_core)) g_module = LuaModule::GetModule();
    }
    return g_module;
}
