///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2024.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

// Disable heavyweight features via macros for this example.
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <examples/example_decwide_t.h>
#include <math/constants/constants_pi_control_for_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <mcal_lcd/mcal_lcd_console.h>
#include <test/stopwatch.h>
#include <util/memory/util_n_slot_array_allocator.h>
#include <util/utility/util_baselexical_cast.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace example002d_pi {

auto lcd0() -> mcal::lcd::lcd_base&
{
  static mcal::lcd::lcd_console lc0;

  return lc0;
}

} // namespace example002d_pi

auto example002d_pi_limb8_digits10_callback(const std::uint32_t d10) -> void
{
  std::array<char, static_cast<std::size_t>(UINT32_C(10))> p_str = {{ '\0' }};

  const char* p_end = util::baselexical_cast(d10, p_str.data(), p_str.data() + p_str.size()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

  const auto len = static_cast<std::uint_fast8_t>(p_end - p_str.data());

  example002d_pi::lcd0().write(p_str.data(), len, 0U);
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example002d_pi_limb08() -> bool
#else
auto ::math::wide_decimal::example002d_pi_limb08() -> bool
#endif
{
  using local_limb_type = std::uint8_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1000001);

  #if defined(WIDE_DECIMAL_NAMESPACE)
  constexpr std::int32_t local_elem_number =
    WIDE_DECIMAL_NAMESPACE::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;
  #else
  constexpr std::int32_t local_elem_number =
    ::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;
  #endif

  #if defined(WIDE_DECIMAL_NAMESPACE)
  constexpr std::int32_t local_elem_digits10 =
    WIDE_DECIMAL_NAMESPACE::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;
  #else
  constexpr std::int32_t local_elem_digits10 =
    ::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;
  #endif

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using local_wide_decimal_type =
    WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, float, std::int32_t>;
  #else
  using local_wide_decimal_type =
    ::math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, float, std::int32_t>;
  #endif

  using stopwatch_type = concurrency::stopwatch;

  stopwatch_type my_stopwatch { };

  #if defined(WIDE_DECIMAL_NAMESPACE)
  const local_wide_decimal_type my_pi =
    WIDE_DECIMAL_NAMESPACE::math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, float, std::int32_t>(example002d_pi_limb8_digits10_callback);
  #else
  const local_wide_decimal_type my_pi =
    ::math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, float, std::int32_t>(example002d_pi_limb8_digits10_callback);
  #endif

  const float execution_time { stopwatch_type::elapsed_time<float>(my_stopwatch) };

  {
    std::stringstream strm { };

    strm << "Time example002d_pi_limb08()        : "
         << std::fixed
         << std::setprecision(1)
         << execution_time
         << "s"
         ;

    std::cout << strm.str() << std::endl;
  }

  #if defined(WIDE_DECIMAL_NAMESPACE)
  const auto head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + WIDE_DECIMAL_NAMESPACE::math::constants::const_pi_control_head_08.size(),
                                     WIDE_DECIMAL_NAMESPACE::math::constants::const_pi_control_head_08.begin());
  #else
  const auto head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + ::math::constants::const_pi_control_head_08.size(),
                                     ::math::constants::const_pi_control_head_08.begin());
  #endif

  using const_iterator_type = typename local_wide_decimal_type::representation_type::const_iterator;

  #if defined(WIDE_DECIMAL_NAMESPACE)
  const_iterator_type
    fi
    (
        my_pi.crepresentation().cbegin()
      + static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
          - static_cast<std::uint32_t>(WIDE_DECIMAL_NAMESPACE::math::constants::const_pi_control_tail_08_1000001.size())
        )
    );
  #else
  const_iterator_type
    fi
    (
        my_pi.crepresentation().cbegin()
      + static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
          - static_cast<std::uint32_t>(::math::constants::const_pi_control_tail_08_1000001.size())
        )
    );
  #endif

  // When using 8-bit limb size, the two least significant limbs are imprecise.
  #if defined(WIDE_DECIMAL_NAMESPACE)
  const auto tail_is_ok = std::equal(fi,
                                     fi + (WIDE_DECIMAL_NAMESPACE::math::constants::const_pi_control_tail_08_1000001.size() - 3U),
                                           WIDE_DECIMAL_NAMESPACE::math::constants::const_pi_control_tail_08_1000001.begin());
  #else
  const auto tail_is_ok = std::equal(fi,
                                     fi + (::math::constants::const_pi_control_tail_08_1000001.size() - 3U),
                                           ::math::constants::const_pi_control_tail_08_1000001.begin());
  #endif

  const auto result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE002D_PI_LIMB08)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = ::math::wide_decimal::example002d_pi_limb08();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic pop
#endif
