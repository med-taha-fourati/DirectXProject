#include "fft.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <unordered_map>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define SAMPLE_RATE 44100 

std::vector<double> targets = { 30, 67, 75, 150, 400, 800, 1600, 4500, 10000, 22050 };

constexpr int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) power *= 2;
    return power;
}

void fft(cplx* x, size_t N) {
    if (N <= 1) return;

    std::vector<cplx> even(N / 2);
    std::vector<cplx> odd(N / 2);

    for (size_t i = 0; i < N / 2; ++i) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    fft(even.data(), N / 2);
    fft(odd.data(), N / 2);

    for (size_t k = 0; k < N / 2; ++k) {
        cplx t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}

void processBuffer(std::unordered_map<double, double>* output, cplx* signal, size_t signalSize, int bufferSize) {
    int numBuffers = (signalSize + bufferSize - 1) / bufferSize;
    int paddedSize = nextPowerOfTwo(bufferSize);
    cplx* buffer = new cplx[paddedSize];

    for (int buf_idx = 0; buf_idx < numBuffers; ++buf_idx) {
        size_t startIdx = buf_idx * static_cast<size_t>(bufferSize);
        size_t endIdx = std::min(startIdx + bufferSize, signalSize);
        size_t currentLen = endIdx - startIdx;

        for (size_t i = 0; i < paddedSize; ++i) {
            if (i < currentLen) {
                buffer[i] = signal[startIdx + i];
            }
            else {
                buffer[i] = cplx(0, 0);
            }
        }

        fft(buffer, paddedSize);

        for (double targetFreq : targets) {
            int binIdx = std::round((targetFreq * paddedSize) / SAMPLE_RATE);

            if (binIdx >= 0 && binIdx < (paddedSize / 2)) {
                double magnitude = std::abs(buffer[binIdx]);
                double phase = std::arg(buffer[binIdx]);
                std::cout << targetFreq << "Hz (Bin " << binIdx << "): Mag = "
                    << magnitude << ", Phase = " << phase << std::endl;

                output->insert({ targetFreq, magnitude });
            }
        }
    }

    delete[] buffer;
}