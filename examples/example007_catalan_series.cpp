///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>

namespace example007_catalan
{
  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1001);

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using dec1001_t = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #else
  using dec1001_t = ::math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #endif

  template<typename FloatingPointType>
  auto pi() -> FloatingPointType
  {
    return static_cast<FloatingPointType>(3.1415926535897932384626433832795029L); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  template<>
  auto pi() -> dec1001_t
  {
    #if defined(WIDE_DECIMAL_NAMESPACE)
    return WIDE_DECIMAL_NAMESPACE::math::wide_decimal::pi<wide_decimal_digits10>();
    #else
    return ::math::wide_decimal::pi<wide_decimal_digits10>();
    #endif
  }

  template<typename FloatingPointType>
  auto catalan() -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    // Adapted from Boost.Math.Constants (see file calculate_constants.hpp).
    // See also http://www.mpfr.org/algorithms.pdf

    floating_point_type k_fact (1U);
    floating_point_type tk_fact(2U);
    floating_point_type sum    (static_cast<floating_point_type>(19U) / 18U); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    const floating_point_type lim = std::numeric_limits<floating_point_type>::epsilon();

    for(auto k = static_cast<std::uint_fast32_t>(2U); k < UINT32_C(10000000); ++k)
    {
      const auto tk          = static_cast<std::uint32_t>(2U * k);
      const auto tk_plus_one = static_cast<std::uint32_t>(tk + 1U);

      const auto tk_plus_one_squared =
        static_cast<std::uint64_t>
        (
          static_cast<std::uint64_t>(tk_plus_one) * tk_plus_one
        );

      k_fact  *= k;
      tk_fact *= static_cast<std::uint64_t>(static_cast<std::uint64_t>(tk) * (tk - 1U));

      floating_point_type term = (k_fact * k_fact) / (tk_fact * tk_plus_one_squared);

      sum += term;

      if(term < lim)
      {
        break;
      }
    }

    using example007_catalan::pi;
    using std::log;
    using std::sqrt;

    return (((pi<floating_point_type>() * log(2U + sqrt(static_cast<floating_point_type>(3U)))) + (sum * 3U)) / 8U); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }
} // namespace example007_catalan

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example007_catalan_series() -> bool
#else
auto ::math::wide_decimal::example007_catalan_series() -> bool
#endif
{
  using example007_catalan::dec1001_t;

  const auto c = example007_catalan::catalan<dec1001_t>();

  const dec1001_t control
  {
    "0."
    "9159655941772190150546035149323841107741493742816721342664981196217630197762547694793565129261151062"
    "4857442261919619957903589880332585905943159473748115840699533202877331946051903872747816408786590902"
    "4706484152163000228727640942388259957741508816397470252482011560707644883807873370489900864775113225"
    "9971343407485407553230768565335768095835260219382323950800720680355761048235733942319149829836189977"
    "0690364041808621794110191753274314997823397610551224779530324875371878665828082360570225594194818097"
    "5350971131571261580424272363643985001738287597797653068370092980873887495610893659771940968726844441"
    "6680462162433986483891628044828150627302274207388431172218272190472255870531908685735423498539498309"
    "9191159673884645086151524996242370437451777372351775440708538464401321748392999947572446199754961975"
    "8706400747487070149093767887304586997986064487497464387206238513712392736304998503539223928787979063"
    "3644032354784535851927777787270906083031994301332316712476158709792455479119092126201854803963934243"
    "5"
  };

  const dec1001_t closeness = fabs(1 - (c / control));

  const auto result_is_ok = (closeness < (std::numeric_limits<dec1001_t>::epsilon() * static_cast<std::uint32_t>(UINT8_C(10))));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE007_CATALAN_SERIES)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = ::math::wide_decimal::example007_catalan_series();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
