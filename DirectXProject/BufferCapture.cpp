#include "BufferCapture.h"
#include "fft.h"
#include <unordered_map>

int32_t* samplesGlobale = nullptr;

void _printByte(BYTE* byte, uint32_t nFrames, uint32_t i) {
	wchar_t x[1024];
	// i hope "casting" this isnt gonna break the application
	int32_t* samples = reinterpret_cast<int32_t*>(byte);
	swprintf_s(x, L"Length: %d | Sample: %d\n", nFrames, samples[(i*2)+1]);
	OutputDebugString(x);
	samplesGlobale = samples;
}

float getAmplitude(BYTE* byte, uint32_t i) {
	wchar_t x[1024];
	int32_t* samples = reinterpret_cast<int32_t*>(byte);
	return std::abs(samples[i*2]) / 2147483647.0f; // MAXINT32;//
}

void captureWasapiData(uint32_t nFrames) {
	size_t signalSize = nFrames;
	short numChannels = 2;

	cplx* complexSignal = new cplx[signalSize];
	for (size_t i = 0; i < signalSize; ++i) {
		float monoSample = 0.0f;
		for (int ch = 0; ch < numChannels; ++ch) {
			monoSample += samplesGlobale[i * numChannels + ch];
		}
		monoSample /= numChannels;

		complexSignal[i] = cplx(monoSample, 0.0);
	}

	std::unordered_map<double, double> hiiii;

	processBuffer(&hiiii, complexSignal, signalSize, 1024);

	delete[] complexSignal;
}