///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////


#include <algorithm>
#include <ctime>
#include <iostream>

#include <math/wide_decimal/decwide_t.h>

#include "example002x_pi_control.h"

namespace
{
  // *****************************************************************************
  // Function    : pi_borwein_quintic(std::ostream* p_ostream = nullptr)
  // 
  // Description : Compute pi using a quintically convergent iteration scheme.
  //               See the book "Pi Unleashed", algorithm 16.153, page 237.
  //               If the input b_trace = true, then the calculation progress
  //               will be output to the output stream os.
  // 
  // *****************************************************************************
  template<typename FloatingPointType>
  FloatingPointType pi_borwein_quintic(std::ostream* p_ostream = nullptr)
  {
    using floating_point_type = FloatingPointType;

    floating_point_type val_pi(floating_point_type(1U) / 2U);

    if(p_ostream != nullptr)
    {
      *p_ostream << "Calculating pi with Borwein quintic.\n";
    }

    std::int64_t five_pow_k = 1;

    const floating_point_type local_one (1U);
    const floating_point_type local_five(5U);

    floating_point_type sk = (sqrt(local_five) - floating_point_type(2U)) * 5U;

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t required_precision_fifth =
        static_cast<std::int32_t>(static_cast<std::int32_t>(std::numeric_limits<floating_point_type>::max_digits10 * 2) + static_cast<std::int32_t>(5))
      / static_cast<std::int32_t>(10);

    for(std::int32_t k = static_cast<std::int32_t>(1); k < static_cast<std::int32_t>(30); ++k)
    {
      using std::ilogb;
      using std::sqrt;

      const floating_point_type x         = (local_five / sk) - local_one;
      const floating_point_type x_squared = x * x;
      const floating_point_type y         = x_squared - (floating_point_type(x) * 2U) + floating_point_type(8U);
      const floating_point_type z         = rootn((x * (y + sqrt((y * y) - ((x_squared * x) * 4U)))) / 2U, 5);

      const floating_point_type term = (z + (x / z) + local_one);

      const floating_point_type sk_squared(sk * sk);

      const floating_point_type previous_ak(val_pi);

      val_pi =
            (sk_squared * val_pi)
      - (  ((sk_squared - local_five) / 2U)
          +  sqrt(sk * (sk_squared - (sk * 2U) + local_five))) * five_pow_k;

      sk = floating_point_type(25U) / (sk * (term * term));

      std::int32_t approximate_digits10 = -ilogb(val_pi - previous_ak);

      if(p_ostream != nullptr)
      {
        *p_ostream << "Digits of pi: " << approximate_digits10 << '\n';
      }

      // Test the significant digits of the last iteration change.
      // If there are enough significant digits, then the calculation
      // is finished.
      if(approximate_digits10 >= required_precision_fifth)
      {
        break;
      }

      five_pow_k *= 5U;
    }

    if(p_ostream != nullptr)
    {
      *p_ostream << "The iteration loop is done.\nCompute the inverse.\n";
    }

    val_pi = 1 / val_pi;

    if(p_ostream != nullptr)
    {
      *p_ostream << "Pi calculation is done." << std::endl;
    }

    return val_pi;
  }
}

bool math::wide_decimal::example002c_pi_1meg_quint()
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using dec1000001_t = math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type>;

  const std::clock_t start = std::clock();

  dec1000001_t my_pi = pi_borwein_quintic<dec1000001_t>(&std::cout);

  const std::clock_t stop = std::clock();

  std::cout << "Time example002c_pi_1meg_quint(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
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
  const bool result_is_ok = math::wide_decimal::example002c_pi_1meg_quint();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
