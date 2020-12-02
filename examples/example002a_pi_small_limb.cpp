///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <ctime>
#include <iostream>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

#include "example002x_pi_control.h"

bool math::wide_decimal::example002a_pi_small_limb()
{
  using local_limb_type = std::uint16_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>;

  const std::clock_t start = std::clock();

  math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>();

  const std::clock_t stop = std::clock();

  std::cout << "Time example002a_pi_small_limb(): "
    << float(stop - start) / float(CLOCKS_PER_SEC)
    << std::endl;

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + const_pi_control_head<local_limb_type>().size(),
                                     const_pi_control_head<local_limb_type>().begin());

  const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + ((std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)) - const_pi_control_tail<wide_decimal_digits10, local_limb_type>().size()),
                                     my_pi.crepresentation().cbegin() +  (std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)),
                                     const_pi_control_tail<wide_decimal_digits10, local_limb_type>().begin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example002a_pi_small_limb();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
