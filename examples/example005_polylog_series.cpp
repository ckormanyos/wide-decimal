///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdint>
#include <deque>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  template<typename FloatingPointType,
           typename OtherUnsignedIntegralTypeP>
  FloatingPointType pow(const FloatingPointType& b, OtherUnsignedIntegralTypeP p)
  {
    using floating_point_type = FloatingPointType;

    // Calculate (b ^ p).

    floating_point_type result;

    const OtherUnsignedIntegralTypeP zero(std::uint8_t(0U));

    if(p == zero)
    {
      result = floating_point_type(std::uint8_t(1U));
    }
    else if(p == 1U)
    {
      result = b;
    }
    else if(p == 2U)
    {
      result  = b;
      result *= b;
    }
    else if(p == 3U)
    {
      result  = b;
      result *= b;
      result *= b;
    }
    else if(p == 4U)
    {
      result  = b;
      result *= b;
      result *= result;
    }
    else
    {
      // Implement the unsigned integral power function
      // using the so-called ladder method.

      result = floating_point_type(std::uint8_t(1U));

      floating_point_type        y      (b);
      OtherUnsignedIntegralTypeP p_local(p);

      while((p_local == zero) == false)
      {
        if((std::uint_fast8_t(p_local) & 1U) != 0U)
        {
          result *= y;
        }

        y *= y;

        p_local >>= 1U;
      }
    }

    return result;
  }
}

namespace local
{
  template<typename FloatingPointType>
  FloatingPointType polylog(const std::int32_t s, const FloatingPointType& x)
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type tol = (x * std::numeric_limits<floating_point_type>::epsilon()) / 10U;

    floating_point_type x_pow_k(x);
    floating_point_type sum    (x);

    for(std::uint_fast32_t k = UINT32_C(2); k < UINT32_C(100000); ++k)
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
}

bool math::wide_decimal::example005_polylog_series()
{
  using dec101_t = math::wide_decimal::decwide_t<101>;

  using std::fabs;

  // N[PolyLog[7, 17/71], 101]
  const dec101_t poly = local::polylog(7U, dec101_t(17U) / 71U);

  const dec101_t control
  {
    "0.23989099751201076665599565769828454152030927256642802570721839696637617308754054721620440634024352282"
  };

  // Check the closeness of the result.
  const dec101_t closeness = fabs(1 - (poly / control));

  const bool result_is_ok = (closeness < (std::numeric_limits<dec101_t>::epsilon() * 10));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example005_polylog_series();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
