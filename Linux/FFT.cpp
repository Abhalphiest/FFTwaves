#include "FFT.h"

//File: FFT
//Author: Margaret Dorsey
//
// FFT files provide a class that performs a fast fourier transform.
// Based on source code by Keith Lantz.

//the constructor allocates resources, precomputes values,
//and gets our bitwise reversed index
FFT::FFT(unsigned int N) : N(N), reversed(0), T(0), pi2((float)(2 * M_PI)) {
	c[0] = c[1] = 0;

	log_2_N = (unsigned int)( log(N) / log(2));

	reversed = new unsigned int[N];		// prep bit reversals
	for (unsigned int i = 0; i < N; i++) reversed[i] = reverse(i);

	int pow2 = 1;
	T = new complex*[log_2_N];		// prep T
	for (unsigned int i = 0; i < log_2_N; i++) {
		T[i] = new complex[pow2];
		for (int j = 0; j < pow2; j++) T[i][j] = t(j, pow2 * 2);
		pow2 *= 2;
	}

	c[0] = new complex[N];
	c[1] = new complex[N];
	which = 0;
}

//releases our allocated memory
FFT::~FFT() {
	if (c[0]) delete[] c[0];
	if (c[1]) delete[] c[1];
	if (T) {
		for (unsigned int i = 0; i < log_2_N; i++) if (T[i]) delete[] T[i];
		delete[] T;
	}
	if (reversed) delete[] reversed;
}

//reverses our index bitwise
unsigned int FFT::reverse(unsigned int i) {
	unsigned int res = 0;
	for (unsigned int j = 0; j < log_2_N; j++) {
		res = (res << 1) + (i & 1);
		i >>= 1;
	}
	return res;
}


//compute function for FFT
complex FFT::t(unsigned int x, unsigned int N) {
	return complex((float)cos(pi2 * x / N), (float)sin(pi2 * x / N));
}

//actually performs the fast fourier transform
void FFT::fft(complex* input, complex* output, int stride, int offset) {
	for (unsigned int i = 0; i < N; i++) c[which][i] = input[reversed[i] * stride + offset];

	int loops = N >> 1;
	int size = 1 << 1;
	int size_over_2 = 1;
	int w_ = 0;
	for (unsigned int i = 1; i <= log_2_N; i++) {
		which ^= 1;
		for (int j = 0; j < loops; j++) {
			for (int k = 0; k < size_over_2; k++) {
				c[which][size * j + k] = c[which ^ 1][size * j + k] +
					c[which ^ 1][size * j + size_over_2 + k] * T[w_][k];
			}

			for (int k = size_over_2; k < size; k++) {
				c[which][size * j + k] = c[which ^ 1][size * j - size_over_2 + k] -
					c[which ^ 1][size * j + k] * T[w_][k - size_over_2];
			}
		}
		loops >>= 1;
		size <<= 1;
		size_over_2 <<= 1;
		w_++;
	}

	for (unsigned int i = 0; i < N; i++) output[i * stride + offset] = c[which][i];
}