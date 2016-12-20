#pragma once
#ifndef COMPLEX_H
#define COMPLEX_H

// File: complex
// Author:	Margaret Dorsey
//
// The complex files provide an implementation of complex numbers.
// Basically straightforward, has all expected operations.
// Based on code by Keith Lantz.
//

class complex {
private:
protected:
public:
	float a, b; //a + bi, real and imaginary parts of our number respectively
	static unsigned int additions, multiplications; //counts number of additions and multiplications made with this number
													//useful for FFT
	complex(); //init to 0
	complex(float a, float b); //init to imaginary number
	complex conj(); //complex conjugation
	complex operator*(const complex& c) const; //multiplication
	complex operator+(const complex& c) const; //addition
	complex operator-(const complex& c) const; //subtraction
	complex operator-() const; //negation
	complex operator*(const float c) const; //scalar mult
	complex& operator=(const complex& c); //assignment
	//resets addition and multiplication counters
	static void reset();
};

#endif