#include "stdafx.h"
#include "RocInc.h"

#ifdef _DEBUG
int main(int argc, char *argv[])
#else
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    ROC::Core *l_core = ROC::Core::Init();
    while(l_core->DoPulse());
    ROC::Core::Terminate();
    return EXIT_SUCCESS;
}
