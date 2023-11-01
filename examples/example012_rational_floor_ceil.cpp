///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/utility/util_baselexical_cast.h>
#include <util/utility/util_pseudorandom_time_point_seed.h>

#if defined(__clang__)
  #if defined __has_feature && (__has_feature(thread_sanitizer) || __has_feature(address_sanitizer))
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#elif defined(__GNUC__)
  #if defined(__SANITIZE_THREAD__) || defined(__SANITIZE_ADDRESS__) || defined(WIDE_DECIMAL_HAS_COVERAGE)
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#endif

namespace example012_rational
{
  auto dist_sign() -> unsigned
  {
    static std::minstd_rand0 my_eng(util::util_pseudorandom_time_point_seed::value<std::minstd_rand0::result_type>());

    static std::uniform_int_distribution<unsigned> my_dist_sign
    (
      0U,
      1U
    );

    return my_dist_sign(my_eng);
  }

  constexpr auto lo_index_min = static_cast<std::int32_t>(INT32_C(101));
  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  constexpr auto lo_index_max = static_cast<std::int32_t>(INT32_C(1010));
  #else
  constexpr auto lo_index_max = static_cast<std::int32_t>(INT32_C(210));
  #endif
  constexpr auto hi_index_min = static_cast<std::int32_t>(INT32_C(10001));
  constexpr auto hi_index_max = static_cast<std::int32_t>(INT32_C(40010));

  static_assert(hi_index_min < hi_index_max, "Error hi_index_min must be less than hi_index_max");
  static_assert(lo_index_min < lo_index_max, "Error lo_index_min must be less than lo_index_max");

  template<typename DecimalType>
  auto test_rational_floor() -> bool
  {
    using decimal_type = DecimalType;

    static_assert(std::numeric_limits<decimal_type>::digits10 < 16, // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                  "Error: This test is designed for less than 16 decimal digits");

    using decimal_type = DecimalType;

    bool result_is_ok = true;

    for(auto lo_index = lo_index_min;
             lo_index < lo_index_max;
             lo_index = static_cast<std::int32_t>(lo_index + static_cast<std::int32_t>(INT32_C(7))))
    {
      for(auto hi_index = hi_index_min;
               hi_index < hi_index_max;
               hi_index = static_cast<std::int32_t>(hi_index + static_cast<std::int32_t>(INT32_C(17))))
      {
        const auto lo_is_neg = static_cast<unsigned>(static_cast<unsigned>(dist_sign() % 2U) == static_cast<unsigned>(UINT8_C(0)));
        const auto hi_is_neg = static_cast<unsigned>(static_cast<unsigned>(dist_sign() % 2U) == static_cast<unsigned>(UINT8_C(0)));

        const auto lo = static_cast<std::int32_t>((!lo_is_neg) ? lo_index : -lo_index);
        const auto hi = static_cast<std::int32_t>((!hi_is_neg) ? hi_index : -hi_index);

        const auto lo_hi = static_cast<std::int32_t>(lo * hi);

        const decimal_type a = decimal_type { lo_hi } / decimal_type { lo };
        const decimal_type b = floor(a);

        result_is_ok = ((static_cast<std::int32_t>(a) == b) && result_is_ok);

        if(!result_is_ok)
        {
          break;
        }
      }
    }

    return result_is_ok;
  }

  template<typename DecimalType>
  auto test_rational_ceil() -> bool
  {
    using decimal_type = DecimalType;

    static_assert(std::numeric_limits<decimal_type>::digits10 < 16, // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                  "Error: This test is designed for less than 16 decimal digits");

    using decimal_type = DecimalType;

    bool result_is_ok = true;

    for(auto lo_index = lo_index_min;
             lo_index < lo_index_max;
             lo_index = static_cast<std::int32_t>(lo_index + static_cast<std::int32_t>(INT32_C(7))))
    {
      for(auto hi_index = hi_index_min;
               hi_index < hi_index_max;
               hi_index = static_cast<std::int32_t>(hi_index + static_cast<std::int32_t>(INT32_C(17))))
      {
        const auto lo_is_neg = static_cast<unsigned>(static_cast<unsigned>(dist_sign() % 2U) == static_cast<unsigned>(UINT8_C(0)));
        const auto hi_is_neg = static_cast<unsigned>(static_cast<unsigned>(dist_sign() % 2U) == static_cast<unsigned>(UINT8_C(0)));

        const auto lo = static_cast<std::int32_t>((!lo_is_neg) ? lo_index : -lo_index);
        const auto hi = static_cast<std::int32_t>((!hi_is_neg) ? hi_index : -hi_index);

        const auto lo_hi = static_cast<std::int32_t>(lo * hi);

        const decimal_type a = decimal_type { lo_hi } / decimal_type { lo };
        const decimal_type b = ceil(a);

        result_is_ok = ((static_cast<std::int32_t>(a) == b) && result_is_ok);

        if(!result_is_ok)
        {
          break;
        }
      }
    }

    return result_is_ok;
  }
} // namespace example012_rational

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example012_rational_floor_ceil() -> bool
#else
auto ::math::wide_decimal::example012_rational_floor_ceil() -> bool
#endif
{
  bool result_is_ok = true;

  // Test floor/ceil for 10 decimal digits.
  {
    #if defined(WIDE_DECIMAL_NAMESPACE)
    using decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(10)), std::uint32_t, void>;
    #else
    using decimal_type = ::math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(10)), std::uint32_t, void>;
    #endif

    result_is_ok = (example012_rational::test_rational_floor<decimal_type>() && result_is_ok);
    result_is_ok = (example012_rational::test_rational_ceil <decimal_type>() && result_is_ok);
  }

  // Test floor/ceil for 12 decimal digits.
  {
    #if defined(WIDE_DECIMAL_NAMESPACE)
    using decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(12)), std::uint32_t, void>;
    #else
    using decimal_type = ::math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(12)), std::uint32_t, void>;
    #endif

    result_is_ok = (example012_rational::test_rational_floor<decimal_type>() && result_is_ok);
    result_is_ok = (example012_rational::test_rational_ceil <decimal_type>() && result_is_ok);
  }

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE012_RATIONAL_FLOOR_CEIL)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = ::math::wide_decimal::example012_rational_floor_ceil();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
