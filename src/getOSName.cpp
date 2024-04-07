#include "functions.hpp"
#include <iostream>
#include <Windows.h>
#include <WbemIdl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

void Functions::getOSName()
{
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
		_bstr_t(L"ROOT\\CIMV2"), 
		NULL,                    
		NULL,                    
		0,                       
		NULL,                    
		0,                      
		0,                       
		&pSvc                    
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
		bstr_t("SELECT * FROM Win32_OperatingSystem"),
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
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		VariantInit(&vtProp);

		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		wcout << " OS Name : " << vtProp.bstrVal << endl;
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

}