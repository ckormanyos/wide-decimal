///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>

bool math::wide_decimal::example001_roots_sqrt()
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

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example001_roots_sqrt();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
