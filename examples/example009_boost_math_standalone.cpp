///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/special_functions/sign.hpp>
#include <boost/math/bindings/decwide_t.hpp>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;
}

bool math::wide_decimal::example009_boost_math_standalone()
{
  const dec1001_t x = dec1001_t(UINT32_C(123456789)) / 100U;

  using std::cbrt;
  using std::fabs;

  // Compare wide-decimal's cube root function with that of Boost.Math.
  // Also exercise several different interpretations of the constant pi.

  const dec1001_t c       = cbrt(x / math::wide_decimal::pi<wide_decimal_digits10, typename dec1001_t::limb_type, std::allocator<void>, double>());

  const dec1001_t control = boost::math::cbrt(x / boost::math::constants::pi<dec1001_t>());

  const dec1001_t closeness = fabs(1 - (c / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example009_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
