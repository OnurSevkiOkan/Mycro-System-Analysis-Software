#include "functions.hpp"
#include <chrono>
#include <iostream>

void Functions::getSystemUptime()
{
    // Get the system uptime in milliseconds
    ULONGLONG uptimeMillis = GetTickCount64();

    // Convert milliseconds to hours, minutes, and seconds
    std::chrono::milliseconds uptime(uptimeMillis);

    auto days = std::chrono::duration_cast<std::chrono::hours>(uptime) / 24;
    uptime -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(uptime) % 24;
    uptime -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(uptime) % 60;
    uptime -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(uptime) % 60;

    // Display the system uptime in a human-readable format
    std::cout << "System Uptime: " << days.count() << " days, " << hours.count() << " hours, "
        << minutes.count() << " minutes, "
        << seconds.count() << " seconds" << std::endl;
}