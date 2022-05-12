///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example000_multiply_nines() -> bool
#else
auto math::wide_decimal::example000_multiply_nines() -> bool
#endif
{
  using local_limb_type = std::uint8_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(50);

  using wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10,
                                  local_limb_type,
                                  void,
                                  float,
                                  std::int16_t>;

  const wide_decimal_type a
  {
    (  std::string("9.")
     + std::string(wide_decimal_digits10 - 1, char('9'))).c_str()
  };

  const wide_decimal_type control
  {
    (  std::string("99.")
     + std::string(wide_decimal_digits10 - 3, char('9'))
     + std::string("8")).c_str()
  };

  const wide_decimal_type result = (a * a);

  const wide_decimal_type closeness = fabs(1 - fabs(result / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<wide_decimal_type>::epsilon() * static_cast<std::uint32_t>(UINT8_C(10))));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE000_MULTIPLY_NINES)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = math::wide_decimal::example000_multiply_nines();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
