#include <Windows.h>

#include "Utils/logs.h"
#include "Utils/hook.h"
#include "hooks.h"

using namespace Logs;

void Hook()
{
    MH_Initialize();
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
    const char* ascii = R"(
  ##  ##               ##      ##                  ##             ####     ####     ##    
  ##  ##                      ##                   ##            ##  ##   ##  ##    ##    
 ##  ##   ## ###    ####   #######     ####    ######            ##       ##       ##     
 ##  ##   ###  ##     ##     ##      ##   ##  ##  ##              ##       ##      ##     
##  ##    ##   ##    ##     ##      ######## ##   ##               ##       ##    ##      
##  ##   ##   ##     ##     ##      ##       ##  ##            ##  ##   ##  ##    ##      
 ###     ##   ##   ######    ###     ####     #####             ####     ####     ######                                                                                    
)"; // ascii-generator.site cli8x8
    Log(ascii);
    Log("Redirecting to " + std::string(FNhost));

    SetupHooks();
    MH_CreateHook((void*)CurlEasyOptP, changeeasyopt, (void**)&curl_easy_setopt_original);
    MH_EnableHook((void*)CurlEasyOptP);

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        Hook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

