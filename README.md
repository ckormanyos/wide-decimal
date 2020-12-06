# Wide-decimal
Wide-decimal implements a generic C++ template for extended precision decimal float types.

This C++ template header-only library implements drop-in big decimal float types such as `dec101_t`, `dec10001_t`, `dec1000001_t`, etc., that can be used essentially like regular built-in floating-point types. Wide-decimal supports decimal float types having digit counts ranging from about <img src="https://render.githubusercontent.com/render/math?math=\sim\,30{\ldots}3,000,000">.

Wide-decimal implements both common algebraic operations as well as a few common `<cmath>`-like functions such as `fabs`, `sqrt` and `log`, and also includes full support for `std::numeric_limits`.

Wide-decimal is written in header-only C++11.

# Implementation goals

  - Wide precision range up to one million decimal digits
  - Moderately good efficiency over the entire wide precision range
  - Clean header-only C++11 design
  - Seamless portability to any modern C++11 compiler
  - Scalability with small memory footprint and efficiency suitable for _bare-metal_ embedded systems

# The examples

  - ![`example001_roots_sqrt`](./examples/example001_roots_sqrt.cpp) computes a square root.
  - ![`example001a_roots_seventh`](./examples/example001a_roots_seventh.cpp) computes a seventh root.
  - ![`example002_pi`](./examples/example002_pi.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a Gauss AGM iteration.
  - ![`example002a_pi_small_limb`](./examples/example002a_pi_small_limb.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a 16-bit internal limb type.
  - ![`example002b_pi_100k`](./examples/example002b_pi_100k.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=100,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi">.
  - ![`example002c_pi_quintic`](./examples/example002c_pi_quintic.cpp) calculates <img src="https://render.githubusercontent.com/render/math?math=1,000,001"> decimal digits of <img src="https://render.githubusercontent.com/render/math?math=\pi"> using a Borwein quintic iteration.
  - ![`example003_zeta`](./examples/example003_zeta.cpp) computes a Riemann zeta function value.
  - ![`example004_bessel_recur`](./examples/example004_bessel_recur.cpp) implements cylindrical Bessel functions of integral order via downward recursion with a Neumann sum.
  - ![`example005_polylog_series`](./examples/example005_polylog_series.cpp) performs a small-argument polylogarithm series calculation.
  - ![`example006_logarithm`](./examples/example006_logarithm.cpp) calculates the value of a logarithm (internally using a Gauss AGM method).
  - ![`example007_catalan_series`](./examples/example007_catalan_series.cpp) computes <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digits of Catalan's constant using an accelerated series.
  - ![`example008_bernoulli_tgamma`](./examples/example008_bernoulli_tgamma.cpp) implements `tgamma(x)` using Stirling's asymptotic expansion of the logarithm of the Gamma function with Bernoulli numbers
and subsequently calculates <img src="https://render.githubusercontent.com/render/math?math=1,001"> decimal digits
of <img src="https://render.githubusercontent.com/render/math?math=\Gamma(21/2)">.

# 1,000,001 digits of pi on a bare metal microcontroller

The wide-decimal float back end is used to compute
<img src="https://render.githubusercontent.com/render/math?math=1,000,001">
decimal digits of the mathematical constant
<img src="https://render.githubusercontent.com/render/math?math=\pi">
on selected bare-metal OS-less microcontroller systems in
[pi-crunch-metal](https://github.com/ckormanyos/pi-crunch-metal)

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
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
```

Note: Activating the option `WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION`
simultaneously disallows using `decwide_t` in a multithreaded application.
So if PC-based or other kinds of multithreading are used, then dynamic memory
allocation is needed and can not be disabled. In other words,

```
// Deactivate the disabling of dynamic memory for multithreaded PC work.
//#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
```

See the examples directory as more use cases are being created.

# Quick start
Easy application follows via a traditional C-style typedef or C++11 alias.
The defined type can be used very much like a built-in floating-point type.

The following sample, for instance, defines and uses a decimal type
called `dec101_t` having
<img src="https://render.githubusercontent.com/render/math?math=101">
decimal digits of precision.

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
The subroutine `do_something()` initializes the variable `d`
of type `dec101_t` with 1/3 and subsequently prints the 101 digit
value of `d` to the console.

# Detailed examples

The example below calculates the square root of the decimal representation of
<img src="https://render.githubusercontent.com/render/math?math=\sqrt{1234.56}">,
the result of which is approximately
<img src="https://render.githubusercontent.com/render/math?math=35.136306009596398663933384640418055759751518287169314528165976164717710895452890928635031219132220\ldots">.

```
#include <array>
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
`util::n_slot_array_allocator` is utilized for exact memory management
of a certain number of temporary storages of mega-digit numbers
(tuned to 16 in this particular example).

```
#include <array>
#include <iomanip>
#include <iostream>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

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

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 16U>;

  const std::clock_t start = std::clock();

  math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>(nullptr);

  const std::clock_t stop = std::clock();

  std::cout << "Time example002_pi(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + const_pi_control_head<local_limb_type>().size(),
                                     const_pi_control_head<local_limb_type>().begin());

  const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + ((std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)) - const_pi_control_tail<wide_decimal_digits10, local_limb_type>().size()),
                                     my_pi.crepresentation().cbegin() +  (std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)),
                                     const_pi_control_tail<wide_decimal_digits10, local_limb_type>().begin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}
```

The million digit run is comparatively slow and requires approximately
10 seconds on a modern PC. Considering, however, the design goals
of header-only and capable of running on bare-metal, this is
a very nice calculational result.
