//
// GETDIGS.CC - getdigs()
//

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#ifdef __GNUC__
#define strnicmp strncasecmp
#endif
#include "types.h"  // typedef of type Digit


/*******************************************************************************
  
  long getdigs(char *s, long *pbase, Digit a[], long n)
  
  Reads up to <n> digits of base *pbase from a single line of stdin
  into the array a[] of Digit's.

  Returns the number of digits read.

  If the input-device is a console then a requesting message like
  "Enter the digits of the <s> : "
  will be printed first.
  
  The first line may be begin with base=<base>. The <base> will be
  stored to pbase and used furtheron.
  
  If *pbase > 10, a digit will be composed of ceil(log10(base))
  adjacent decimal digit-chars on stdin, unless a non-digit char
  comes earlier.
  (E.g.: no hexits like A-F for 10-16 will be used if base=16.)

  The digits may be preceded or separated by space, tab or comma.
  They are ended by a \n, EOF, non-digit char or a digit >= base.
  The rest of the line will be skipped.

  If !console, the line may be preceded by empty lines.
   
  Ignored: blanks or \t's before the first digit.
           empty lines before the first digit, unless console.
           blanks, \t's or commas between digits
  Examples:
  base=10,  input:3 1 4 1 \n    gives 3 1 4 1
  base=100, input:31415\n       gives 31 41 05
  base=1000,input:987\t6,54\n   gives 987 006 54
  any base, input:3.14\n        gives 3, .14 is skipped
  base=100, input:#             bypassed
                  base=64       sets new base 64
                  636465        gives 63, because 64 >= new base
  
*******************************************************************************/

long getdigs(char *s, long *pbase, Digit a[], long m_max)
{
  long         m = 0; // digits read
  static int   isconsole = isatty(fileno(stdin)) != 0;
  char         line[10000]; // should suffice
  char         *p = line;

  if (isconsole)
    printf("Enter the digits of the %s : ", s);

  while (1) {
    *line = '\0';
    if (fgets(p=line, sizeof(line), stdin) == NULL) break;
    // bypass whitespace before the first digit
    while (isspace(*p)) ++p;
    // bypass comment lines (they begin with ; or #)
    if (*p == '#' || *p == ';')
      continue;
    // Ask base=
    if (strnicmp(p, "base=", 5) == 0) {
      sscanf(p+=5, "%ld", pbase);
      continue;
    }
    // bypass empty lines if !console input   
    if (*p || isconsole)
      break;
  }
  // read and build the digits
  for (m=0; m < m_max; ++m) 
  {
    if (!isdigit(*p)) break;
    a[m] = 0;
    for(long b=*pbase; b > 1; b /= 10) {
      a[m] = a[m]*10 + *p - '0';
      if (!isdigit(*++p)) break;
    }
    if (a[m] >= (Digit)*pbase)
      break;
    // skip commas and whitespace after digit
    while (*p == ',' || isspace(*p))
      ++p;
  }
  return m;
}

