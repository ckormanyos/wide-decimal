//
// SPEEDCMP: SPEED COMPARISON : FFT- vs KARATSUBA vs DIRECT multiplication
//

#include <stdio.h>
#include <unistd.h> // isatty()
#include "speedcmp.h"

/*******************************************************************************

                          direct_multiplication()

 Performs a direct multiplication of two Digit sequences a[] and b[]
 and stores the resulting product (after normalization to base <base>) in r[],

 a,b have length n, r has length 2*n.
 n must not necessarily be a power of 2.
 
 Returns void

*******************************************************************************/

void direct_multiplication(Digit r[], Digit a[], Digit b[], long n, Digit base)
{
  long n2 = n+n;
  
  for (long k=0; k<n2; ++k)
  {
    Digit s = 0;
    for (long i=0; i <= k; ++i)
      if (i < n && k-i < n)
        s += a[i]*b[k-i];
    r[k] = s;
  }
  // normalization + shift-right by 1
  Digit carry = 0;
  for (long k=n2-2; k >= 0; --k)
  {
     Digit t      = r[k] + carry;
     carry        = t / base;
     r[k+1]       = t - carry * base;
  }
  r[0] = carry;
  return;
}

/*******************************************************************************

                          fft_multiplication()

 Performs a fast fourier transform multiplication of two Digit sequences a[] and b[]
 and stores the resulting product (after normalization to base <base>) in r[],

 a,b have length n, r has length 2*n.
 n must noc necessarily be a power of 2.
 
 Calls fft() in fft.cc

 Returns void 

*******************************************************************************/

void fft_multiplication(Digit r[], Digit a[], Digit b[], long n, Digit base)
{
    long n2 = n+n;
    long nf;
    for (nf=2; nf < n2; nf+=nf)  ; 

    CxDigit *fa = new CxDigit[nf];
    CxDigit *fb = new CxDigit[nf];
    for (long k=0; k<n; ++k)  fa[k] = a[k];  // copy to complex array
    for (long k=n; k<nf; ++k) fa[k] = 0;     // zero padding
    for (long k=0; k<n; ++k)  fb[k] = b[k];
    for (long k=n; k<nf; ++k) fb[k] = 0;
    fft(fa,nf,+1);                           // fast fourier transforms forward
    fft(fb,nf,+1);
    for (long k=0; k<nf; ++k)  fa[k] *= fb[k]; // multiply elementwise
    fft(fa,nf,-1);                          // fast fourier transform back
    // Copy back, shift right by 1 
    r[0] = 0;
    for (long k=0; k<n2-1; ++k)  r[k+1] = (Digit)(fa[k].real()+0.5);
    // carry operation
    for (long k = n2-1; k >= 1; --k)
    {
       long carry  = r[k] / base;
       r[k]       -= carry * base;
       r[k-1]     += carry;
    }
    delete[] fa, fb;
    return;
}
// --------------------------

/*******************************************************************************

                          karatsuba_multiplication()

 Performs a karatsuba multiplication of two Digit sequences a[] and b[]
 and stores the resulting product (after normalization to base <base>) in r[],

 a,b have length n, r has length 2*n.
 n must not necessarily be a power of 2.
 
 Calls karatsuba() in kara.cc

 Returns void


*******************************************************************************/

void karatsuba_multiplication(Digit r[], Digit a[], Digit b[], long n, Digit base)
{
    long nab, nc = n+n;
    for (nab=2; nab < n; nab+=nab)  ; // nab := smallest power-of-2 >= n

    Digit *a1 = new Digit[nab];
    Digit *b1 = new Digit[nab];
    Digit *r1 = new Digit[2*nab];
    for (long k=0; k<n; ++k)   a1[k] = a[k];  
    for (long k=n; k<nab; ++k) a1[k] = 0;     // zero padding
    for (long k=0; k<n; ++k)   b1[k] = b[k];  
    for (long k=n; k<nab; ++k) b1[k] = 0;     // zero padding
    karatsuba(r1, a1, b1, nab, NULL);
    // Copy to r1 and unify
    Digit cy = 0;
    for (long k=nc-1; k >= 0; --k)
    {
      Digit t   = r1[k] + cy;
      cy        = t / base;
      r[k]      = t - cy * base;
    }
    
    delete [] a1;
    delete [] b1;
    delete [] r1;
    return;
}
// --------------------------

int main()
{
  const  long   base = 10;
         double t_limit = 30.0;// time limit for each multiplication
  const  long   ldn_max = 18;  // maxn = 2**ldn_max
         long   n_max = (1<<ldn_max);
         int    isconsole = (isatty(fileno(stdin)) != 0);
         int    ifirst    = 1;
         double t0, t1;        // timer values
         double tf_fft = 0,    // time factor, initially = 0  
                tf_karatsuba = 0,
                tf_direct = 0;

  printf("\nSPEED COMPARISON:\n");

  while ( 1 )
  {
    char line[100];
    long n;
    
    if (isconsole)
      printf("Enter length of multipliers (0 to exit): ");
    
    if (fgets(line, sizeof(line), stdin) == NULL)  break;
    if (sscanf(line, "limit=%lf", &t_limit) == 1)
      continue;
    if (sscanf(line, "%ld", &n) == 0)  break;
    if ( n <=0 )                break;
    if ( n > n_max )            n = n_max;
    
    if (ifirst)
       printf("Length        FFT mult  KARATSUBA mult     DIRECT mult\n");
    ifirst = 0;
    Digit *a = new Digit[n];       // input arrays
    Digit *b = new Digit[n];    
    Digit *rfft = new Digit[n+n];  // result array of fftmult
    Digit *rkar = new Digit[n+n];  // result array of karatsuba mult
    Digit *rdir = new Digit[n+n];  // result array of direct mult

    for (long k=0; k<n; ++k)  a[k] = 9;  // a[] = 9999999...
    for (long k=0; k<n; ++k)  b[k] = 7;  // b[] = 7777777...

    printf("%6ld", n);
    if (tf_fft * n * log10(n) > t_limit) 
      printf("       >%5.1f sec", t_limit);
    else 
    {
      t0 = clock();
      fft_multiplication(rfft, a, b, n, base);
      t1 = (clock()-t0)/CLOCKS_PER_SEC;
      printf("%12.1f sec",  t1);
      // compute time factor
      tf_fft = t1 / n / log10(n);
    }
    if (tf_karatsuba * pow(n, 1.58) > t_limit)
      printf("      >%5.1f sec", t_limit);
    else 
    {
      t0 = clock();
      karatsuba_multiplication(rkar, a, b, n, base);
      t1 = (clock() - t0) / CLOCKS_PER_SEC;
      printf("%12.1f sec",  t1);
      // compute time factor
      tf_karatsuba = t1 / pow(n, 1.58);
    }
    
    if (tf_direct * pow(n, 2) > t_limit) 
      printf("      >%5.1f sec", t_limit);
    else 
    {
      t0 = clock();
      direct_multiplication(rdir, a, b, n, base);
      t1 = (clock() - t0) / CLOCKS_PER_SEC;
      printf("%12.1f sec",  t1);
    
      // compute time factor
      tf_direct = t1/pow(n,2);

      // check correctness
      long err = 0;
      for (long k=0; k < n+n; ++k) 
      {
        if (rfft[k] != rdir[k]) err = 1;
        if (rkar[k] != rdir[k]) err = 1;
      }
      if (err)
          printf("   !!!There were errors!!!");
    }
    printf("\n");
    delete [] rfft;
    delete [] rkar;
    delete [] rdir;
    delete [] a, b;
  }
  return 0;
}
// --------------------------

