/* wmi.c
 *
 * Copyright 2023 OrbitalMk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "wmi.h"

void
wmi_query(wchar_t* query, QueryFunc func, char* str, gsize size) {
    IWbemLocator* locator  = NULL;
    IWbemServices* services = NULL;
    IEnumWbemClassObject *results  = NULL;

    BSTR resource = SysAllocString(L"ROOT\\CIMV2");
    BSTR language = SysAllocString(L"WQL");
    BSTR sysquery = SysAllocString(query);

    CoInitializeEx(0, COINIT_MULTITHREADED);
    CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    CoCreateInstance(&CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, &IID_IWbemLocator, (LPVOID*) &locator);
    locator->lpVtbl->ConnectServer(locator, resource, NULL, NULL, NULL, 0, NULL, NULL, &services);

    services->lpVtbl->ExecQuery(services, language, sysquery, WBEM_FLAG_BIDIRECTIONAL, NULL, &results);

    if (results != NULL) {
        func(results, str, size);
    }

    // release WMI COM interfaces
    results->lpVtbl->Release(results);
    services->lpVtbl->Release(services);
    locator->lpVtbl->Release(locator);

    // unwind everything else we've allocated
    CoUninitialize();

    SysFreeString(sysquery);
    SysFreeString(language);
    SysFreeString(resource);
}

void
wmi_os(IEnumWbemClassObject* results, char* str, gsize size) {
    HRESULT hr = 0;
    IWbemClassObject* result = NULL;
    ULONG returnedCount = 0;

    // enumerate the retrieved objects
    while (results->lpVtbl->Next(results, WBEM_INFINITE, 1, &result, &returnedCount) == S_OK) {
        VARIANT name;

        // obtain the desired properties of the next result and print them out
        hr = result->lpVtbl->Get(result, L"Caption", 0, &name, 0, 0);

        if (SUCCEEDED(hr)) {
            g_snprintf (str, size, "%ls", name.bstrVal);
            VariantClear(&name);
        }

        // release the current result object
        result->lpVtbl->Release(result);
    }
}

void
wmi_cpu(IEnumWbemClassObject *results, char *str, gsize size) {
    HRESULT hr = 0;
    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;

    // enumerate the retrieved objects
    while (results->lpVtbl->Next(results, WBEM_INFINITE, 1, &result, &returnedCount) == S_OK) {
        VARIANT name;

        // obtain the desired properties of the next result and print them out
        hr = result->lpVtbl->Get(result, L"Name", 0, &name, 0, 0);

        if (SUCCEEDED(hr)) {
            g_snprintf (str, size, "%ls", name.bstrVal);
            VariantClear(&name);
        }

        // release the current result object
        result->lpVtbl->Release(result);
    }
}

void
wmi_ram(IEnumWbemClassObject *results, char *str, gsize size) {
    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;
    HRESULT hrcapacity, hrspeed, hrmanufacturer;

    hrcapacity = hrspeed = hrmanufacturer = 0;

    // enumerate the retrieved objects
    while (results->lpVtbl->Next(results, WBEM_INFINITE, 1, &result, &returnedCount) == S_OK) {
        VARIANT capacity, speed, manufacturer;

        // obtain the desired properties of the next result and print them out
        hrcapacity = result->lpVtbl->Get(result, L"Capacity", 0, &capacity, 0, 0);
        hrspeed = result->lpVtbl->Get(result, L"Speed", 0, &speed, 0, 0);
        hrmanufacturer = result->lpVtbl->Get(result, L"Manufacturer", 0, &manufacturer, 0, 0);

        // SUCCEEDED(hr)
        gint64 out = _wtoi64 (capacity.bstrVal);
        g_snprintf (str, size, "%.2fGB %dMHz %ls\n", out / pow(2, 30), speed.uintVal, manufacturer.bstrVal);
        VariantClear(&capacity);
        VariantClear(&speed);
        VariantClear(&manufacturer);

        // release the current result object
        result->lpVtbl->Release(result);
    }

    str[strlen(str) > 0 ? strlen(str) - 1 : 0] = '\0';
}

void
wmi_disc(IEnumWbemClassObject *results, char *str, gsize size) {
    HRESULT hr = 0;
    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;

    // enumerate the retrieved objects
    while (results->lpVtbl->Next(results, WBEM_INFINITE, 1, &result, &returnedCount) == S_OK) {
        VARIANT prop;

        // obtain the desired properties of the next result and print them out
        hr = result->lpVtbl->Get(result, L"DeviceID", 0, &prop, 0, 0);
        if (SUCCEEDED(hr)) {
            g_snprintf (str, size, "%s %ls", str, prop.bstrVal);
            VariantClear(&prop);
        }

        hr = result->lpVtbl->Get(result, L"Size", 0, &prop, 0, 0);
        if (SUCCEEDED(hr)) {
            long long out = _wtoi64 (prop.bstrVal);
            g_snprintf (str, size, "%s %.2fGB", str, out / pow(2, 30));
            VariantClear(&prop);
        }

        result->lpVtbl->Release(result);
    }
}