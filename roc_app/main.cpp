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
    int a = 5;
    unsigned int b = 0;
    std::memcpy(&b, &a, sizeof(int));
    std::cout << a << b << std::endl;

    if(ROC::Core::Init())
    {
        ROC::Core *l_core = ROC::Core::GetCore();
        while(l_core->DoPulse());
        ROC::Core::Terminate();
    }

    return EXIT_SUCCESS;
}
