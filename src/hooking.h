#include "headers.h"

namespace Hooking
{
    bool IsLargeAddressAware(DWORD modBase);
    void WriteToMemory(DWORD addressToWrite, const BYTE *valueToWrite, size_t byteNum);
}