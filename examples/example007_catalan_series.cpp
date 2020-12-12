///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;

  template<typename FloatingPointType>
  FloatingPointType pi()
  {
    return FloatingPointType(3.1415926535897932384626433832795029L);
  }

  template<>
  dec1001_t pi()
  {
    return math::wide_decimal::pi<wide_decimal_digits10>();
  }
}

namespace local
{
  template<typename FloatingPointType>
  FloatingPointType catalan()
  {
    using floating_point_type = FloatingPointType;

    // Adapted from Boost.Math.Constants (see file calculate_constants.hpp).
    // See also http://www.mpfr.org/algorithms.pdf

    floating_point_type k_fact (1U);
    floating_point_type tk_fact(2U);
    floating_point_type sum    (floating_point_type(19U) / 18U);

    const floating_point_type lim = std::numeric_limits<floating_point_type>::epsilon();

    for(std::uint_fast32_t k = UINT32_C(2); k < UINT32_C(10000000); ++k)
    {
      const std::uint32_t tk = 2U * k;
      const std::uint64_t tk_plus_one_squared = (std::uint64_t) (tk + 1U) * (tk + 1U);

      k_fact  *= k;
      tk_fact *= (std::uint64_t) tk * (tk - 1U);

      floating_point_type term = (k_fact * k_fact) / (tk_fact * tk_plus_one_squared);

      sum += term;

      if(term < lim)
      {
        break;
      }
    }

    using ::pi;
    using std::log;
    using std::sqrt;

    const floating_point_type result =
      ((pi<floating_point_type>() * log(2U + sqrt(floating_point_type(3U)))) + (sum * 3U)) / 8U;

    return result;
  }
}

bool math::wide_decimal::example007_catalan_series()
{
  const dec1001_t c = local::catalan<dec1001_t>();

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

  const bool result_is_ok = (closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example007_catalan_series();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
