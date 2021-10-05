///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>

#include <math/wide_decimal/decwide_t.h>
#include <math/wide_decimal/decwide_t_examples.h>
#include <util/utility/util_baselexical_cast.h>

namespace local
{
   std::minstd_rand0 eng(static_cast<std::minstd_rand0::result_type>(std::clock()));

   std::uniform_int_distribution<unsigned> dist_sign
   (
      0U,
      1U
   );

  template<typename DecimalType>
  bool test_rational_floor()
  {
    using decimal_type = DecimalType;

    static_assert(std::numeric_limits<decimal_type>::digits10 < 16,
                  "Error: This test is designed for less than 16 decimal digits");

    using decimal_type = DecimalType;

    bool result_is_ok = true;

    for(std::int32_t lo_index = 101U; lo_index < 1010U; lo_index += 7U)
    {
      for(std::int32_t hi_index = 10001U; hi_index < 100010U; hi_index += 17U)
      {
        const bool lo_is_neg = ((dist_sign(eng) % 2U) == 0U);
        const bool hi_is_neg = ((dist_sign(eng) % 2U) == 0U);

        const std::int32_t lo = ((lo_is_neg == false) ? lo_index : -lo_index);
        const std::int32_t hi = ((hi_is_neg == false) ? hi_index : -hi_index);

        const std::int32_t lo_hi = lo * hi;

        const decimal_type a = decimal_type { lo_hi } / decimal_type { lo };
        const decimal_type b = floor(a);

        result_is_ok &= (static_cast<std::int32_t>(a) == b);

        if(result_is_ok == false)
        {
          break;
        }
      }
    }

    return result_is_ok;
  }

  template<typename DecimalType>
  bool test_rational_ceil()
  {
    using decimal_type = DecimalType;

    static_assert(std::numeric_limits<decimal_type>::digits10 < 16,
                  "Error: This test is designed for less than 16 decimal digits");

    using decimal_type = DecimalType;

    bool result_is_ok = true;

    for(std::int32_t lo_index = 101U; lo_index < 1010U; lo_index += 7U)
    {
      for(std::int32_t hi_index = 10001U; hi_index < 100010U; hi_index += 17U)
      {
        const bool lo_is_neg = ((dist_sign(eng) % 2U) == 0U);
        const bool hi_is_neg = ((dist_sign(eng) % 2U) == 0U);

        const std::int32_t lo = ((lo_is_neg == false) ? lo_index : -lo_index);
        const std::int32_t hi = ((hi_is_neg == false) ? hi_index : -hi_index);

        const std::int32_t lo_hi = lo * hi;

        const decimal_type a = decimal_type { lo_hi } / decimal_type { lo };
        const decimal_type b = floor(a);

        result_is_ok &= (static_cast<std::int32_t>(a) == b);

        if(result_is_ok == false)
        {
          break;
        }
      }
    }

    return result_is_ok;
  }
}

bool math::wide_decimal::example012_rational_floor_ceil()
{
  bool result_is_ok = true;

  // Test floor/ceil for 10 decimal digits.
  {
    using decimal_type = math::wide_decimal::decwide_t<10U, std::uint32_t, void>;

    result_is_ok &= local::test_rational_floor<decimal_type>();
    result_is_ok &= local::test_rational_ceil <decimal_type>();
  }

  // Test floor/ceil for 12 decimal digits.
  {
    using decimal_type = math::wide_decimal::decwide_t<12U, std::uint32_t, void>;

    result_is_ok &= local::test_rational_floor<decimal_type>();
    result_is_ok &= local::test_rational_ceil <decimal_type>();
  }

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example012_rational_floor_ceil();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
