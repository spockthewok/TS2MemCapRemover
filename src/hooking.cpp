#include "hooking.h"

namespace Hooking
{
    bool IsLargeAddressAware(DWORD modBase)
    {
        PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)modBase + ((PIMAGE_DOS_HEADER)modBase)->e_lfanew);

        return (ntHeader->FileHeader.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE) != 0;
    }

    void WriteToMemory(DWORD addressToWrite, const BYTE *valueToWrite, size_t byteNum)
    {
        DWORD oldProtection;
        BYTE *pAddress = reinterpret_cast<BYTE *>(addressToWrite);

        VirtualProtect(pAddress, byteNum, PAGE_EXECUTE_READWRITE, &oldProtection);
        memcpy(pAddress, valueToWrite, byteNum);
        VirtualProtect(pAddress, byteNum, oldProtection, NULL);
    }
}