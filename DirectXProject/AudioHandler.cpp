#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "resource.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <strsafe.h>
#include <audioclient.h>
#include <assert.h>

#define EXIT_ON_ERROR(hr)  \
              if (FAILED(hr)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

static const auto GetDefaultPlaybackAudioDevice() {
	LPCWSTR x = L"null";
	auto hr = CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
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

static const auto GetDefaultRecordingAudioDevice() {
	LPCWSTR x = L"null";
	auto hr = CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	//EXIT_ON_ERROR(hr);

	IMMDevice* defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &defaultDevice);

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

	hr = deviceEnumerator->Release();
	hr = pProps->Release();

	return x;
}

LPWSTR GetDeviceName(IMMDeviceCollection* deviceCollection, uint32_t DeviceIndex) {
	IMMDevice* device;
	LPWSTR deviceId;
	HRESULT hr;

	hr = deviceCollection->Item(DeviceIndex, &device);
	hr = device->GetId(&deviceId);

	IPropertyStore* propertyStore;
	hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
	device->Release();

	PROPVARIANT friendlyName;
	PropVariantInit(&friendlyName);

	hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);

	wchar_t deviceName[128];
	hr = StringCbPrintf(deviceName, sizeof(deviceName),
		L"%s (%s)", friendlyName.vt != VT_LPWSTR ? L"Unknown" : friendlyName.pwszVal, deviceId);

	return deviceName;
}

bool PickDevice(IMMDeviceCollection* deviceCollection, HRESULT* hr, std::vector<LPCWSTR>* result) {
	uint32_t deviceCount = 0;

	*hr = deviceCollection->GetCount(&deviceCount);
	for (uint32_t i = 0; i < deviceCount; i += 1) {
		LPWSTR deviceName;

		deviceName = GetDeviceName(deviceCollection, i);

		result->push_back(deviceName);
	}

	return true;
}

bool PickDeviceFromIndex(IMMDeviceCollection* deviceCollection, HRESULT* hr, long index) {
	IMMDevice* device = nullptr;
	*hr = deviceCollection->Item(index, &device);

	return true;
}

static const auto DeviceEnum() {
	std::vector<LPCWSTR> deviceNames = {};

	LPCWSTR x = L"null";
	auto hr = CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	//EXIT_ON_ERROR(hr);
	assert(SUCCEEDED(hr));

	IMMDeviceCollection* deviceCollection = NULL;

	hr = deviceEnumerator->EnumAudioEndpoints(eCapture,
		DEVICE_STATE_ACTIVE,
		&deviceCollection);

	PickDevice(deviceCollection, &hr, &deviceNames);

	return deviceNames;
}

HRESULT CaptureDevice(IMMDevice* recorder, IAudioClient* recorderClient, IAudioCaptureClient* capturer) {
	IMMDeviceEnumerator* enumerator = NULL;
	HRESULT hr = NULL;

	//Optional
	WAVEFORMATEX* format = NULL;

	std::vector<LPCWSTR> devices = DeviceEnum();

	hr = enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &recorder);
	assert(SUCCEEDED(hr));
	hr = enumerator->Release();
	assert(SUCCEEDED(hr));
	hr = recorder->Activate(IID_IAudioClient, 
							CLSCTX_ALL, 
							NULL, 
							(void**)&recorderClient);
	assert(SUCCEEDED(hr));
	hr = recorderClient->GetMixFormat(&format);
	assert(SUCCEEDED(hr));

	hr = recorderClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0, // Stream Flags
		10000000, // Time to capture in nanoseconds
		0,
		format,
		NULL
	);

	hr = recorderClient->GetService(IID_IAudioCaptureClient, (void**)&capturer);
	assert(SUCCEEDED(hr));

	hr = recorderClient->Start();
	assert(SUCCEEDED(hr));

	return hr;
	// ...and away it goes, see you on the loop
}

void StartCaptureLoop(BYTE* buffer, 
					  DWORD flags,
					  uint32_t* nFrames,
					  IAudioCaptureClient* capturer, 
					  HRESULT* hr) {
	// ...aaaand we're back

	*hr = capturer->GetBuffer(&buffer, nFrames, &flags, nullptr, nullptr);
	*hr = capturer->ReleaseBuffer(*nFrames);

	// memcpy(renderBuffer, captureBuffer, format->nBlockAlign * nFrames); loopback thing
}

void StopRecorderService(IAudioClient* recorderClient,
	IAudioCaptureClient* capturer,
	IMMDevice* recorderDevice) {

	recorderClient->Stop();
	capturer->Release();
	recorderDevice->Release();

	CoUninitialize();
}

