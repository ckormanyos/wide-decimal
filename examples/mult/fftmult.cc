//
// Fast Fourier Transform (FFT) multiplication in action
//

#include <iostream.h>
#include <Complex.h>

#include "types.h"      // declaration of types Digit and CxDigit
#include "print.h"      // prototypes of print()

// function prototypes:

// fft(): fast fourier transform 
void fft(CxDigit f[], long n, int isign);               // in fft.cc

// getdigs(): read digits from stdin into array a[]
long getdigs(char *s, long *pbase, Digit a[], long d_max); // in getdigs.cc 

int main()
{
  cout << "\nDEMO OF THE FAST FOURIER TRANSFORM MULTIPLICATION\n";

  long  base  = 10;                   // you can safely change it to 100 or 1000
  const long  nab_max = 1000;         // max no of digits of each multplier
  Digit       a[nab_max], b[nab_max]; // operand arrays (real)
  
  while ( 1 )  // main loop
  {
  // 0) Read the two multipliers
        long na = getdigs("Multiplicand ", &base, a, nab_max);
        if (na == 0) break;

        long nb = getdigs("Multiplicator", &base, b, nab_max);
        if (nb == 0) break;

  // 1) Find length values
        long nc = na + nb;  // length of final product a*b
        long nf;            // length of complex arrays fa, fb
                            // = smallest power-of-2 >= nc
                            // when sft(): letting nf = nc; is sufficient
        for (nf = 2; nf < nc; nf += nf)  {;}

        cout << "Base=" << base
             << ", na=" << na << ", nb=" << nb 
             << ", nc=" << nc << ", nf=" << nf << endl;
        print("Real Multiplicand                        : ", a, na);
        print("Real Multiplicator                       : ", b, nb);

  // 2) Copy a[] to complex array fa[], also b[] to fb[].
        // Justify left, pad with zeros to the right:
        CxDigit *fa = new CxDigit[nf];
        for (long k=0;  k < na; ++k)  fa[k] = a[k];
        for (long k=na; k < nf; ++k)  fa[k] = 0;
        print("Complex Multiplicand  (zero padded right): ", fa, nf);

        CxDigit *fb = new CxDigit[nf];
        for (long k=0 ; k < nb; ++k)  fb[k] = b[k];
        for (long k=nb; k < nf; ++k)  fb[k] = 0;
        print("Complex Multiplicator (zero padded right): ", fb, nf);

  // 3) Perform fast fourier transforms forward on fa[] and fb[]

        fft(fa, nf, +1);
        print("Complex Multiplicand  (after forward FFT): ", fa, nf);

        fft(fb, nf, +1);
        print("Complex Multiplicator (after forward FFT): ", fb, nf);

  // 4) Multiply elementwise: fa[k] = fa[k] * fb[k]
        for (long k=0; k < nf; ++k)  fa[k] *= fb[k];
        print("After elementwise complex multiplication : ", fa, nf);

  // 5) Perform fast fourier transform back (incl. normalisation):

        fft(fa, nf, -1);
        print("Complex Product (after backwards FFT)    : ", fa, nf);

  // 6) Copy result in fa[] to the real array c[]:
        Digit *c = new Digit[nc];
        for (long k=0; k < nc; ++k) c[k] = (Digit)(fa[k].real()+0.5);
        print("Real Product                             : ", c, nc);

  // 7) Shift right by one; the least significant digit is c[nc-2]
        for (long k=nc-2; k>=0; --k)  c[k+1] = c[k];
        c[0] = 0;
        print("After right shift by one                 : ", c, nc);

  // 8) carry operation:
        for (long k = nc-1; k >= 1; --k)
        {
          Digit carry  = c[k] / base;
          c[k]       -= carry * base;
          c[k-1]     += carry;
        }
        print("Final Product (after carry operation)    : ", c, nc);
        cout << endl;
        delete [] fa, fb;
        delete [] c;
  }
  return 0;
}

