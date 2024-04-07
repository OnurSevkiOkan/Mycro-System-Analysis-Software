#include "functions.hpp"
#include <Windows.h>

void Functions::delete_file()
{
    string filename;
    cout << "File Name: ";
    cin >> filename;

    wstring temp = wstring(filename.begin(), filename.end());

    LPCWSTR wideString = temp.c_str();

    DeleteFile(wideString);
}