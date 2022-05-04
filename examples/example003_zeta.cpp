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

namespace local_zeta
{
  #if defined(WIDE_DECIMAL_NAMESPACE)
  using dec51_t = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(51)), std::uint32_t, void>;
  #else
  using dec51_t = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(51)), std::uint32_t, void>;
  #endif

  template<typename FloatType>
  auto pi() -> FloatType { return FloatType(); }

  template<>
  auto pi() -> float { return static_cast<float>(3.14159265358979323846264338327950288419716939937510582097L); } // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  template<>
  auto pi() -> double { return static_cast<double>(3.14159265358979323846264338327950288419716939937510582097L); } // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  template<>
  auto pi() -> long double { return 3.14159265358979323846264338327950288419716939937510582097L; } // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  // N[Pi, 57]
  template<>
  auto pi() -> dec51_t { return dec51_t( "3.14159265358979323846264338327950288419716939937510582097"); }

  auto compute_primes_via_square_root(std::deque<std::uint_fast16_t>& primes, // NOLINT(google-runtime-references)
                                      const std::uint_fast16_t maximum_value) -> void
  {
    // This is a helper function that produces a small table of primes.
    // It uses rudimentary (and slow) trial division with denominator
    // ranging from 3 up to the square root of the largest expected prime.

    for(auto i  = static_cast<std::uint_fast16_t>(UINT16_C(3));
             i <= maximum_value;
             i  = static_cast<std::uint_fast16_t>(i + UINT16_C(2)))
    {
      using std::sqrt;

      const auto maximum_square_root_value =
        static_cast<std::uint_fast16_t>
        (
          static_cast<float>(sqrt(static_cast<float>(i)) + 0.1F)
        );

      bool is_prime = true;

      for(auto   j = static_cast<std::uint_fast16_t>(UINT16_C(3));
                 j <= maximum_square_root_value;
               ++j)
      {
        if(static_cast<std::uint_fast16_t>(i % j) == UINT16_C(0))
        {
          is_prime = false;

          break;
        }
      }

      if(is_prime)
      {
        primes.push_back(i);
      }
    }
  }

  template<typename FloatingPointType>
  auto zeta16(const std::deque<std::uint_fast16_t>& primes) -> FloatingPointType
  {
    // This is a dedicated template function that computes
    // zeta(16) from an elementary series of prime powers.

    using float_type = FloatingPointType;

    float_type z = float_type(65536U) / 65535U; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    for(auto i = 1U; i < primes.size(); ++i)
    {
      float_type p16 = pow(float_type(primes.at(i)), 16); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      const float_type term = 1 / (1 - (1 / p16));

      using std::fabs;

      if(fabs(float_type(1) - term) < std::numeric_limits<float_type>::epsilon())
      {
        break;
      }

      z *= term;
    }

    return z;
  }
} // namespace local_zeta

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example003_zeta() -> bool
#else
auto math::wide_decimal::example003_zeta() -> bool
#endif
{
  std::deque<std::uint_fast16_t> primes(1U, UINT32_C(2));

  using local_zeta::dec51_t;

  // Get a table of the first 1000 primes.
  local_zeta::compute_primes_via_square_root(primes, UINT32_C(7920));

  // From https://functions.wolfram.com/ZetaFunctionsandPolylogarithms/Zeta/03/02/
  // we find that zeta(16) = (3617 Pi^16)/325641566250
  //                       = 1.000015282259408651871732571487636722023237388990471531...

  // Compute zeta(16).
  const auto r16 = local_zeta::zeta16<dec51_t>(primes);

  using std::fabs;

  // Check the closeness of the result.
  const dec51_t control
  {
    dec51_t(3617U * pow(local_zeta::pi<dec51_t>(), 16U)) / 325641566250ULL
  };

  const dec51_t closeness = fabs(1 - (r16 / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<dec51_t>::epsilon() * 10));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE003_ZETA)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = math::wide_decimal::example003_zeta();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
