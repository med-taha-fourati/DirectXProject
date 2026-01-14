#include "fft.h"

constexpr double M_PI = 3.14159265358979323846;

void fft(cplx* x, size_t N) {
	if (N <= 1) return;
	// Divide
	std::vector<cplx> even(N / 2);
	std::vector<cplx> odd(N / 2);
	for (size_t i = 0; i < N / 2; ++i) {
		even[i] = x[i * 2];
		odd[i] = x[i * 2 + 1];
	}
	// Conquer
	fft(even.data(), N / 2);
	fft(odd.data(), N / 2);
	// Combine
	for (size_t k = 0; k < N / 2; ++k) {
		cplx t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}


	// idk if this is necessary but yea i put it for safety
	delete[] even.data();
	delete[] odd.data();
}

void processBuffer(cplx* signal, size_t signalSize, int bufferSize) {
	int numBuffers = (signalSize + bufferSize - 1) / bufferSize;

	for (int buf_idx = 0; buf_idx < numBuffers; ++buf_idx) {
		//TODO: continue this later
	}
}