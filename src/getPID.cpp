#include "functions.hpp"

int Functions::getProcessID()
{
    int pid = GetCurrentProcessId();
    std::cout << "Current PID: " << pid << endl;
    return pid;
}
