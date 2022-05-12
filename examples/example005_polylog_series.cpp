///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdint>
#include <deque>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>

namespace local_polylog
{
  template<typename FloatingPointType>
  auto polylog(std::int32_t s, const FloatingPointType& x) -> FloatingPointType;

  template<typename FloatingPointType>
  auto polylog(std::int32_t s, const FloatingPointType& x) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type tol = (x * std::numeric_limits<floating_point_type>::epsilon()) / 10U;

    floating_point_type x_pow_k(x);
    floating_point_type sum    (x);

    for(auto   k = static_cast<std::uint_fast32_t>(UINT32_C(2));
               k < static_cast<std::uint_fast32_t>(UINT32_C(100000));
             ++k)
    {
      x_pow_k *= x;

      const floating_point_type k_pow_s = pow(floating_point_type(k), s);
      const floating_point_type term    = x_pow_k / k_pow_s;

      if(term < tol)
      {
        break;
      }

      sum += term;
    }

    return sum;
  }
} // namespace local_polylog

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example005_polylog_series() -> bool
#else
auto math::wide_decimal::example005_polylog_series() -> bool
#endif
{
  using dec101_t = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(101))>;

  using std::fabs;

  const dec101_t poly = local_polylog::polylog(7U, dec101_t(17U) / 71U);

  // N[PolyLog[7, 17/71], 101]
  const dec101_t control
  {
    "0.23989099751201076665599565769828454152030927256642802570721839696637617308754054721620440634024352282"
  };

  // Check the closeness of the result.
  const dec101_t closeness = fabs(1 - (poly / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<dec101_t>::epsilon() * static_cast<std::uint32_t>(UINT8_C(10))));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE005_POLYLOG_SERIES)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = math::wide_decimal::example005_polylog_series();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
