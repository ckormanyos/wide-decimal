///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////


#include <algorithm>
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

namespace example002c_pi {

// *****************************************************************************
// Function    : pi_borwein_quintic(std::ostream* p_ostream = nullptr)
// 
// Description : Compute pi using a quintically convergent iteration scheme.
//               See the book "Pi Unleashed", algorithm 16.153, page 237.
//               If the input b_trace = true, then the calculation progress
//               will be written to the (optional) output stream p_ostream.
// 
// *****************************************************************************
template<typename FloatingPointType>
auto pi_borwein_quintic(std::ostream* p_ostream = nullptr) -> FloatingPointType;

template<typename FloatingPointType>
auto pi_borwein_quintic(std::ostream* p_ostream) -> FloatingPointType
{
  using floating_point_type = FloatingPointType;

  floating_point_type val_pi(floating_point_type(1U) / 2U);

  if(p_ostream != nullptr)
  {
    *p_ostream << 0 << std::endl;
  }

  std::uint64_t five_pow_k = 1U;

  const floating_point_type local_one (1U);
  const floating_point_type local_five(5U);

  floating_point_type sk = (sqrt(local_five) - floating_point_type(2U)) * 5U; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  // Determine the requested precision of the upcoming iteration in units of digits10.
  const auto required_precision_fifth =
    static_cast<std::uint32_t>
    (
        static_cast<std::uint32_t>(static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::max_digits10 * 2) + static_cast<std::uint32_t>(5U))
      / static_cast<std::uint32_t>(10U)
    );

  using std::log;
  using std::lround;

  const auto digits10_scale =
    static_cast<std::uint32_t>
    (
      lround(static_cast<float>(1000.0F * log(static_cast<float>(std::numeric_limits<floating_point_type>::radix))) / log(10.0F))
    );

  for(auto   k = static_cast<std::int32_t>(1);
             k < static_cast<std::int32_t>(INT32_C(30));
           ++k)
  {
    using std::ilogb;
    using std::sqrt;

    const floating_point_type x          = (local_five / sk) - local_one;
    const floating_point_type x_squared  = x * x;
    const floating_point_type y          = x_squared - (x * 2U) + floating_point_type(8U);
    const floating_point_type one_over_z = rootn_inv((x * (y + sqrt((y * y) - ((x_squared * x) * 4U)))) / 2U, 5);

    const floating_point_type term = ((local_one / one_over_z) + (x * one_over_z) + local_one);

    const floating_point_type sk_squared(sk * sk);

    const floating_point_type previous_ak(val_pi);

    val_pi =
          (sk_squared * val_pi)
    - (  ((sk_squared - local_five) / 2U)
        +  sqrt(sk * (sk_squared - (sk * 2U) + local_five))) * five_pow_k;

    const auto ib = (std::max)(static_cast<std::int32_t>(0),
                               static_cast<std::int32_t>(-ilogb(val_pi - previous_ak)));

    const auto digits10_of_iteration =
      static_cast<std::uint32_t>
      (
        static_cast<std::uint64_t>(static_cast<std::uint64_t>(ib) * digits10_scale) / UINT32_C(1000)
      );

    if(p_ostream != nullptr)
    {
      *p_ostream << digits10_of_iteration << std::endl;
    }

    // Test the significant digits of the last iteration change.
    // If there are enough significant digits, then the calculation
    // is finished.
    if(digits10_of_iteration >= required_precision_fifth)
    {
      break;
    }

    sk = floating_point_type(25U) / (sk * (term * term)); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    five_pow_k *= 5U; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  if(p_ostream != nullptr)
  {
    *p_ostream << std::numeric_limits<floating_point_type>::digits10 << std::endl;
  }

  val_pi = local_one / val_pi;

  return val_pi;
}

} // namespace example002c_pi

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example002c_pi_quintic() -> bool
#else
auto math::wide_decimal::example002c_pi_quintic() -> bool
#endif
{
  using local_limb_type = std::uint32_t;

  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1000001);

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using local_wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10>;

  const auto start = std::clock();

  const auto my_pi =
    example002c_pi::pi_borwein_quintic<math::wide_decimal::decwide_t<wide_decimal_digits10>>(&std::cout);

  const auto stop = std::clock();

  std::cout << "Time example002c_pi_quintic(): "
            << static_cast<float>(stop - start) / static_cast<float>(CLOCKS_PER_SEC)
            << std::endl;

  const auto head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_32.size(),
                                     math::constants::const_pi_control_head_32.begin());

  using const_iterator_type = typename local_wide_decimal_type::representation_type::const_iterator;

  const_iterator_type
    fi
    (
        my_pi.crepresentation().cbegin()
      + static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
          - static_cast<std::uint32_t>(math::constants::const_pi_control_tail_32_1000001.size())
        )
    );

  const auto tail_is_ok = std::equal(fi,
                                     fi + math::constants::const_pi_control_tail_32_1000001.size(),
                                          math::constants::const_pi_control_tail_32_1000001.begin());

  const auto result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE002C_PI_QUINTIC)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = math::wide_decimal::example002c_pi_quintic();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
