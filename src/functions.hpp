#pragma once
#include <Windows.h>
#include <iostream>

#define DIV 1048576
#define WIDTH 7

using namespace std;

class Functions
{
public:
    void getDate();
    void getRamInformation();
    int getProcessID();
    void delete_file();
    int getVolumeLevel();
    void getSystemUsername();
    void getPCModel();
    void getOSName();
    void getSystemUptime();
    void getProccesorModel();
    void getBatteryInformation();
};