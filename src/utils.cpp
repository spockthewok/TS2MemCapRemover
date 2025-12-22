#include "utils.h"

bool IsLargeAddressAware(char *modBase)
{
    PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)modBase + ((PIMAGE_DOS_HEADER)modBase)->e_lfanew);
    return (ntHeader->FileHeader.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE) != 0;
}

void WriteToMemory(DWORD addressToWrite, char *valueToWrite, int byteNum)
{
    // Used to change our file access type, stores the old access type and restores it after memory is written
    unsigned long OldProtection;
    // Give that address read and write permissions and store the old permissions at oldProtection
    VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

    // Write the memory into the program and overwrite previous value
    memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);

    // Reset the permissions of the address back to oldProtection after writing memory
    VirtualProtect((LPVOID)(addressToWrite), byteNum, OldProtection, NULL);
}