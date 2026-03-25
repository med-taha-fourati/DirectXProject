#pragma once
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
#include <unordered_map>

static const auto GetDefaultPlaybackAudioDevice();
extern "C" LPWSTR __cdecl GetDeviceName(IMMDeviceCollection* deviceCollection, uint32_t DeviceIndex);
extern "C" bool __cdecl PickDeviceFromIndex(IMMDeviceCollection* deviceCollection, HRESULT* hr, long index);
static const auto DeviceEnum();
extern "C" HRESULT __cdecl CaptureDevice(IMMDevice** recorder, IAudioClient** recorderClient, IAudioCaptureClient** capturer);
extern "C" void __cdecl StartCaptureLoop(
	std::vector<double>* output,
	BYTE** buffer,
	DWORD flags,
	uint32_t* nFrames,
	IAudioCaptureClient** capturer,
	HRESULT* hr);
extern "C" void __cdecl StopRecorderService(IAudioClient** recorderClient,
	IAudioCaptureClient** capturer,
	IMMDevice** recorderDevice);
