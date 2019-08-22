#include "stdafx.h"

#include "Core/Core.h"

#ifdef _DEBUG
int main(int argc, char *argv[])
{
    SetConsoleTitleA("Debug Console");
#else
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#endif
    if(ROC::Core::Init())
    {
        while(ROC::Core::GetCore()->DoPulse());
        ROC::Core::Terminate();
    }

    return EXIT_SUCCESS;
}
