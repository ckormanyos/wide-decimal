///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <array>
#include <cstdint>

#if defined(__GNUC__) && defined(__AVR__)
#include <avr/pgmspace.h>

#define MY_PROGMEM PROGMEM
#else
#define MY_PROGMEM
#endif

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS
#define WIDE_DECIMAL_DISABLE_USE_STD_FUNCTION

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

#if(__cplusplus >= 201703L)
namespace mcal::memory::progmem {
#else
namespace mcal { namespace memory { namespace progmem { // NOLINT(modernize-concat-nested-namespaces)
#endif

using std::array;

#if(__cplusplus >= 201703L)
} // namespace mcal::memory::progmem
#else
} // namespace progmem
} // namespace memory
} // namespace mcal
#endif

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example013a_embeddable_agm() -> bool
#else
auto math::wide_decimal::example013a_embeddable_agm() -> bool
#endif
{
  // N[Pi, 106] and truncate the final digit.
  using local_limb_type = std::uint16_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(53);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  using dec53_t = math::wide_decimal::decwide_t<wide_decimal_digits10,
                                                local_limb_type,
                                                local_allocator_type,
                                                float,
                                                std::int16_t,
                                                float>;

  static const mcal::memory::progmem::array<typename dec53_t::limb_type, 14U> app_benchmark_pi_agm_control MY_PROGMEM =
  {{
    static_cast<typename dec53_t::limb_type>(UINT16_C(   3)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(1415)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(9265)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(3589)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(7932)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(3846)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(2643)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(3832)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(7950)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(2884)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(1971)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(6939)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(9375)),
    static_cast<typename dec53_t::limb_type>(UINT16_C(1058))
  }};

  const dec53_t my_pi =
    math::wide_decimal::pi<wide_decimal_digits10,
                           local_limb_type,
                           local_allocator_type,
                           float,
                           std::int16_t,
                           float>();

  const bool result_is_ok = std::equal(app_benchmark_pi_agm_control.cbegin(),
                                       app_benchmark_pi_agm_control.cend(),
                                       my_pi.crepresentation().cbegin());

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE013A_EMBEDDABLE_AGM)

#if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
#include <iomanip>
#include <iostream>
#endif

auto main() -> int
{
  const bool result_is_ok = math::wide_decimal::example013a_embeddable_agm();

  #if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
  #endif

  return (result_is_ok ? 0 : -1);
}

#endif // WIDE_DECIMAL_STANDALONE_EXAMPLE013A_EMBEDDABLE_AGM
