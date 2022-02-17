///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>
#include <math/wide_decimal/decwide_t_examples.h>
#include <util/memory/util_n_slot_array_allocator.h>

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example001_roots_sqrt() -> bool
#else
auto math::wide_decimal::example001_roots_sqrt() -> bool
#endif
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(101);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 32U>; // NOLINT(cppcoreguidelines-avoid-magic-numbers)

  using dec101_t = math::wide_decimal::decwide_t<wide_decimal_digits10,
                                                 local_limb_type,
                                                 local_allocator_type,
                                                 float,
                                                 std::int32_t>;

  const dec101_t s = sqrt(dec101_t(123456U) / 100);

  // N[Sqrt[123456/100], 111]
  // 35.1363060095963986639333846404180557597515182871693145281659761647177108954528909286350312191322209780537650946
  const dec101_t control
  (
    "35.1363060095963986639333846404180557597515182871693145281659761647177108954528909286350312191322209780537650946"
  );

  using std::fabs;

  const dec101_t closeness = fabs(1 - fabs(s / control));

  const bool result_is_ok = (closeness < (std::numeric_limits<dec101_t>::epsilon() * 10));

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
