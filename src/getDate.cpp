#include "functions.hpp"

void Functions::getDate()
{
    SYSTEMTIME st = { 1 };

    GetSystemTime(&st);

    //std::cout << (L"The UTC time is: %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);
    //cout << st.wDay;

    std::cout << "Date: " << st.wDay << "/" << st.wMonth << "/" << st.wYear << "\n";
}

