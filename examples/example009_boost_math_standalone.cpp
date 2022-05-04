///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <boost/version.hpp>

#if !defined(BOOST_VERSION)
#error BOOST_VERSION is not defined. Ensure that <boost/version.hpp> is properly included.
#endif

#if ((BOOST_VERSION >= 107900) && !defined(BOOST_MP_STANDALONE))
#define BOOST_MP_STANDALONE
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif

#include <boost/math/bindings/decwide_t.hpp>
#include <boost/math/special_functions/cbrt.hpp>

#include <examples/example_decwide_t.h>

namespace example009_boost
{
  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1001);

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using dec1001_t = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #else
  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #endif
} // namespace example009_boost

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example009_boost_math_standalone() -> bool
#else
auto math::wide_decimal::example009_boost_math_standalone() -> bool
#endif
{
  using example009_boost::dec1001_t;

  const dec1001_t x = dec1001_t(UINT32_C(123456789)) / 100U;

  using std::cbrt;
  using std::fabs;

  // Compare wide-decimal's cube root function with that of Boost.Math.
  // Also exercise several different interpretations of the constant pi.

  const dec1001_t c       = cbrt(x / math::wide_decimal::pi<example009_boost::wide_decimal_digits10, typename dec1001_t::limb_type, std::allocator<void>, double>());

  const dec1001_t control = boost::math::cbrt(x / boost::math::constants::pi<dec1001_t>());

  const dec1001_t closeness = fabs(1 - (c / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE009_BOOST_MATH_STANDALONE)

#include <iomanip>
#include <iostream>

// TBD: Handle exception catching in example009_boost_math_standalone at a later time.
auto main() -> int // NOLINT(bugprone-exception-escape)
{
  const auto result_is_ok = math::wide_decimal::example009_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
