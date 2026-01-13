#include "BufferCapture.h"

int32_t* samplesGlobale = nullptr;

void _printByte(BYTE* byte, uint32_t nFrames, uint32_t i) {
	wchar_t x[1024];
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