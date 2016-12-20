#pragma once
#ifndef FFT_H
#define FFT_H

//File: FFT
//Author: Margaret Dorsey
//
// FFT files provide a class that performs a fast fourier transform.
// Based on source code by Keith Lantz.

#include <math.h>
#include "complex.h"
//#define M_PI 3.14159265359

class FFT {
private:
	unsigned int N, which;
	unsigned int log_2_N;
	float pi2;
	unsigned int *reversed; //our bitwise reversed index
	complex **T;
	complex *c[2];
protected:
public:
	//the constructor allocates resources, precomputes values,
	//and gets our bitwise reversed index
	FFT(unsigned int N);
	//releases our allocated memory
	~FFT();
	//reverses our index bitwise
	unsigned int reverse(unsigned int i);
	//compute function for FFT
	complex t(unsigned int x, unsigned int N);
	//actually performs the fast fourier transform
	void fft(complex* input, complex* output, int stride, int offset);
};

#endif
