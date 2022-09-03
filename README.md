Wide-decimal
==================

<p align="center">
    <a href="https://github.com/ckormanyos/wide-decimal/actions">
        <img src="https://github.com/ckormanyos/wide-decimal/actions/workflows/wide_decimal.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/ckormanyos/wide-decimal/issues">
        <img src="https://img.shields.io/github/issues-raw/ckormanyos/wide-decimal" alt="Open Issues" /></a>
    <a href="https://lgtm.com/projects/g/ckormanyos/wide-decimal/context:cpp">
        <img src="https://img.shields.io/lgtm/grade/cpp/g/ckormanyos/wide-decimal.svg?logo=lgtm&logoWidth=18" alt="Language grade: C/C++"></a>
    <a href="https://lgtm.com/projects/g/ckormanyos/wide-decimal/alerts/">
        <img src="https://img.shields.io/lgtm/alerts/g/ckormanyos/wide-decimal.svg?logo=lgtm&logoWidth=18" alt="Total alerts" /></a>
    <a href="https://scan.coverity.com/projects/ckormanyos-wide-decimal">
        <img src="https://scan.coverity.com/projects/24792/badge.svg" alt="Coverity Scan"></a>
    <a href="https://sonarcloud.io/summary/new_code?id=ckormanyos_wide-decimal">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=ckormanyos_wide-decimal&metric=alert_status" alt="Quality Gate Status"></a>
    <a href="https://www.codacy.com/gh/ckormanyos/wide-decimal/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ckormanyos/wide-decimal&amp;utm_campaign=Badge_Grade">
        <img src="https://app.codacy.com/project/badge/Grade/3f3d6624f20e4b53b95de69202cfe297"/></a>
    <a href="https://codecov.io/gh/ckormanyos/wide-decimal">
        <img src="https://codecov.io/gh/ckormanyos/wide-decimal/branch/main/graph/badge.svg?token=9SEP7RLNSO" alt="code coverage"></a>
    <a href="https://github.com/ckormanyos/wide-decimal/blob/master/LICENSE_1_0.txt">
        <img src="https://img.shields.io/badge/license-BSL%201.0-blue.svg" alt="Boost Software License 1.0"></a>
    <a href="https://img.shields.io/github/commit-activity/y/ckormanyos/wide-decimal">
        <img src="https://img.shields.io/github/commit-activity/y/ckormanyos/wide-decimal" alt="GitHub commit activity" /></a>
    <a href="https://github.com/ckormanyos/wide-decimal">
        <img src="https://img.shields.io/github/languages/code-size/ckormanyos/wide-decimal" alt="GitHub code size in bytes" /></a>
</p>

Wide-decimal implements a generic C++ template for extended precision decimal float types.

This C++ template header-only library implements drop-in big decimal float types,
such as `dec51_t`, `dec101_t`, `dec1001_t`, `dec10001_t`, `dec1000001_t`, etc.,
that can be used essentially like regular built-in floating-point types.
Wide-decimal supports decimal float types having digit counts ranging
roughly from about ${\sim}10{\ldots}10,000,000$

Wide-decimal implements both common algebraic operations as well as
a few common `<cmath>`-like functions such as `fabs`, `sqrt` and `log`,
and also includes full support for `std::numeric_limits`.

Wide-decimal is written in header-only C++11, and compatible through C++11, 14, 17, 20, 23.

## Implementation goals

  - Wide precision range up to one million decimal digits
  - Moderately good efficiency over the entire wide precision range
  - Clean header-only C++11 design
  - Seamless portability to any modern C++11, 14, 17, 20, 23 compiler
  - Scalability with small memory footprint and efficiency suitable for _bare-metal_ embedded systems

## Quick start

Easy application follows via a traditional C-style typedef or C++11 alias.
The defined type can be used very much like a built-in floating-point type.

The following sample, for instance, defines and uses a decimal type
called `dec101_t` having $101$ decimal digits of precision.
In this example, the subroutine `do_something()` initializes the variable `d`
of type `dec101_t` with $\frac{1}{3}$. The $101$ digit value of `d`
is subsequently printed to the console. The console precision is set with
`std::setprecision` in combination with `dec101_t`'s specialization
of `std::numeric_limits`.

