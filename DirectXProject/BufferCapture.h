#pragma once
#include <iostream>
#include <mmdeviceapi.h>
#include <unordered_map>

extern int32_t* samplesGlobale;
void _printByte(BYTE* byte, uint32_t nFrames, uint32_t i);
float getAmplitude(BYTE* byte, uint32_t i);
std::unordered_map<double, double> captureWasapiData(uint32_t nFrames);