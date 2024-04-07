#include "functions.hpp"
#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>
#include <iostream>

#pragma comment(lib, "wbemuuid.lib")

void Functions::getBatteryInformation()
{

	SYSTEM_POWER_STATUS powerStatus;
	if (GetSystemPowerStatus(&powerStatus)) {
		// Battery life percentage
		std::cout << "Battery Percentage: " << static_cast<int>(powerStatus.BatteryLifePercent) << "%" << std::endl;

		// Battery status (0 = discharging, 1 = charging, 255 = unknown)
	}
	else {
		std::cerr << "Failed to retrieve power status." << GetLastError() << "\n";
		return;
	}

	HRESULT hres;
	
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		std::cerr << "Failed to initialize COM library. Error code: " << hres << "\n";
		return;
	}

	hres = CoInitializeSecurity(
		nullptr,
		-1,
		nullptr,
		nullptr,
		RPC_C_AUTHN_LEVEL_DEFAULT, // 0
		RPC_C_IMP_LEVEL_IMPERSONATE, // 3
		nullptr,
		EOAC_NONE, // 0
		nullptr
	);
	if (FAILED(hres))
	{
		std::cerr << "Failed to initialize security. Error code: " << hres << "\n";
		CoUninitialize();
	}

	IWbemLocator* pLoc = NULL; // Used to connect WMI namespace
	
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc
	);
	
	if (FAILED(hres))
	{
		std::cerr << "Failed to create IWbemLocator object. Error code: " << hres << "\n";
		CoUninitialize();
	}

	IWbemServices* pSvc = NULL;

	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		std::cout << "Failed to connect. Error code: " << hres << "\n";
		CoUninitialize();
	}

	//std::cout << "Connected to ROOT\\CIMV2 WMI namespace." << "\n";

	hres = CoSetProxyBlanket(
		pSvc,                        
		RPC_C_AUTHN_WINNT,           
		RPC_C_AUTHZ_NONE,            
		NULL,                        
		RPC_C_AUTHN_LEVEL_CALL,      
		RPC_C_IMP_LEVEL_IMPERSONATE, 
		NULL,                        
		EOAC_NONE                    
	);

	if (FAILED(hres))
	{
		std::cout << "Could not set proxy. Error code: " << hres << "\n";
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Battery"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtBatteryStatus;
		VARIANT vtProp;

		VariantInit(&vtProp);

		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr) && (vtProp.vt == VT_BSTR))
		{
			std::wcout << "Battery Name: " << vtProp.bstrVal << "\n";
			VariantClear(&vtProp);
		}
		else
		{
			std::wcerr << "Failed to get Battery Name property. Error code: " << hr << "\n";
			VariantClear(&vtProp);
			pclsObj->Release();
			continue;
		}

		VariantInit(&vtBatteryStatus);

		hr = pclsObj->Get(L"BatteryStatus", 0, &vtBatteryStatus, 0, 0);
		if (SUCCEEDED(hr) && (vtBatteryStatus.vt == VT_I4))
		{
			int batteryStatus = vtBatteryStatus.intVal;

			switch (batteryStatus) {
			case 1:
				std::wcout << "Battery Status: Other (unknown)\n";
				break;
			case 2:
				std::wcout << "Battery Status: Fully charged\n";
				break;
			case 3:
				std::wcout << "Battery Status: Discharging\n";
				break;
			case 4:
				std::wcout << "Battery Status: Charging\n";
				break;
			case 5:
				std::wcout << "Battery Status: Critical (low battery)\n";
				break;
			case 6:
				std::wcout << "Battery Status: Partially charged\n";
				break;
			default:
				std::wcout << "Battery Status: Unknown\n";
				break;
			}

			VariantClear(&vtBatteryStatus);
		}
		else
		{
			std::cerr << "Failed to get Battery Status property. Error code: " << hr << "\n";
			VariantClear(&vtBatteryStatus);
		}

		pclsObj->Release();
	}

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

}