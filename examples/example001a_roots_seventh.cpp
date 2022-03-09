///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example001a_roots_seventh() -> bool
#else
auto math::wide_decimal::example001a_roots_seventh() -> bool
#endif
{
  using dec101_t = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(101)), std::uint32_t, void>;

  const dec101_t r7 = rootn(dec101_t(123456U) / 100, 7);

  // N[(123456/100)^(1/7), 101]
  const dec101_t control
  {
    "2.7646782775741109196428393776807250048855576457486681034865195752391286191642449378095974978251349039"
  };

  const dec101_t closeness = fabs(1 - fabs(r7 / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec101_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example001a_roots_seventh();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
