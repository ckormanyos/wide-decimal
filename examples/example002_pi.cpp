///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <ctime>
#include <iostream>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

#include <math/wide_decimal/decwide_t.h>
#include <mcal_lcd/mcal_lcd_console.h>
#include <util/memory/util_n_slot_array_allocator.h>
#include <util/utility/util_baselexical_cast.h>

namespace mcal { namespace lcd {

mcal::lcd::lcd_base& lcd0() noexcept
{
  static mcal::lcd::lcd_console lc0;

  return lc0;
}

} } // namespace mcal::lcd

void example002_pi_digits10_callback(const std::uint32_t d10)
{
  char p_str[10U] = { 0 };
  char* p_end = util::baselexical_cast(d10, p_str);

  mcal::lcd::lcd0().write_n(p_str, (std::uint_fast8_t) (p_end - p_str), 0U);
}

bool math::wide_decimal::example002_pi()
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 16U>;

  const std::clock_t start = std::clock();

  math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>(example002_pi_digits10_callback);

  const std::clock_t stop = std::clock();

  std::cout << "Time example002_pi(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  constexpr std::array<local_limb_type, 8U> control_head =
  {{
    (local_limb_type) UINT32_C(3),
    (local_limb_type) UINT32_C(14159265),
    (local_limb_type) UINT32_C(35897932),
    (local_limb_type) UINT32_C(38462643),
    (local_limb_type) UINT32_C(38327950),
    (local_limb_type) UINT32_C(28841971),
    (local_limb_type) UINT32_C(69399375),
    (local_limb_type) UINT32_C(10582097)
  }};

  constexpr std::array<local_limb_type, 8U> control_tail =
  {{
    (local_limb_type) UINT32_C(20875424),
    (local_limb_type) UINT32_C(50598956),
    (local_limb_type) UINT32_C(78796130),
    (local_limb_type) UINT32_C(33116462),
    (local_limb_type) UINT32_C(83996346),
    (local_limb_type) UINT32_C(46042209),
    (local_limb_type) UINT32_C( 1061057),
    (local_limb_type) UINT32_C(79458151)
  }};

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + control_head.size(),
                                     control_head.cbegin());

  const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + (125001UL - control_tail.size()),
                                     my_pi.crepresentation().cbegin() +  125001UL,
                                     control_tail.cbegin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example002_pi();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
