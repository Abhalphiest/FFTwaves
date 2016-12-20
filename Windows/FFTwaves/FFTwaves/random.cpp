#include"random.h"

//File: random
//Author: Margaret Dorsey
//
// The random files simply provide random numbers
// for use in FFT.

float uniformRandomVariable() {
	return (float)rand() / RAND_MAX; //the most boring random number function imaginable
}

//gaussian random for complex
//based on code by Keith Lantz
complex gaussianRandomVariable() {
	float x1, x2, w;
	do {
		x1 = 2.f * uniformRandomVariable() - 1.f;
		x2 = 2.f * uniformRandomVariable() - 1.f;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.f);
	w = (float)sqrt((-2.f * log(w)) / w);
	return complex(x1 * w, x2 * w);
}