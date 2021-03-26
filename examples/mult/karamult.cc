//
// KARAMULT.CC Karatsuba multiplication in action
//


#include <iostream.h>
#include "types.h" // declarations of types Digit and CxDigit
#include "print.h" // prototypes of print()

// function prototypes:

// karatsuba(): basic karatsuba function 
void karatsuba(Digit *pR, Digit *pP, Digit *pQ, long n, Digit *pTmp); // in kara.cc
// getdigs(): read digits from stdin into array a[]
long getdigs(char *s, long *pbase, Digit a[], long d_max); // in getdigs.cc 

int main()
{
  cout << "\nDEMO OF THE KARATSUBA MULTIPLICATION\n";

  long          base  = 10;             // 100 and 1000 are poss. also
  const long    nab_max = 1024;         // max no of digits of each multplier
                                        // must be a power of 2  
  Digit         a[nab_max], b[nab_max]; // multipliers, incl. padded zeros
  long          na, nb;                 // length of the multipliers
  
  while ( 1 )  // main loop
  {
  // 0) Read the two multipliers
        na = getdigs("Multiplicand ", &base, a, nab_max);
        if (na == 0) break;

        nb = getdigs("Multiplicator", &base, b, nab_max);
        if (nb == 0) break;

  // 1) Find length values
        long nab;             // length of zero padded operands
                              // = smallest power of 2 >= max(na, nb)
        for (nab = 1; nab < na || nab < nb; nab += nab)  {;}
        long nr = 2*nab;      // length of result array r = 2*n
        long nc = na + nb;    // nc = length of final product a*b

        cout << "Base=" << base
             << ", na="  << na  << ", nb=" << nb 
             << ", nab=" << nab << ", nr=" << nr << endl;

        for (long k=na; k<nab; ++k)  a[k] = 0;     // zero padding
        for (long k=nb; k<nab; ++k)  b[k] = 0;     // zero padding

        print("Multiplicand                             : ", a, na);
        print("Multiplicator                            : ", b, nb);

  // 2) Perform karatsuba convolution
        Digit *r = new Digit[nr];                  // result array
        karatsuba(r, a, b, nab, NULL);
        print("Result after plain karatsuba             : ", r, nc);

  // 3) Perform carry operation
        for (long k=nc-1; k >= 1; --k)
        {
          Digit cy  = r[k] / base;
          r[k]     -= cy * base;
          r[k-1]   += cy;
        }
        print("Final Product (after carry operation)    : ", r, nc);
        cout << endl;
  }
  return 0;
}


