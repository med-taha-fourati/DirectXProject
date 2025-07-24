#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "resource.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#define EXIT_ON_ERROR(hr)  \
              if (FAILED(hr)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

static const auto DeviceEnum() {
	LPCWSTR x = L"null";
	CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	//EXIT_ON_ERROR(hr);
	IMMDevice* defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);

	IPropertyStore* pProps = NULL;
	hr = defaultDevice->OpenPropertyStore(STGM_READ, &pProps);

	PROPVARIANT varName;
	PropVariantInit(&varName);
	hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);

	if (SUCCEEDED(hr)) {
		if (varName.vt == VT_LPWSTR) {
			x = varName.pwszVal;
		}
	}
	else {
		std::cerr << "GetValue failed: " << hr << std::endl;
	}
	return x;
}