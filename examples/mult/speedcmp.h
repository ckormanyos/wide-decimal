//
// SPEEDCMP.H
//
// Declarations for SPEEDCMP.CC
//

#include <iostream.h>
#include <math.h>
#include <stdio.h>   // stdin
#include <Complex.h> // fft()
#include <time.h>    // timer functions
#include "types.h"   // typedef's of Complex and Digit
#include "print.h"

// in fft.cc
void fft(CxDigit f[], long n, int isign);

// in kara.cc
void karatsuba(Digit *r, Digit *u, Digit *v, long n, Digit *t);

