#include "functions.hpp"
#include <Windows.h>

void Functions::getPCModel()
{
    LPCTSTR subkey = L"HARDWARE\\DESCRIPTION\\System\\BIOS";
    LPCTSTR name = L"SystemProductName";
    DWORD type = REG_SZ;

    HKEY hkey = HKEY_LOCAL_MACHINE;
    TCHAR value[255];
    DWORD value_length = 255;

    RegOpenKey(hkey, subkey, &hkey);
    RegQueryValueEx(hkey, name, NULL, &type, (LPBYTE)&value, &value_length);
    RegCloseKey(hkey);

    std::wstring pcModel(value);

    std::wcout << "PC Model: " << pcModel << "\n";
}