///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  template<typename real_value_type,
            typename real_function_type>
  real_value_type integral(const real_value_type& a,
                            const real_value_type& b,
                            const real_value_type& tol,
                            real_function_type real_function)
  {
    std::uint_fast32_t n2(1);

    real_value_type step = ((b - a) / 2U);

    real_value_type result = (real_function(a) + real_function(b)) * step;

    const std::uint_fast8_t k_max = UINT8_C(32);

    for(std::uint_fast8_t k = UINT8_C(0); k < k_max; ++k)
    {
      real_value_type sum(0);

      for(std::uint_fast32_t j(0U); j < n2; ++j)
      {
        const std::uint_fast32_t two_j_plus_one = (j * UINT32_C(2)) + UINT32_C(1);

        sum += real_function(a + (step * two_j_plus_one));
      }

      const real_value_type tmp = result;

      result = (result / 2U) + (step * sum);

      using std::fabs;

      const real_value_type ratio = fabs(tmp / result);

      const real_value_type delta = fabs(ratio - 1U);

      if((k > UINT8_C(1)) && (delta < tol))
      {
        break;
      }

      n2 *= 2U;

      step /= 2U;
    }

    return result;
  }
}

bool math::wide_decimal::example010_trapezoid_integration()
{
  using float_type = math::wide_decimal::decwide_t<101U>;

  const float_type epsilon = std::numeric_limits<float_type>::epsilon();

  using std::exp;
  using std::sqrt;

  const float_type tol = sqrt(epsilon);

  const float_type t = integral(float_type(0),
                                float_type(1),
                                tol,
                                [](const float_type& t) -> float_type
                                {
                                  return exp(t) * sqrt(t);
                                });

  const float_type control
  {
    "1.2556300825518636265562388844956743426363769974832203893068530475053133160625684237494823658076312666081"
  };

  const float_type closeness = fabs(1 - (t / control));

  const bool result_is_ok = closeness < (std::numeric_limits<float_type>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example010_trapezoid_integration();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
