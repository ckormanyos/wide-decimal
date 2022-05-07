///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>
#include <ctime>
#include <iostream>

// Disable heavyweight features via macros for this example.
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <examples/example_decwide_t.h>
#include <math/constants/constants_pi_control_for_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <mcal_lcd/mcal_lcd_console.h>
#include <util/memory/util_n_slot_array_allocator.h>
#include <util/utility/util_baselexical_cast.h>

namespace example002a_pi
{
  auto lcd0() -> mcal::lcd::lcd_base&
  {
    static mcal::lcd::lcd_console lc0;

    return lc0;
  }
} // namespace example002a_pi

auto example002a_pi_small_limb_digits10_callback(const std::uint32_t d10) -> void
{
  std::array<char, static_cast<std::size_t>(UINT32_C(10))> p_str = {{ '\0' }};

  const char* p_end = util::baselexical_cast(d10, p_str.data()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

  example002a_pi::lcd0().write(p_str.data(), static_cast<std::uint_fast8_t>(p_end - p_str.data()), 0U);
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example002a_pi_small_limb() -> bool
#else
auto math::wide_decimal::example002a_pi_small_limb() -> bool
#endif
{
  using local_limb_type = std::uint16_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  using local_wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type>;

  const auto start = std::clock();

  const local_wide_decimal_type my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type>(example002a_pi_small_limb_digits10_callback);

  const auto stop = std::clock();

  std::cout << "Time example002a_pi_small_limb(): "
            << static_cast<float>(stop - start) / static_cast<float>(CLOCKS_PER_SEC)
            << std::endl;

  const auto head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_16.size(),
                                     math::constants::const_pi_control_head_16.begin());

  using const_iterator_type = typename local_wide_decimal_type::representation_type::const_iterator;

  const_iterator_type
    fi
    (
        my_pi.crepresentation().cbegin()
      + static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
          - static_cast<std::uint32_t>(math::constants::const_pi_control_tail_16_1000001.size())
        )
    );

  const auto tail_is_ok = std::equal(fi,
                                     fi + math::constants::const_pi_control_tail_16_1000001.size(),
                                          math::constants::const_pi_control_tail_16_1000001.begin());

  const auto result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE002A_PI_SMALL_LIMB)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = math::wide_decimal::example002a_pi_small_limb();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
