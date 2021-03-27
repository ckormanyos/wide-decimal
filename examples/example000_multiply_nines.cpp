///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

bool math::wide_decimal::example000_multiply_nines()
{
  using local_limb_type = std::uint8_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(50);

  using wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10,
                                  local_limb_type,
                                  void,
                                  float,
                                  std::int16_t>;

  const wide_decimal_type a
  {
      std::string("9.")
    + std::string(wide_decimal_digits10 - 1, char('9'))
  };

  const wide_decimal_type control
  {
      std::string("99.")
    + std::string(wide_decimal_digits10 - 3, char('9'))
    + std::string("8")
  };

  const wide_decimal_type result = (a * a);

  const wide_decimal_type closeness = fabs(1 - fabs(result / control));

  const bool result_is_ok = closeness < (std::numeric_limits<wide_decimal_type>::epsilon() * 10);

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
