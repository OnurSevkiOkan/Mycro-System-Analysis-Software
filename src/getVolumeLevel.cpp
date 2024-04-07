#include "functions.hpp"
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>


int Functions::getVolumeLevel()
{
    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;
    float fLevel = 0.0f;

    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        printf("Error: Failed to initialize COM library: hr = 0x%08x\n", hr);
        return hr;
    }

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        printf("Error: Failed to create device enumerator: hr = 0x%08x\n", hr);
        CoUninitialize();
        return hr;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) {
        printf("Error: Failed to get default audio endpoint: hr = 0x%08x\n", hr);
        pEnumerator->Release();
        CoUninitialize();
        return hr;
    }

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr)) {
        printf("Error: Failed to activate endpoint volume interface: hr = 0x%08x\n", hr);
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return hr;
    }

    hr = pEndpointVolume->GetMasterVolumeLevelScalar(&fLevel);
    if (FAILED(hr)) {
        printf("Error: Failed to get master volume level: hr = 0x%08x\n", hr);
        pEndpointVolume->Release();
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return hr;
    }

    printf("Master Volume Level: %f\n", fLevel);

    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();
    return 1;
}