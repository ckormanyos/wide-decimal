
#include <iostream.h>
#include <math.h>

#include "print.h"

void print(const char *bla, Digit a[], long n)
{
    cout << bla;
    for (long k=0; k<n; ++k)  cout << a[k] << " ";
    cout << endl;
}
// --------------------------

double chop(double v)
// used for nicer printing of doubles:
// values close to zero are set to zero
{
    if ( fabs(v)<1.0e-6 )  v = 0.0;
    return v;
}
// --------------------------

CxDigit chop(CxDigit v)
{
    return CxDigit(chop(v.real()),chop(v.imag()));
}
// --------------------------

void print(const char *bla, CxDigit f[], long n)
{
    cout << bla;
    for (long k=0; k<n; ++k)  cout << chop(f[k]) << " ";
    cout << endl;
}
// --------------------------
