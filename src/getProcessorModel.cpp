#include "functions.hpp"
#include <iostream>
#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

void Functions::getProccesorModel()
{
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize COM library. Error code: " << hres << std::endl;
        return;
    }

    // Set security for local connections.
    hres = CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr);
    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize security. Error code: " << hres << std::endl;
        CoUninitialize();
        return;
    }

    IWbemLocator* pLoc = nullptr;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        reinterpret_cast<LPVOID*>(&pLoc));

    if (FAILED(hres))
    {
        std::cerr << "Failed to create IWbemLocator object. Error code: " << hres << std::endl;
        CoUninitialize();
        return;
    }

    //Connect to the desired namespace
    IWbemServices* pSvc = nullptr;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr,
        &pSvc);

    if (FAILED(hres))
    {
        std::cerr << "Failed to connect to WMI namespace. Error code: " << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return;
    }

    //Set security levels of the proxy
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE);
    if (FAILED(hres))
    {
        std::cerr << "Failed to set proxy blanket. Error code: " << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    IEnumWbemClassObject* pEnumerator = nullptr;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Processor"), // Class name for processor information
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &pEnumerator);
    if (FAILED(hres))
    {
        std::cerr << "Getting processor data failed. Error code: " << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // Retrieves processor information
    IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0)
            break;

        VARIANT vtProp;
        hres = pclsObj->Get(L"Name", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hres))
        {
            std::wcout << "Processor Model: " << vtProp.bstrVal << "\n";
            VariantClear(&vtProp);
        }

        VARIANT Architecture;
        hres = pclsObj->Get(L"Architecture", 0, &Architecture, nullptr, nullptr);
        if (SUCCEEDED(hres))
        {
            int architecture_value = Architecture.uintVal;
            switch (architecture_value)
            {
            case 0:
                std::wcout << "CPU Architecture: " << "x86" << "\n";
                break;
            case 1:
                std::wcout << "CPU Architecture: " << "MIPS" << "\n";
                break;
            case 2:
                std::wcout << "CPU Architecture: " << "Alpha" << "\n";
                break;
            case 3:
                std::wcout << "CPU Architecture: " << "PowerPC" << "\n";
                break;
            case 6:
                std::wcout << "CPU Architecture: " << "ia64" << "\n";
                break;
            case 9:
                std::wcout << "CPU Architecture: " << "x64" << "\n";
                break;
            default:
                break;
            }
            VariantClear(&Architecture);
        }
        if (FAILED(hres))
        {
            std::cerr << "Failed to get CPU architecture. Error code: " << hres << "\n";
            VariantClear(&Architecture);
        }


        pclsObj->Release();
    }

    // Step 8: Clean up
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    // Print number of processor cores
    std::cout << "Number of Cores: " << sysInfo.dwNumberOfProcessors << "\n";
    //std::cout << "CPU Architecture: " << sysInfo.dwProcessorType << "\n";
}


/*
[Dynamic, Provider("CIMWin32"), UUID("{8502C4BB-5FBB-11D2-AAC1-006008C78BC7}"), AMENDMENT]
class Win32_Processor : CIM_Processor
{
  uint16   AddressWidth;
  uint16   Architecture;
  string   AssetTag;
  uint16   Availability;
  string   Caption;
  uint32   Characteristics;
  uint32   ConfigManagerErrorCode;
  boolean  ConfigManagerUserConfig;
  uint16   CpuStatus;
  string   CreationClassName;
  uint32   CurrentClockSpeed;
  uint16   CurrentVoltage;
  uint16   DataWidth;
  string   Description;
  string   DeviceID;
  boolean  ErrorCleared;
  string   ErrorDescription;
  uint32   ExtClock;
  uint16   Family;
  datetime InstallDate;
  uint32   L2CacheSize;
  uint32   L2CacheSpeed;
  uint32   L3CacheSize;
  uint32   L3CacheSpeed;
  uint32   LastErrorCode;
  uint16   Level;
  uint16   LoadPercentage;
  string   Manufacturer;
  uint32   MaxClockSpeed;
  string   Name;
  uint32   NumberOfCores;
  uint32   NumberOfEnabledCore;
  uint32   NumberOfLogicalProcessors;
  string   OtherFamilyDescription;
  string   PartNumber;
  string   PNPDeviceID;
  uint16   PowerManagementCapabilities[];
  boolean  PowerManagementSupported;
  string   ProcessorId;
  uint16   ProcessorType;
  uint16   Revision;
  string   Role;
  boolean  SecondLevelAddressTranslationExtensions;
  string   SerialNumber;
  string   SocketDesignation;
  string   Status;
  uint16   StatusInfo;
  string   Stepping;
  string   SystemCreationClassName;
  string   SystemName;
  uint32   ThreadCount;
  string   UniqueId;
  uint16   UpgradeMethod;
  string   Version;
  boolean  VirtualizationFirmwareEnabled;
  boolean  VMMonitorModeExtensions;
  uint32   VoltageCaps;
};
*/