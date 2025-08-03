#include "temperature_monitor.h"
#include <iostream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <objbase.h> // For CoInitializeEx, CoUninitialize
#include <comdef.h>
#include <Wbemidl.h> // For WMI
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "ole32.lib") // For CoInitializeEx, CoUninitialize
#pragma comment(lib, "oleaut32.lib") // For VariantClear
#endif

std::vector<TemperatureInfo> getTemperatureInfo() {
    std::vector<TemperatureInfo> temps;
#ifdef _WIN32
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        // std::cerr << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
        return temps; // Program has failed.
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if (FAILED(hres)) {
        // std::cerr << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return temps;
    }

    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        // std::cerr << "Failed to create IWbemLocator object. Err code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return temps;
    }

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\WMI"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

    if (FAILED(hres)) {
        // std::cerr << "Could not connect. Error code = 0x" << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return temps;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM MSAcpi_ThermalZoneTemperature"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

    if (FAILED(hres)) {
        // std::cerr << "Query for MSAcpi_ThermalZoneTemperature failed. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return temps;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            TemperatureInfo tempInfo;
            tempInfo.currentTemperature = (vtProp.uintVal - 2732) / 10.0f;
            tempInfo.unit = L"Celsius";

            hr = pclsObj->Get(L"InstanceName", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr)) {
                tempInfo.sensorName = vtProp.bstrVal;
            } else {
                tempInfo.sensorName = L"Unknown Sensor";
            }
            temps.push_back(tempInfo);
            VariantClear(&vtProp);
        }
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
#endif
    return temps;
}