In particular,

```cpp
#include <iomanip>
#include <iostream>

#include <math/wide_decimal/decwide_t.h>

void do_something()
{
  using dec101_t = math::wide_decimal::decwide_t<INT32_C(101), std::uint32_t, void>;

  dec101_t d = dec101_t(1) / 3;

  // 0.33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
  std::cout << std::setprecision(std::numeric_limits<dec101_t>::digits) << d << std::endl;
}
```

The local data type `dec101_t` is defined with a C++11 alias.
The first template parameter `INT32_C(101)` sets the decimal digit
count while the second optional template parameter `std::uint32_t`
sets the internal _limb_ _type_. If the second template parameter is left blank,
the default limb type is 32 bits in width and unsigned.

The template signature of the `decwide_t` class is shown below.

```cpp
// Forward declaration of the decwide_t template class.
template<const std::int32_t ParamDigitsBaseTen,
         typename LimbType          = std::uint32_t,
         typename AllocatorType     = std::allocator<void>,
         typename InternalFloatType = double,
         typename ExponentType      = std::int64_t,
         typename FftFloatType      = double>
class decwide_t;
```

`decwide_t` also has a third (and a few more) optional template paramter(s).
The third template parameter `AllocatorType`
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

The examples include the following.

  - ![`example000_multiply_nines.cpp`](./examples/example000_multiply_nines.cpp) performa a check of shoolbook multiplication using a small digit range and small 8-bit limb.
  - ![`example000a_multiply_pi_squared.cpp`](./examples/example000a_multiply_pi_squared.cpp) performs a hard-coded multiplication check resulting in $\pi^2$.
  - ![`example001_roots_sqrt.cpp`](./examples/example001_roots_sqrt.cpp) computes a square root.
  - ![`example001a_roots_seventh.cpp`](./examples/example001a_roots_seventh.cpp) computes a seventh root.
  - ![`example001b_roots_almost_integer.cpp`](./examples/example001b_roots_almost_integer.cpp) uses roots and algebraic operations to compute $5,001$ decimal digits of a fascinating [Pisot number](https://mathworld.wolfram.com/PisotNumber.html) that is [almost integer](https://mathworld.wolfram.com/AlmostInteger.html).
  - ![`example001c_roots_sqrt_limb08.cpp`](./examples/example001c_roots_sqrt_limb08.cpp) computes a square root with a wide decimal representation having 8-bit limbs.
  - ![`example001d_pow2_from_list.cpp`](./examples/example001d_pow2_from_list.cpp) verifies a list of values $2^n$ with $-128{\le}n{\le}127$.
  - ![`example002_pi.cpp`](./examples/example002_pi.cpp) calculates $1,000,001$ decimal digits of $\pi$ using a Gauss AGM iteration.
  - ![`example002a_pi_small_limb.cpp`](./examples/example002a_pi_small_limb.cpp) calculates $1,000,001$ decimal digits of $\pi$ using a 16-bit internal limb type.
  - ![`example002b_pi_100k.cpp`](./examples/example002b_pi_100k.cpp) calculates $100,001$ decimal digits of $\pi$.
  - ![`example002c_pi_quintic.cpp`](./examples/example002c_pi_quintic.cpp) calculates $1,000,001$ decimal digits of $\pi$ using a Borwein quintic iteration.
  - ![`example002d_pi_limb08.cpp`](./examples/example002d_pi_limb08.cpp) calculates yet again $1,000,001$ decimal digits of $\pi$ using an 8-bit internal limb type and `float` internal floating-point type.
  - ![`example003_zeta.cpp`](./examples/example003_zeta.cpp) computes a Riemann zeta function value.
  - ![`example004_bessel_recur.cpp`](./examples/example004_bessel_recur.cpp) implements cylindrical Bessel functions of integral order via downward recursion with a Neumann sum.
  - ![`example005_polylog_series.cpp`](./examples/example005_polylog_series.cpp) performs a small-argument polylogarithm series calculation.
  - ![`example006_logarithm.cpp`](./examples/example006_logarithm.cpp) calculates the value of a logarithm (internally using a Gauss AGM method).
  - ![`example007_catalan_series.cpp`](./examples/example007_catalan_series.cpp) computes $1,001$ decimal digits of Catalan's constant using an accelerated series.
  - ![`example008_bernoulli_tgamma.cpp`](./examples/example008_bernoulli_tgamma.cpp) implements `tgamma(x)` using Stirling's asymptotic expansion of the logarithm of the Gamma function with Bernoulli numbers and subsequently calculates $1,001$ decimal digits of $\Gamma(n/2)$ for small integer $n$.
  - ![`example009_boost_math_standalone.cpp`](./examples/example009_boost_math_standalone.cpp) checks basic compatibility of standalone `decwide_t` with `Boost.Math` by testing a cube root value obtained from `boost::math::cbrt`.
  - ![`example009a_boost_math_standalone.cpp`](./examples/example009a_boost_math_standalone.cpp) also checks standalone `decwide_t` with significantly more of `Boost.Math` by testing a $1,001$ digit generalized Legendre function value (using `boost::math::tgamma` and more to do so).
  - ![`example009b_boost_math_standalone.cpp`](./examples/example009b_boost_math_standalone.cpp) checks yet again standalone `decwide_t` with `Boost.Math`'s available `boost::math::tgamma` function for small-ish decimal floats having $\lesssim100$ decimal digits.
  - ![`example010_hypergeometric_2f1.cpp`](./examples/example010_hypergeometric_2f1.cpp) calculates a $1,001$ decimal digit hypergeometric function value using an iterative rational approximation scheme.
  - ![`example010a_hypergeometric_1f1.cpp`](./examples/example010a_hypergeometric_1f1.cpp) calculates another $1,001$ decimal digit hypergeometric function in a similar fashion.
  - ![`example011_trig_trapezoid_integral.cpp`](./examples/example011_trig_trapezoid_integral.cpp) uses trapezoid integration with an integral representation involving locally-written trigonometric sine and cosine functions to compute several cylindrical Bessel function values.
  - ![`example012_rational_floor_ceil.cpp`](./examples/example012_rational_floor_ceil.cpp) verifies the proper representation of a wide selection of small-valued, pure integral rational quotients.
  - ![`example013_embeddable_sqrt.cpp`](./examples/example013_embeddable_sqrt.cpp) and ![`example013a_embeddable_agm.cpp`](./examples/example013a_embeddable_agm.cpp) exercise calculations that also run on tiny bare-metal embedded systems, featuring a $101$ digit square root calculation and a $53$ digit AGM iteration for $\pi$.

## Testing and CI

Testing is a big issue and a growing test suite is in continued progress
providing for tested, efficient functionality on the PC and workstation.
The GitHub code is delivered with an affiliated MSVC project that uses easy-to-understand
subroutines called from `main()` that exercise various test cases.

Continuous integration runs on push using GitHub Actions.
Various compilers, operating systems, and C++ standards
ranging from C++11, 14, 17, 20, 23 are included in CI.
An assortment of syntax checkers including enhanced GCC warnings,
`clang-tidy` and various third party tools run in and
also in association with CI and pull requests.
Badges can be found above.

### Build status
[![Build Status](https://github.com/ckormanyos/wide-decimal/actions/workflows/wide_decimal.yml/badge.svg)](https://github.com/ckormanyos/wide-decimal/actions)

## Additional details

Wide-Decimal has been tested with numerous compilers for target systems ranging from 8 to 64 bits.
The library is specifically designed for modest efficiency (not the world's fastest)
over the entire range of small to large digit counts.
How efficient is the code? Based on a very general
comparison, a million digit AGM calculation of
$\pi$
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
$\pi$
and
$\log(2)$
can optionally be disabled with the compiler switches:

```cpp
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS
#define WIDE_DECIMAL_DISABLE_USE_STD_FUNCTION
#define WIDE_DECIMAL_NAMESPACE
```

Each one of these compiler switches has an intuitive name
intended to represent its meaning.

Note: Activating the option `WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION`
simultaneously disallows using `decwide_t` in a multithreaded application.
So if PC-based or other kinds of multithreading are used, then dynamic memory
allocation is needed and can not be disabled. In other words,

```cpp
// Activate/Deactivate the disable of dynamic memory.
// For most multithreaded PC work, comment out or remove
// this line entirely (i.e., thereby enable dynamic memory).

#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
```

Let's consider also the macro `WIDE_DECIMAL_NAMESPACE` in greater detail.

```cpp
#define WIDE_DECIMAL_NAMESPACE
```

This is an advanced macro intended to be used in strict, exacting applications for which
using the unqualified, global namespace `math` (i.e., `namespace` `::math`) is undesired or inacceptable.
We recall that all parts of the wide-decimal implementation,
such as the `decwide_t` class and its associated implementation
details reside within `namespace` `::math::wide_decimal`

Consider defining the macro `WIDE_DECIMAL_NAMESPACE` to be a self-chosen,
unique namespace name, for instance something like

```sh
-DWIDE_DECIMAL_NAMESPACE=something_unique
```

This places all parts of the wide-decimal's `uintwide_t` template class implementation
and its associated details within the prepended outer namespace
`something_unique` - as in

```cpp
namespace something_unique::math::wide_decimal
{
  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType          = std::uint32_t,
           typename AllocatorType     = std::allocator<void>,
           typename InternalFloatType = double,
           typename ExponentType      = std::int64_t,
           typename FftFloatType      = double>
  class decwide_t;

  // And also all its related types and functions.
}
```

When utilizing the `WIDE_DECIMAL_NAMESPACE` option,
it is easy to vary the actual name or nesting depth of the desired prepended
outer namespace if/as needed for your particular project.

By default the macro `WIDE_DECIMAL_NAMESPACE` is not defined.
In this default state, `namespace` `::math::wide_decimal` is used
and the `decwide_t` class and its associated implementation
details reside therein.

## Examples

### Basic square root

The example below performs a basic square root calculation,
given by approximately

$$\sqrt{\frac{123456}{100}} \approx 35.1363060095963986639333846404180557597515{\ldots}$$

```cpp
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <math/wide_decimal/decwide_t.h>

auto main() -> int
{
  using local_limb_type = std::uint16_t;

  using dec101_t = math::wide_decimal::decwide_t<INT32_C(101), local_limb_type, void>;

  const dec101_t s = sqrt(dec101_t(123456U) / 100);

  // N[Sqrt[123456/100], 101]
  const dec101_t control
  {
    "35.136306009596398663933384640418055759751518287169314528165976164717710895452890928635031219132220978"
  };

  const dec101_t closeness = fabs(1 - (s / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<dec101_t>::epsilon() * 10));

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}
```

### 1,000,001 digits of pi

In the following code, we compute $1,000,001$
(one million and one) decimal digits of the fundamental constant
$\pi$
The truncated (non-rounded) expected result is
$3.14159265{\ldots}79458151$
In this particular example, all _heavy-weight_ components are deactivated and
this particular calculation is, in fact, suitable for a _bare-metal_ mega-digit pi calculation.

In this example, note how a specialized custom allocator called
`util::n_slot_array_allocator` is utilized for exact, efficient memory management
of a certain number of temporary storages of mega-digit numbers
(tuned to 18 in this particular example).

```cpp
#include <iomanip>
#include <iostream>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

auto main() -> int
{
  using local_limb_type = std::uint32_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>;

  using local_wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double>;

  const auto start = std::clock();

  const local_wide_decimal_type my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>(nullptr);

  const auto stop = std::clock();

  std::cout << "Time example002_pi(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  const auto head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_32.size(),
                                     math::constants::const_pi_control_head_32.begin());

  using const_iterator_type = typename local_wide_decimal_type::array_type::const_iterator;

  const_iterator_type
    fi
    (
        my_pi.crepresentation().cbegin()
      + static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
          - static_cast<std::uint32_t>(math::constants::const_pi_control_tail_32_1000001.size())
        )
    );

  const auto tail_is_ok = std::equal(fi,
                                     fi + math::constants::const_pi_control_tail_32_1000001.size(),
                                          math::constants::const_pi_control_tail_32_1000001.begin());

  const auto result_is_ok = (head_is_ok && tail_is_ok);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}
```

The million digit run is comparatively slow and requires approximately
10 seconds on a modern PC. Considering, however, the design goals
of header-only and capable of running on bare-metal, this is
a very nice calculational result.

### 1,000,001 digits of pi on a bare metal microcontroller

The wide-decimal float back end is used to compute
$1,000,001$
decimal digits of the mathematical constant
$\pi$
on selected bare-metal OS-less microcontroller systems in
[pi-crunch-metal](https://github.com/ckormanyos/pi-crunch-metal)

## Further details

### Mixing wide-decimal with wide-integer

[Wide-decimal](https://github.com/ckormanyos/wide-decimal)
can be used simultaneously with
[wide-integer](https://github.com/ckormanyos/wide-integer).
Although this was not primarily foreseen in the designs
of these libraries, harmonized mixing can be done in the same project,
and even in the same file.
This can, however, lead to a conflicting multiple definition
of the container class `util::dynamic_array<>`, as has been shown in
[issue 166](https://github.com/ckormanyos/wide-decimal/issues/166).

In order to use the
[`uintwide_t.h`](https://github.com/ckormanyos/wide-integer/tree/master/math/wide_integer)
header together with the
[`decwide_t.h`](https://github.com/ckormanyos/wide-integer/tree/master/math/wide_integer)
header in the same translation unit, use the
definition shown below.

```cpp
#define WIDE_INTEGER_DISABLE_IMPLEMENT_UTIL_DYNAMIC_ARRAY
```

Ensure that this definition appears _above_ the line that `#include`s
the `uintwide_t.h` header.
Do this for all files including both big-number headers.
Alternatively, this preprocessor
switch can be defined on the command line of the compiler call(s)
for the project.

The rational for this is as follows. The helper-container template class
`util::dynamic_array<>` is used in both the
[`decwide_t_detail.h`](https://github.com/ckormanyos/wide-decimal/tree/main/math/wide_decimal)
header (secondarily included in `decwide_t.h`) as well as the
[`uintwide_t.h`](https://github.com/ckormanyos/wide-integer/tree/master/math/wide_integer)
header.
Defining `WIDE_INTEGER_DISABLE_IMPLEMENT_UTIL_DYNAMIC_ARRAY`
upstream of the inclusion of the `uintwide_t.h` header
disables that file's definition
of `util::dynamic_array<>`, and ensures that
only one definition is visivble in the file
containing both big-number headers.

A use-case that uses both
[wide-decimal](https://github.com/ckormanyos/wide-decimal)
as well as
[wide-integer](https://github.com/ckormanyos/wide-integer)
simultaneously has been implemented in a test file.
This file is called
[`test_mixed_wide_decimal_wide_integer_b2n.cpp`](https://github.com/ckormanyos/wide-decimal/blob/main/test/test_mixed_wide_decimal_wide_integer_b2n.cpp).
It is included in the
[test folder](https://github.com/ckormanyos/wide-decimal/tree/main/test)
and also included in continuous integration.
This test file involves computations and comparisons of Bernoulli numbers.

### Alternatives and limitations

Alternative libraries for big float types include,
among others, most notably
[GMP](https://gmplib.org/)
and
[`Boost.Multiprecision`](https://www.boost.org/doc/libs/1_79_0/libs/multiprecision/doc/html/index.html).

At the moment, wide-decimal has a rather large number of
so-called _guard_ _digits_. In addition, wide-decimal
does not make use of rounding when performing multiplication
or other common algebraic operations. The combined
result is that exact calculations such as those potentially
needed in some areas requiring pure, exact decimal calculations
may not work well with this library. In this sense, wide-decimal suffers
from the same rounding limitations as the
[`cpp_dec_float`](https://www.boost.org/doc/libs/1_79_0/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/cpp_dec_float.html)
class known from
[`Boost.Multiprecision`](https://www.boost.org/doc/libs/1_79_0/libs/multiprecision/doc/html/index.html).
