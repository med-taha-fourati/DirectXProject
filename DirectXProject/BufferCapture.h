#pragma once
#include <iostream>
#include <mmdeviceapi.h>

void _printByte(BYTE* byte, uint32_t nFrames, uint32_t i);
float getAmplitude(BYTE* byte, uint32_t i);