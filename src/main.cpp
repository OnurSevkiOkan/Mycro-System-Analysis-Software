#include <iostream>
#include "functions.hpp"

using namespace std;

Functions function;

bool askToContinue() {
    char response;
    cout << "Do you want to continue? (Y/N): ";
    cin >> response;
    return (response == 'Y' || response == 'y');
}

int main() {
    bool continueProgram = true;

    while (continueProgram) {
        int function_num;

        cout << "1) Date\n2) Delete File\n3) Ram Usage\n4) Get PID\n5) Get Volume Level\n6) Get System Username\n7) Get PC Model\n8) Get OS Name\n9) Get System Uptime\n10) Get CPU Model\n11) Get Battery Information\nChoose what you want: ";
        cin >> function_num;

        switch (function_num) {
        case 1:
            function.getDate();
            break;
        case 2:
            function.delete_file();
            cout << "File Deleted!";
            break;
        case 3:
            function.getRamInformation();
            break;
        case 4:
            function.getProcessID();
            break;
        case 5:
            function.getVolumeLevel();
            break;
        case 6:
            function.getSystemUsername();
            break;
        case 7:
            function.getPCModel();
            break;
        case 8:
            function.getOSName();
            break;
        case 9:
            function.getSystemUptime();
            break;
        case 10:
            function.getProccesorModel();
            break;
        case 11:
            function.getBatteryInformation();
            break;
        default:
            cout << "Invalid choice!" << endl;
        }

        continueProgram = askToContinue();
        system("cls");
    }

    cout << "Exiting.." << endl;
    return 0;
}
