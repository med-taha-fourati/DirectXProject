#pragma once

#include <vector>
#include <math.h>

#ifdef __cplusplus
	#include <complex>
	typedef std::complex<double> cplx;
#else
	#include <complex.h>
	typedef double complex cplx;
#endif

	void fft(cplx* x, size_t N);
	void processBuffer(cplx* signal, size_t signalSize, int bufferSize);
	int nextPowerOfTwo(int n);