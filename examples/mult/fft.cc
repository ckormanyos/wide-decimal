//
// FFT.CC
//
// Basic fft function
//

#include <iostream.h>
#include <math.h>
#include <Complex.h> // fft()

// Basic type of a complex number
typedef complex<double> Complex;

/*******************************************************************************

                          fft()

 fft() : fast fourier transform
         (radix2, decimation in frequency)
 isign   determines whether forward (> 0) or backward (< 0) transform
 n       length of array f, must be a power of 2

*******************************************************************************/

void fft(Complex f[], long n, int isign)
{
  double pi = isign >= 0 ? +M_PI : -M_PI;

  for (long m=n; m > 1; m /= 2)  
  {
    long mh = m / 2;
    double phi = pi / mh;

    for (long j=0; j < mh; ++j)
    {
      double  p  = phi * j;
      Complex cs = Complex(cos(p), sin(p));

      for (long t1=j; t1 < j+n; t1 += m) {
        Complex u = f[t1];
        Complex v = f[t1+mh];
        f[t1] = u+v;
        f[t1+mh] = (u-v) * cs;
      }
    }
  }

  // data reordering:
  for (long m=1, j=0; m < n-1; ++m)
  {
    for (long k=n>>1; (!((j^=k)&k)); k>>=1)  {;}
    if ( j > m ) // SWAP(f[m], f[j]);
    {    
      Complex t = f[m];
           f[m] = f[j];
           f[j] = t;
    }
  }
  if ( isign < 0 )  // normalise if backwards transform
    for (long k=0; k < n; ++k)  f[k] /= n;
}


