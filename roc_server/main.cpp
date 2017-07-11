#include "stdafx.h"

#include "RocInc.h"

std::atomic<int> g_quitSetter = 0;

#ifndef _WIN32 
void SignalHandleFunction(int f_sig)
{
    signal(f_sig, SignalHandleFunction);
    g_quitSetter = 1;
}
#else
int WINAPI ConsoleHandlerRoutine(unsigned long l_type)
{
    g_quitSetter = 1;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 1;
}
#endif

int main(int argc, char *argv[])
{
#ifndef _WIN32
    signal(SIGINT, SignalHandleFunction);
    signal(SIGTERM, SignalHandleFunction);
#ifdef SIGBREAK
    signal(SIGBREAK, SignalHandleFunction);
#endif
#else
    SetConsoleCtrlHandler(ConsoleHandlerRoutine, 1);
    EnableScrollBar(GetConsoleWindow(), SB_BOTH, ESB_DISABLE_BOTH);
    HANDLE l_handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD l_handleMode;
    GetConsoleMode(l_handle, &l_handleMode);
    SetConsoleMode(l_handle, l_handleMode & ~ENABLE_QUICK_EDIT_MODE);

    l_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO l_sbInfo;
    GetConsoleScreenBufferInfo(l_handle, &l_sbInfo);
    l_sbInfo.dwSize.Y = l_sbInfo.srWindow.Bottom + 1;
    SetConsoleWindowInfo(l_handle, TRUE, &l_sbInfo.srWindow);
    SetConsoleScreenBufferSize(l_handle, l_sbInfo.dwSize);

    SetConsoleTitleA("RunOnCoal Server");
#endif
    ROC::Core *l_core = ROC::Core::Init();
    while(!g_quitSetter) l_core->DoPulse();
    ROC::Core::Terminate();
    return EXIT_SUCCESS;
}
