//
// KARA.CC
//
// Basic Karatsuba functions
//

#include <assert.h>  // assert
#include <limits.h>  // CHAR_BIT
#include <string.h>  // memmove, memset

#include "types.h"   // typedef of Digit

#include "print.h"

void  karatsuba (Digit *r, Digit *u, Digit *v, long n, Digit *t);
Digit add       (Digit *a, long sizeA, Digit *b, long sizeB);
Digit subtract  (Digit *a, long sizeA, Digit *b, long sizeB);
void  complement(Digit *a, long sizeA);

/*******************************************************************************

                          add()

  Computes A += B modulo 2^(32*sizeA)
  sizeA must be >= sizeB must be >= 1
  Returns the last carry

*******************************************************************************/

Digit add(Digit *a, long sizeA, Digit *b, long sizeB)
{
  assert(sizeA >= 1);
  assert(sizeA >= sizeB);

  Digit t, cy = 0;
  Digit *x   = a+sizeA;
  Digit *y   = b+sizeB;
  do { // add the right parts
    t   = *--x;
    cy += *--y;
    *x = t += cy;
  	 cy = t < cy;
  } while (y != b);
  // add cy to the left part of A
  while (cy && --x >= a)
    cy = ++*x == 0;
  return cy;
}

/*******************************************************************************

                          subtract()

  Computes A -= B modulo 2 ^ (32*sizeA)
  sizeA must be >= sizeB must be >= 1
  Returns Carry.

*******************************************************************************/

Digit subtract(Digit *a, long sizeA, Digit *b, long sizeB)
{
  assert(sizeA >= 1);
  assert(sizeA >= sizeB);

  Digit t, cy = 0;
  Digit *x  = a+sizeA;
  Digit *y  = b+sizeB;
  // Subtract the right parts of A and B
  do {
    t   = *--x;
    cy += *--y;
    *x = t - cy;
  	cy  = t < cy;
  } while (y != b);
  // Subtract carry from left part of A
  while (cy && --x >= a)
    cy = (*x)-- == 0;
  return cy;
}

/*******************************************************************************

                          complement()

  Complements A by converting bits, then adding 1

*******************************************************************************/

void complement(Digit *a, long sizeA)
{
  Digit cy  = 1;
  Digit *x = a + sizeA;

	do {
   	*--x ^= ~0;
   	if (cy)
     	cy = ++(*x) == 0;
  } while (x != a);
  return;
}

/*******************************************************************************

                          digitmul()

  Multiplies two single Digits to a pair of Digits

*******************************************************************************/

static void digitmul(Digit *r, const Digit *u, const Digit *v)
{
	static Digit BETA_HALF = sizeof(Digit)*CHAR_BIT / 2;
  static Digit GAMMA = ~Digit(0);
  static Digit GAMMA_LOW = GAMMA >> BETA_HALF;

	const Digit c  = *u >> BETA_HALF;
  const Digit d  = *u & GAMMA_LOW;
  const Digit e  = *v >> BETA_HALF;
  const Digit f  = *v & GAMMA_LOW;
  const Digit z1 = c * f;
        Digit z2 = c * e;
        Digit z3 = d * f;
        Digit z4 = d * e + z1;
  if (z4 < z1)
   	z2 += GAMMA_LOW+1;
  z2 += z4 >> BETA_HALF;
  z3 += z4 <<= BETA_HALF;
  if (z3 < z4) ++z2;
  *r = z2; 
  *++r = z3; 
  return;
}

/*******************************************************************************

                          karatsuba()

  Performs Karatsuba-Multiplication of the Digit sequences P and Q
  and stores the resulting product into R.

  P and Q have length n, R has length 2*n.
  n must be a power of 2.

  P, Q, and R are absolute values.

  Uses a temorary storage t of length 2*n_max.
  This storage will be allocated only once during 1st call.

  Returns void.

*******************************************************************************/

void karatsuba(Digit *r, Digit *u, Digit *v, long n, Digit *t)
{

  if (n == 1) { // conventional multiplication
     digitmul(r, u, v);
     return;
  }

  long n2 = n+n;
  long nh = n/2;
  long n3h = 3*nh;
  Digit cy_u10, cy_v01;

  // The temporary storage is handled sequentially
  // The length of all tmp storage is 4*n
  if (t == NULL)
    t = new Digit[4*n];
  karatsuba(r, u, v, nh, t);          // u1*v1 -> r
  karatsuba(r+n, u+nh, v+nh, nh, t);  // u0*v0 -> r+n
  memcpy(t, r, n2*sizeof(Digit));     // r->t
  add(r, n3h, t, n);                  // r..r+3nh += u1*v1
  add(r, n3h, t+n, n);                // r..r+3nh += u0*v0
  memcpy(t, u, nh*sizeof(Digit));     // u1 -> t
  cy_u10 = subtract(t, nh, u+nh, nh); // u1-u0 -> t
  if (cy_u10)                         // |u1-u0| -> t  
    complement(t, nh);                
  memcpy(t+nh,v+nh, nh*sizeof(Digit));// v0 -> t+nh  
  cy_v01 = subtract(t+nh, nh, v, nh); // v0-v1 -> t+nh  
  if (cy_v01)                         // |v0-v1| -> t+nh
    complement(t+nh, nh);
  karatsuba(t+n, t, t+nh, nh, t+n2);  // |u1-u0|*|v0-v1| -> t+n
  if (cy_u10 == cy_v01)               // r..r+3nh +/- |u1-u0|*|v0-v1|
    add(r, n3h, t+n, n);
  else
    subtract(r, n3h, t+n, n);
  return;
}

