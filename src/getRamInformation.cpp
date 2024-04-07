#include "functions.hpp"
#include <tchar.h>

void Functions::getRamInformation()
{
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    _tprintf(TEXT("There is  %*ld percent of memory in use.\n"), WIDTH, statex.dwMemoryLoad);
    _tprintf(TEXT("There are %*I64d total Mbytes of physical memory.\n"), WIDTH, statex.ullTotalPhys / DIV);
    _tprintf(TEXT("There are %*I64d free Mbytes of physical memory.\n"), WIDTH, statex.ullAvailPhys / DIV);
}