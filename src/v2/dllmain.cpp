#include "utils.h"

// Sims2RPC module base address = 0x400000
// Offset = instruction absolute address - 0x400000
#define FUN1_JMPFROM_OFFSET 0x994812 // 0xD9481D
#define FUN1_JMPTO_OFFSET 0xDD2F9A   // 0x11D2F9A

DWORD WINAPI MainThread(LPVOID param)
{
    HMODULE module = GetModuleHandleA(NULL);
    char *modBase = (char *)module;

    if (IsLargeAddressAware(modBase))
    {
        ///* FUN_00D947F3() *///
        // JMP 0x11D2F9A; NOP
        char fun1Bytes[] = {0xE9, 0x83, 0xE7, 0x43, 0x00,
                            0x90};
        WriteToMemory((DWORD)modBase + FUN1_JMPFROM_OFFSET, fun1Bytes, sizeof(fun1Bytes) / sizeof(*fun1Bytes));

        ///* Code cave at 0x11D2F9A *///
        // CMP ECX,30000001 (check address < 0xCFFFFFFF - address is negated by earlier logic for comparison)
        // JL 0xD9481A (jump to failsafe logic if > 0xCFFFFFFF)
        // NEG ECX (undo negation)
        // JMP 0xD9482E; NOP (jump to non-failsafe logic)
        char caveBytes[] = {0x81, 0xF9, 0x01, 0x00, 0x00, 0x30,
                            0x0F, 0x8C, 0x74, 0x18, 0xBC, 0xFF,
                            0xF7, 0xD9,
                            0xE9, 0x81, 0x18, 0xBC, 0xFF,
                            0x90};
        WriteToMemory((DWORD)modBase + FUN1_JMPTO_OFFSET, caveBytes, sizeof(caveBytes) / sizeof(*caveBytes));
    }
    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}