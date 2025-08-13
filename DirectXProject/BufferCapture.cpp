#include "BufferCapture.h"

void _printByte(BYTE* byte, uint32_t nFrames, uint32_t i) {
	wchar_t x[1024];
	int32_t* samples = reinterpret_cast<int32_t*>(byte);
	swprintf_s(x, L"Length: %d | Sample: %d\n", sizeof(samples), samples[i]);
	OutputDebugString(x);
}

float getAmplitude(BYTE* byte, uint32_t i) {
	wchar_t x[1024];
	int32_t* samples = reinterpret_cast<int32_t*>(byte);
	return std::abs(samples[i])/ 32768.0f;
}