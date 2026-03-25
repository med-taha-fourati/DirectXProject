#include "IIDRedefinitions.h"
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
//#include "resource.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <strsafe.h>
#include <audioclient.h>
#include <assert.h>
#include "AudioHandler.hpp"
#include "BufferCapture.h"
#include <unordered_map>

std::vector<double> targetOther = { 75, 150, 400, 800, 1600, 4500, 10000, 22050 };


#define EXIT_ON_ERROR(hr)  \
              if (FAILED(hr)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }


static const auto GetDefaultPlaybackAudioDevice() {
	LPCWSTR x = L"null";
	auto hr = CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
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
	hr = CoCreateInstance(IID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
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

extern "C" LPWSTR __cdecl GetDeviceName(IMMDeviceCollection* deviceCollection, uint32_t DeviceIndex) {
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

extern "C" bool __cdecl PickDevice(IMMDeviceCollection* deviceCollection, HRESULT* hr, std::vector<LPCWSTR>* result) {
	uint32_t deviceCount = 0;

	*hr = deviceCollection->GetCount(&deviceCount);
	for (uint32_t i = 0; i < deviceCount; i += 1) {
		LPWSTR deviceName;

		deviceName = GetDeviceName(deviceCollection, i);

		result->push_back(deviceName);
	}

	return true;
}

extern "C" bool __cdecl PickDeviceFromIndex(IMMDeviceCollection* deviceCollection, HRESULT* hr, long index) {
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

extern "C" HRESULT __cdecl CaptureDevice(IMMDevice** recorder, IAudioClient** recorderClient, IAudioCaptureClient** capturer) {
	IMMDeviceEnumerator* enumerator = NULL;
	HRESULT hr = NULL;
	hr = CoCreateInstance(IID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_IMMDeviceEnumerator, (void**)&enumerator);
	//Optional
	WAVEFORMATEX* format = NULL;

	std::vector<LPCWSTR> devices = DeviceEnum();

	hr = enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, recorder);
	assert(SUCCEEDED(hr));
	hr = enumerator->Release();
	assert(SUCCEEDED(hr));
	hr = (*recorder)->Activate(IID_IAudioClient, 
							CLSCTX_ALL, 
							NULL, 
							(void**)recorderClient);
	assert(SUCCEEDED(hr));
	hr = (*recorderClient)->GetMixFormat(&format);
	assert(SUCCEEDED(hr));

	wchar_t bps[500];
	swprintf_s(bps, L"Bitrate per sample: %d\n", format->wBitsPerSample);
	OutputDebugString(bps);
	wchar_t nch[500];
	swprintf_s(nch, L"No of channels: %d\n", format->nChannels);
	OutputDebugString(nch);
	wchar_t nba[500];
	swprintf_s(nba, L"Nbr Blocks aligned: %d\n", format->nBlockAlign);
	OutputDebugString(nba);
	

	hr = (*recorderClient)->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0, // Stream Flags
		10000000, // Time to capture in nanoseconds
		0,
		format,
		NULL
	);

	hr = (*recorderClient)->GetService(IID_IAudioCaptureClient, (void**)capturer);
	assert(SUCCEEDED(hr));

	hr = (*recorderClient)->Start();
	assert(SUCCEEDED(hr));

	return hr;
	// ...and away it goes, see you on the loop
}

extern "C" void __cdecl StartCaptureLoop(
					std::vector<double>* output,
					BYTE** buffer,
					  DWORD flags,
					  uint32_t* nFrames,
					  IAudioCaptureClient** capturer, 
					  HRESULT* hr) {
	// ...aaaand we're back

	BYTE* tempBuffer = nullptr;
	DWORD captureFlags = 0;

	*hr = (*capturer)->GetBuffer(&tempBuffer, nFrames, &flags, nullptr, nullptr);
	assert(SUCCEEDED(*hr));

	if (FAILED(*hr)) {
		wchar_t err[128];
		swprintf_s(err, L"GetBuffer failed with HRESULT: 0x%08X\n", *hr);
		OutputDebugString(err);
		return;
	}

	/*wchar_t debug[256];
	swprintf_s(debug, L"GetBuffer SUCCESS - tempBuffer: %p, nFrames: %u, flags: 0x%08X\n",
		tempBuffer, *nFrames, captureFlags);
	OutputDebugString(debug);*/

	if (*nFrames == 0) {
		//OutputDebugString(L"No frames available\n");
		(*capturer)->ReleaseBuffer(0);
		return;
	}

	// Check if buffer pointer is valid
	if (tempBuffer == nullptr) {
		OutputDebugString(L"ERROR: tempBuffer is NULL!\n");
		(*capturer)->ReleaseBuffer(*nFrames);
		return;
	}

	_printByte(tempBuffer, *nFrames, 0);
	*output = captureWasapiData(*nFrames);
	for (size_t i = 0; i < output->size(); ++i) {
		wchar_t buf[256];
		swprintf_s(buf, L"Freq : %lf | Magnitude: %lf \n", targetOther[i], (*output)[i]);
		OutputDebugString(buf);
	}
	/*wchar_t buf[32];
	swprintf_s(buf, L"AmplitudeL : %f\n", getAmplitude(tempBuffer, 0));
	OutputDebugString(buf);*/

	*buffer = tempBuffer; // this must be released

	*hr = (*capturer)->ReleaseBuffer(*nFrames);
	assert(SUCCEEDED(*hr));

	// memcpy(renderBuffer, captureBuffer, format->nBlockAlign * nFrames); loopback thing
}

extern "C" void __cdecl StopRecorderService(IAudioClient** recorderClient,
	IAudioCaptureClient** capturer,
	IMMDevice** recorderDevice) {

	(*recorderClient)->Stop();
	(*capturer)->Release();
	(*recorderDevice)->Release();

	CoUninitialize();
}

