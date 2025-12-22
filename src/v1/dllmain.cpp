#include "utils.h"

// Sims2RPC module base address = 0x400000
// Offset = instruction absolute address - 0x400000
#define FUN1_BASE_OFFSET 0x99481A    // 0xD9481A
#define FUN1_JMPFROM_OFFSET 0x99481D // 0xD9481D
#define FUN1_JMPTO_OFFSET 0x99482E   // 0xD9482E
// #define FUN2_BASE_OFFSET 0x9948E2 // 0xD948E2
// #define FUN2_JMPFROM_OFFSET 0x9948E9   // 0xD948E9
// #define FUN2_JMPTO_OFFSET 0x9948F3   // 0xD948F3

DWORD WINAPI MainThread(LPVOID param)
{
    HMODULE module = GetModuleHandleA(NULL);
    char *modBase = (char *)module;

    if (IsLargeAddressAware(modBase))
    {
        ///* FUN_00D947F3() *///
        // NEG ECX; NOP
        char fun1Bytes[] = {0xF7, 0xD9, 0x90};
        WriteToMemory((DWORD)modBase + FUN1_BASE_OFFSET, fun1Bytes, sizeof(fun1Bytes) / sizeof(*fun1Bytes));

        // JMP 0xD9482E
        DWORD fun1JmpFrom = (DWORD)modBase + FUN1_JMPFROM_OFFSET;
        DWORD fun1JmpTo = (DWORD)modBase + FUN1_JMPTO_OFFSET;
        INT8 fun1JmpOffset = (INT8)(fun1JmpTo - (fun1JmpFrom + 2)); // Calculate short jump offset
        char fun1JmpBytes[] = {0xEB, (char)fun1JmpOffset};
        WriteToMemory(fun1JmpFrom, fun1JmpBytes, sizeof(fun1JmpBytes) / sizeof(*fun1JmpBytes));

        ///* FUN_00D948B6() *///
        // NEG EAX; NOP; MOV dword ptr ss:[EBP+0x08], EAX; NOP
        /* char fun2Bytes[] = {0xF7, 0xD8, 0x90, 0x89, 0x45, 0x08, 0x90};
        WriteToMemory((DWORD)modBase + FUN2_BASE_OFFSET, fun2Bytes, sizeof(fun2Bytes) / sizeof(*fun2Bytes));

        // JMP 0xD948F3; NOP; NOP; NOP; NOP
        DWORD fun2JmpFrom = (DWORD)modBase + FUN2_JMPFROM_OFFSET;
        DWORD fun2JmpTo = (DWORD)modBase + FUN2_JMPTO_OFFSET;
        INT8 fun2JmpOffset = (INT8)(fun2JmpTo - (fun2JmpFrom + 2));
        char fun2JmpBytes[] = {0xEB, (char)fun2JmpOffset, 0x90, 0x90, 0x90, 0x90};
        WriteToMemory(fun2JmpFrom, fun2JmpBytes, sizeof(fun2JmpBytes) / sizeof(*fun2JmpBytes)); */
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
