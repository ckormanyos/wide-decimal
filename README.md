# Wide-decimal
Wide-decimal implements a generic C++ template for extended precision decimal float types.

This C++ template header-only library implements drop-in big decimal float types such as `dec101_t`, `dec10001_t`, `dec1000001_t`, etc., that can be used essentially like regular built-in floating-point types. Wide-decimal supports decimal float types having digit counts ranging from about <img src="https://render.githubusercontent.com/render/math?math=30{\ldots}2,000,000">.

Wide-decimal features realizations of a few `<cmath>`-like anc C++-like functions and features such as `fabs`, `sqrt` and `std::numeric_limits>`.

Wide-decimal is written in header-only C++11.

# Details
Wide-Decimal has been tested with numerous compilers for target systems ranging from 8 to 64 bits.
The library is specifically designed for modest efficiency (not the world's fastest)
over the entire range of small to large digit counts.

Portability of the code is another key point of focus. Special care
has been taken to test in certain high-performance embedded real-time
programming environments.

Testing is a big issue. Generating a full test suite is in continued progress
and tested efficient functionality on the PC and workstation is present.
The code is delivered with an affiliated MSVC project that uses Boost.Test
for various test cases.

When working with even the most tiny microcontroller systems,
various _heavy-wieght_ features such as I/O streaming,
conversion to and from built-in floating-point types
and construction from character string
can optionally be disabled with the compiler switches:

```
define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
```

See the examples directory as more use cases are being created.

# Quick start
Easy application follows via a traditional C-style typedef or C++11 alias.
The defined type can be used very much like a built-in unsinged integral type.

For instance,

```
#include <iomanip>
#include <iostream>

#include <math/wide_decimal/decwide_t.h>

void do_something()
{
  using dec101_t = math::wide_decimal::decwide_t<101U, std::uint32_t>;

  dec101_t d = dec101_t(1) / 3;

  // 0.33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
  std::cout << std::setprecision(std::numeric_limits<dec101_t>::digits) << d << std::endl;
}
```

The code sequence above defines the local data type `dec101_t` with
a C++11 alias. The first template parameter `101U` sets the decimal digit
count while the second optional template parameter `std::uint32_t`
sets the internal _limb_ _type_. If the second template parameter is left blank,
the default limb type is 32 bits in width and unsigned.
The subroutine `do_something()` initializes the `dec101_t` named `d`
with the representation of 1/3 and prints its 101 digit value to the console.

# Detailed examples
