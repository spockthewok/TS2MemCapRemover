#include "hooking.h"

DWORD WINAPI MainThread(LPVOID param)
{
    const DWORD modBase = reinterpret_cast<DWORD>(GetModuleHandleA(NULL));

    if (Hooking::IsLargeAddressAware(modBase))
    {
        ///* FUN_00D947F3() *///
        // NEG ECX; NOP; JMP 0xD9482E
        const BYTE funcBytes[] = {0xF7, 0xD9, 0x90, 0xEB, 0x0F};
        Hooking::WriteToMemory((0x99481A + modBase), funcBytes, sizeof(funcBytes));
    }

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
