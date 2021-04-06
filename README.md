Wide-decimal
[![Build Status](https://github.com/ckormanyos/wide-decimal/actions/workflows/wide_decimal.yml/badge.svg)](https://github.com/ckormanyos/wide-decimal/actions)
==================

Wide-decimal implements a generic C++ template for extended precision decimal float types.

This C++ template header-only library implements drop-in big decimal float types,
such as `dec101_t`, `dec1001_t`, `dec10001_t`, `dec1000001_t`, etc.,
that can be used essentially like regular built-in floating-point types.
Wide-decimal supports decimal float types having digit counts ranging
roughly from about
<img src="https://render.githubusercontent.com/render/math?math=\sim\,10{\ldots}10,000,000">.

Wide-decimal implements both common algebraic operations as well as
a few common `<cmath>`-like functions such as `fabs`, `sqrt` and `log`,
and also includes full support for `std::numeric_limits`.

Wide-decimal is written in header-only C++11, and compatible through C++11, 14, 17, 20.

## Implementation goals

  - Wide precision range up to one million decimal digits
  - Moderately good efficiency over the entire wide precision range
  - Clean header-only C++11 design
  - Seamless portability to any modern C++11, 14, 17, 20 compiler
  - Scalability with small memory footprint and efficiency suitable for _bare-metal_ embedded systems

## Quick start

Easy application follows via a traditional C-style typedef or C++11 alias.
The defined type can be used very much like a built-in floating-point type.

The following sample, for instance, defines and uses a decimal type
called `dec101_t` having
<img src="https://render.githubusercontent.com/render/math?math=101">
decimal digits of precision.
The subroutine `do_something()` initializes the variable `d`
of type `dec101_t` with 1/3 and subsequently prints the 101 digit
value of `d` to the console.

In particular,

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

The local data type `dec101_t` is defined with a C++11 alias.
The first template parameter `101U` sets the decimal digit
count while the second optional template parameter `std::uint32_t`
sets the internal _limb_ _type_. If the second template parameter is left blank,
the default limb type is 32 bits in width and unsigned.

`decwide_t` also has a third (and a few more) optional template paramter(s).
The third template parameter
can be used to set the allocator type for internal storage of the
wide decimal type. The default allocator type is
`std::allocator<limb_type>` which helps to reduce
stack consumption, especially when using higher digit counts.
If low digit counts are used, the allocator type can
be explicitly set to `void`
and stack allocation is used with an `array`-like
internal representation.

## Examples

Various interesting and algorithmically challenging
[examples](./examples) have been implemented.
It is hoped that the examples provide inspiration and guidance on
how to use wide-decimal.

  - ![`example000_multiply_nines.cpp`](./examples/example000_multiply_nines.cpp) performa a check of shoolbook multiplication using a small digit range and small 8-bit limb.
  - ![`example000a_multiply_pi_squared.cpp`](./examples/example000a_multiply_pi_squared.cpp) performs a hard-coded multiplication check resulting in <img src="https://render.githubusercontent.com/render/math?math=\pi^2">.
  - ![`example001_roots_sqrt.cpp`](./examples/example001_roots_sqrt.cpp) computes a square root.
  - ![`example001a_roots_seventh.cpp`](./examples/example001a_roots_seventh.cpp) computes a seventh root.
  - ![`example001b_roots_almost_integer.cpp`](./examples/example001b_roots_almost_integer.cpp) uses roots and algebraic operations to compute <img src="https://render.githubusercontent.com/render/math?math=5,001"> decimal digits of a fascinating [Pisot number](https://mathworld.wolfram.com/PisotNumber.html) that is [almost integer](https://mathworld.wolfram.com/AlmostInteger.html).
  - ![`example001c_roots_sqrt_limb08.cpp`](./examples/example001c_roots_sqrt_limb08.cpp) computes a square root with a wide decimal representation having 8-bit limbs.
  - ![`example001d_pow2_from_list.cpp`](./examples/example001d_pow2_from_list.cpp) verifies a list of values <img src="https://render.githubusercontent.com/render/math?math=2^n"> with <img src="https://render.githubusercontent.com/render/math?math=-128\,\le\,n\,\le\,127">.
  - ![`example002_pi.cpp`](./examples/example002_pi.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a Gauss AGM iteration.
  - ![`example002a_pi_small_limb.cpp`](./examples/example002a_pi_small_limb.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a 16-bit internal limb type.
  - ![`example002b_pi_100k.cpp`](./examples/example002b_pi_100k.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=100,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi">.
  - ![`example002c_pi_quintic.cpp`](./examples/example002c_pi_quintic.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a Borwein quintic iteration.
  - ![`example002d_pi_limb08.cpp`](./examples/example002d_pi_limb08.cpp) calculates yet again <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using an 8-bit internal limb type and `float` internal floating-point type.
  - ![`example003_zeta.cpp`](./examples/example003_zeta.cpp) computes a Riemann zeta function value.
  - ![`example004_bessel_recur.cpp`](./examples/example004_bessel_recur.cpp) implements cylindrical Bessel functions of integral order via downward recursion with a Neumann sum.
  - ![`example005_polylog_series.cpp`](./examples/example005_polylog_series.cpp) performs a small-argument polylogarithm series calculation.
  - ![`example006_logarithm.cpp`](./examples/example006_logarithm.cpp) calculates the value of a logarithm (internally using a Gauss AGM method).
  - ![`example007_catalan_series.cpp`](./examples/example007_catalan_series.cpp) computes <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digits of Catalan's constant using an accelerated series.
  - ![`example008_bernoulli_tgamma.cpp`](./examples/example008_bernoulli_tgamma.cpp) implements `tgamma(x)` using Stirling's asymptotic expansion of the logarithm of the Gamma function with Bernoulli numbers and subsequently calculates <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\Gamma(n/2)"> for small integer <img src="https://render.githubusercontent.com/render/math?math=n">.
  - ![`example009_boost_math_standalone.cpp`](./examples/example009_boost_math_standalone.cpp) checks basic compatibility of standalone `decwide_t` with `Boost.Math` by testing a cube root value obtained from `boost::math::cbrt`.
  - ![`example009a_boost_math_standalone.cpp`](./examples/example009a_boost_math_standalone.cpp) also checks standalone `decwide_t` with significantly more of `Boost.Math` by testing a <img src="https://render.githubusercontent.com/render/math?math=1,001"> digit generalized Legendre function value (using `boost::math::tgamma` and more to do so).
  - ![`example010_hypergeometric_2f1.cpp`](./examples/example010_hypergeometric_2f1.cpp) calculates a <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digit hypergeometric function value using an iterative rational approximation scheme.
  - ![`example010a_hypergeometric_1f1.cpp`](./examples/example010a_hypergeometric_1f1.cpp) calculates another <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digit hypergeometric function in a similar fashion.
  - ![`example011_trig_trapezoid_integral.cpp`](./examples/example011_trig_trapezoid_integral.cpp) uses trapezoid integration with an integral representation involving locally-written trigonometric sine and cosine functions to compute several cylindrical Bessel function values.

## Testing and CI

Testing is a big issue and a growing test suite is in continued progress
providing for tested, efficient functionality on the PC and workstation.
The GitHub code is delivered with an affiliated MSVC project that uses easy-to-understand
subroutines called from `main()` that exercise various test cases.

Continuous integration runs on push using GitHub Actions.
Various compilers, operating systems, and C++ standards
ranging from C++11, 14, 17, 20 are included in CI.

### Build Status
[![Build Status](https://github.com/ckormanyos/wide-decimal/actions/workflows/wide_decimal.yml/badge.svg)](https://github.com/ckormanyos/wide-decimal/actions)

## Details

Wide-Decimal has been tested with numerous compilers for target systems ranging from 8 to 64 bits.
The library is specifically designed for modest efficiency (not the world's fastest)
over the entire range of small to large digit counts.
How efficient is the code? Based on a very general
comparison, a million digit AGM calculation
of <img src="https://render.githubusercontent.com/render/math?math=\pi">
is about five times slower than an equivalent calculation
performed with a big float data type based on
[GMP](https://gmplib.org/).

Portability of the code is another key point of focus. Special care
has been taken to test in certain high-performance embedded real-time
programming environments.

When working with even the most tiny microcontroller systems,
various _heavy-wieght_ features such as I/O streaming,
dynamic memory allocation,
construction from character string
and caching constant values such as
<img src="https://render.githubusercontent.com/render/math?math=\pi">
and
<img src="https://render.githubusercontent.com/render/math?math=\log(2)">
can optionally be disabled with the compiler switches:

```
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS
```

Each one of these compiler switches has an intuitive name
intended to represent its meaning.

Note: Activating the option `WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION`
simultaneously disallows using `decwide_t` in a multithreaded application.
So if PC-based or other kinds of multithreading are used, then dynamic memory
allocation is needed and can not be disabled. In other words,

```
// Deactivate the disabling of dynamic memory for multithreaded PC work.
//#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
```

## Detailed examples

The example below calculates the square root of the decimal representation of
<img src="https://render.githubusercontent.com/render/math?math=\sqrt{1234.56}">,
the result of which is approximately
<img src="https://render.githubusercontent.com/render/math?math=35.136306009596398663933384640418055759751518287169314528165976164717710895452890928635031219132220\ldots">.

```
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <math/wide_decimal/decwide_t.h>

int main()
{
  using local_limb_type = std::uint16_t;

  using dec101_t = math::wide_decimal::decwide_t<101U, local_limb_type>;

  const dec101_t s = sqrt(dec101_t(123456U) / 100);

  // N[Sqrt[123456/100], 101]
  const dec101_t control
  {
    "35.136306009596398663933384640418055759751518287169314528165976164717710895452890928635031219132220978"
  };

  const dec101_t closeness = fabs(1 - (s / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec101_t>::epsilon() * 10);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}
```

In the following code, we compute <img src="https://render.githubusercontent.com/render/math?math=1,000,001">
(one million and one) decimal digits of the fundamental constant
<img src="https://render.githubusercontent.com/render/math?math=\pi">.
The truncated (non-rounded) expected result is
<img src="https://render.githubusercontent.com/render/math?math=3.14159265\,\ldots\,79458151">.
In this particular example, all _heavy-weight_ components are deactivated and
this particular calculation is, in fact, suitable for a _bare-metal_ mega-digit pi calculation.

In this example, note how a specialized custom allocator called
`util::n_slot_array_allocator` is utilized for exact, efficient memory management
of a certain number of temporary storages of mega-digit numbers
(tuned to 18 in this particular example).

```
#include <iomanip>
#include <iostream>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

int main()
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>;

  using local_wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double>;

  const std::clock_t start = std::clock();

  const local_wide_decimal_type my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>(nullptr);

  const std::clock_t stop = std::clock();

  std::cout << "Time example002_pi(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_32.size(),
                                     math::constants::const_pi_control_head_32.begin());

  using const_iterator_type = typename local_wide_decimal_type::array_type::const_iterator;

  const_iterator_type fi(my_pi.crepresentation().cbegin() + (std::uint32_t) (  (std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
                                                                             - (std::uint32_t) math::constants::const_pi_control_tail_32_1000001.size()));

  const bool tail_is_ok = std::equal(fi,
                                     fi + math::constants::const_pi_control_tail_32_1000001.size(),
                                          math::constants::const_pi_control_tail_32_1000001.begin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}
```

The million digit run is comparatively slow and requires approximately
10 seconds on a modern PC. Considering, however, the design goals
of header-only and capable of running on bare-metal, this is
a very nice calculational result.

## 1,000,001 digits of pi on a bare metal microcontroller

The wide-decimal float back end is used to compute
<img src="https://render.githubusercontent.com/render/math?math=1,000,001">
decimal digits of the mathematical constant
<img src="https://render.githubusercontent.com/render/math?math=\pi">
on selected bare-metal OS-less microcontroller systems in
[pi-crunch-metal](https://github.com/ckormanyos/pi-crunch-metal)
