#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "resource.h"
#define EXIT_ON_ERROR(hr)  \
              if (FAILED(hr)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

static const auto DeviceEnum() {
	CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	//EXIT_ON_ERROR(hr);
	IMMDevice* defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	return hr;
}