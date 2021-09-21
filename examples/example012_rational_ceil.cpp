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

#include <math/wide_decimal/decwide_t.h>
#include <math/wide_decimal/decwide_t_examples.h>
#include <util/utility/util_baselexical_cast.h>

namespace local
{
  template<typename DecimalType>
  bool test_rational_ceil()
  {
    using decimal_type = DecimalType;

    static_assert(std::numeric_limits<decimal_type>::digits10 < 16,
                  "Error: This test is designed for less than 16 decimal digits");

    bool result_is_ok = true;

    for(std::uint32_t lo_index = 101U; lo_index < 1010U; lo_index += 7U)
    {
      for(std::uint32_t hi_index = 10001U; hi_index < 100010U; hi_index += 17U)
      {
        const std::uint32_t lo_hi = lo_index * hi_index;

        const decimal_type a = decimal_type { lo_hi } / decimal_type { lo_index };
        const decimal_type b = ceil(a);

        char pstr_a[16U] = { char('\0') };
        char pstr_b[16U] = { char('\0') };

        static_cast<void>(util::baselexical_cast((std::uint32_t) a, pstr_a));
        static_cast<void>(util::baselexical_cast((std::uint32_t) b, pstr_b));

        const std::string str_a(pstr_a);
        const std::string str_b(pstr_b);

        const bool a_and_b_are_equal = (str_a == str_b);

        result_is_ok &= a_and_b_are_equal;

        if(a_and_b_are_equal == false)
        {
          break;
        }
      }
    }

    return result_is_ok;
  }
}

bool math::wide_decimal::example012_rational_ceil()
{
  bool result_is_ok = true;

  result_is_ok &= local::test_rational_ceil<math::wide_decimal::decwide_t<10U, std::uint32_t, void>>();
  result_is_ok &= local::test_rational_ceil<math::wide_decimal::decwide_t<12U, std::uint32_t, void>>();

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example012_rational_ceil();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
