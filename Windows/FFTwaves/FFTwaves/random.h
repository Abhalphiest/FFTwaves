#pragma once

//File: random
//Author: Margaret Dorsey
//
// The random files simply provide random numbers
// for use in FFT.


#include"complex.h"
#include<math.h>
#include<stdlib.h>
float uniformRandomVariable();
complex gaussianRandomVariable();