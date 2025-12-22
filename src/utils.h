#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>

bool IsLargeAddressAware(char *modBase);
void WriteToMemory(DWORD addressToWrite, char *valueToWrite, int byteNum);