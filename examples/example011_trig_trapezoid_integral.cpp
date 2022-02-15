///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdint>
#include <limits>

#include <math/wide_decimal/decwide_t.h>
#include <math/wide_decimal/decwide_t_examples.h>

namespace example011_trig
{
  #if defined(WIDE_DECIMAL_NAMESPACE)
  using dec51_t = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<51U, std::uint32_t, void>;
  #else
  using dec51_t = math::wide_decimal::decwide_t<51U, std::uint32_t, void>;
  #endif

  auto sin(const dec51_t& x) -> dec51_t;
  auto cos(const dec51_t& x) -> dec51_t;

  template<typename FloatType>
  auto pi() -> FloatType { return FloatType(); }

  template<>
  auto pi() -> float { return static_cast<float>(3.14159265358979323846264338327950288419716939937510582097L); }

  template<>
  auto pi() -> double { return static_cast<double>(3.14159265358979323846264338327950288419716939937510582097L); }

  template<>
  auto pi() -> long double { return 3.14159265358979323846264338327950288419716939937510582097L; }

  // N[Pi, 57]
  template<>
  auto pi() -> dec51_t { return dec51_t( "3.14159265358979323846264338327950288419716939937510582097"); }

  template<typename FloatType>
  auto pi_half() -> FloatType { return FloatType(); }

  template<>
  auto pi_half() -> float { return static_cast<float>(1.57079632679489661923132169163975144209858469968755291049L); }

  template<>
  auto pi_half() -> double { return static_cast<double>(1.57079632679489661923132169163975144209858469968755291049L); }

  template<>
  auto pi_half() -> long double { return 1.57079632679489661923132169163975144209858469968755291049L; }

  // N[Pi/2, 57]
  template<>
  auto pi_half() -> dec51_t { return dec51_t("1.57079632679489661923132169163975144209858469968755291049"); }

  auto sin_pade(const dec51_t& x) -> dec51_t
  {
    // PadeApproximant[Sin[x], {x, 0, {17,16}}]
    // FullSimplify[%]

    // (x   (  57828920795643563975499605577896522721894516695040000
    //       -  9043391472275269780714387149296370169813546383360000 x^2
    //       +   385846343114084908761955730262486848794451953152000 x^4
    //       -     7018152388055659489803429450576784126723621632000 x^6
    //       +       65186705285311601343595013455383632198000736000 x^8
    //       -         335313110786511542878126683344786959795547520 x^10
    //       +            972385828035106690717007134524653036979360 x^12
    //       -              1502689094832213435936046050121649006736 x^14
    //       +                  975427620658093848827309139414158497 x^16))
    // /
    // (153 (  377966802585905646898690232535271390339179847680000
    //       +   3887333291930223628772639087274838456441436160000 x^2
    //       +     20037050727293539214561964088393508178132480000 x^4
    //       +        68200520504187115173832862332389865608960000 x^6
    //       +          169332568024472667784161045847789731936000 x^8
    //       +             317626354038315116377645707275132261760 x^10
    //       +                445350801787805370805536947589710880 x^12
    //       +                   433338433638456058832246268252880 x^14
    //       +                      228136247823161748567303716089 x^16))

    static const dec51_t coef_sin_top_0("+57828920795643563975499605577896522721894516695040000");
    static const dec51_t coef_sin_top_1("-9043391472275269780714387149296370169813546383360000");
    static const dec51_t coef_sin_top_2("+385846343114084908761955730262486848794451953152000");
    static const dec51_t coef_sin_top_3("-7018152388055659489803429450576784126723621632000");
    static const dec51_t coef_sin_top_4("+65186705285311601343595013455383632198000736000");
    static const dec51_t coef_sin_top_5("-335313110786511542878126683344786959795547520");
    static const dec51_t coef_sin_top_6("+972385828035106690717007134524653036979360");
    static const dec51_t coef_sin_top_7("-1502689094832213435936046050121649006736");
    static const dec51_t coef_sin_top_8("+975427620658093848827309139414158497");

    static const dec51_t coef_sin_bot_0("+377966802585905646898690232535271390339179847680000");
    static const dec51_t coef_sin_bot_1("+3887333291930223628772639087274838456441436160000");
    static const dec51_t coef_sin_bot_2("+20037050727293539214561964088393508178132480000");
    static const dec51_t coef_sin_bot_3("+68200520504187115173832862332389865608960000");
    static const dec51_t coef_sin_bot_4("+169332568024472667784161045847789731936000");
    static const dec51_t coef_sin_bot_5("+317626354038315116377645707275132261760");
    static const dec51_t coef_sin_bot_6("+445350801787805370805536947589710880");
    static const dec51_t coef_sin_bot_7("+433338433638456058832246268252880");
    static const dec51_t coef_sin_bot_8("+228136247823161748567303716089");

    const dec51_t x2(x * x);

    const dec51_t top = ((((((((     + coef_sin_top_8
                                * x2 + coef_sin_top_7)
                                * x2 + coef_sin_top_6)
                                * x2 + coef_sin_top_5)
                                * x2 + coef_sin_top_4)
                                * x2 + coef_sin_top_3)
                                * x2 + coef_sin_top_2)
                                * x2 + coef_sin_top_1)
                                * x2 + coef_sin_top_0);

    const dec51_t bot = ((((((((     + coef_sin_bot_8
                                * x2 + coef_sin_bot_7)
                                * x2 + coef_sin_bot_6)
                                * x2 + coef_sin_bot_5)
                                * x2 + coef_sin_bot_4)
                                * x2 + coef_sin_bot_3)
                                * x2 + coef_sin_bot_2)
                                * x2 + coef_sin_bot_1)
                                * x2 + coef_sin_bot_0);

    return (x * top) / (bot * 153U);
  }

  auto cos_pade(const dec51_t& x) -> dec51_t
  {
    // PadeApproximant[Cos[x] - 1, {x, 0, {16,16}}]
    // FullSimplify[%]

    // (4080 x^2
    //   (- 28643866967211840381315076427148712448256000
    //    +  2068723964861834717737700624778651377280000 x^2
    //    -    54825374739699401813893604183092480684800 x^4
    //    +      678567657127205417463462536146189008000 x^6
    //    -        4414103805950492221587581703439618320 x^8
    //    +          15618617697340582072918058021733000 x^10
    //    -             28694126544002685269364786894162 x^12
    //    +                21704729660904289827492622415 x^14))
    //   /
    //   (  233733954452448617511531023645533493577768960000
    //    +   2597041984431480162554614872267329226209280000 x^2
    //    +     14534238655102083704447983807607929078272000 x^4
    //    +        54015113519445709765268899260608496384000 x^6
    //    +          147364442364155066776323466153142457600 x^8
    //    +             305938843489986501963241647984009600 x^10
    //    +                478745282533904215906776562810080 x^12
    //    +                   524970227826487957177863426960 x^14
    //    +                      315071778950855885168016323 x^16)

    static const dec51_t coef_cos_top_0("-28643866967211840381315076427148712448256000");
    static const dec51_t coef_cos_top_1("+2068723964861834717737700624778651377280000");
    static const dec51_t coef_cos_top_2("-54825374739699401813893604183092480684800");
    static const dec51_t coef_cos_top_3("+678567657127205417463462536146189008000");
    static const dec51_t coef_cos_top_4("-4414103805950492221587581703439618320");
    static const dec51_t coef_cos_top_5("+15618617697340582072918058021733000");
    static const dec51_t coef_cos_top_6("-28694126544002685269364786894162");
    static const dec51_t coef_cos_top_7("+21704729660904289827492622415");

    static const dec51_t coef_cos_bot_0("+233733954452448617511531023645533493577768960000");
    static const dec51_t coef_cos_bot_1("+2597041984431480162554614872267329226209280000");
    static const dec51_t coef_cos_bot_2("+14534238655102083704447983807607929078272000");
    static const dec51_t coef_cos_bot_3("+54015113519445709765268899260608496384000");
    static const dec51_t coef_cos_bot_4("+147364442364155066776323466153142457600");
    static const dec51_t coef_cos_bot_5("+305938843489986501963241647984009600");
    static const dec51_t coef_cos_bot_6("+478745282533904215906776562810080");
    static const dec51_t coef_cos_bot_7("+524970227826487957177863426960");
    static const dec51_t coef_cos_bot_8("+315071778950855885168016323");

    const dec51_t x2(x * x);

    const dec51_t top =  (((((((     + coef_cos_top_7
                                * x2 + coef_cos_top_6)
                                * x2 + coef_cos_top_5)
                                * x2 + coef_cos_top_4)
                                * x2 + coef_cos_top_3)
                                * x2 + coef_cos_top_2)
                                * x2 + coef_cos_top_1)
                                * x2 + coef_cos_top_0);

    const dec51_t bot = ((((((((     + coef_cos_bot_8
                                * x2 + coef_cos_bot_7)
                                * x2 + coef_cos_bot_6)
                                * x2 + coef_cos_bot_5)
                                * x2 + coef_cos_bot_4)
                                * x2 + coef_cos_bot_3)
                                * x2 + coef_cos_bot_2)
                                * x2 + coef_cos_bot_1)
                                * x2 + coef_cos_bot_0);

    return 1U + (((x2 * 4080U) * top) / bot);
  }

  auto sin(const dec51_t& x) -> dec51_t // NOLINT(misc-no-recursion)
  {
    dec51_t s;

    if(x < 0)
    {
      s = -sin(-x);
    }
    else if(x > 0)
    {
      // Perform argument reduction and subsequent scaling of the result.

      // Given x = k * (pi/2) + r, compute n = (k % 4).

      // | n |  sin(x) |  cos(x) |  sin(x)/cos(x) |
      // |----------------------------------------|
      // | 0 |  sin(r) |  cos(r) |  sin(r)/cos(r) |
      // | 1 |  cos(r) | -sin(r) | -cos(r)/sin(r) |
      // | 2 | -sin(r) | -cos(r) |  sin(r)/cos(r) |
      // | 3 | -cos(r) |  sin(r) | -cos(r)/sin(r) |

      const auto k = static_cast<std::uint32_t>     (x / pi_half<dec51_t>());
      const auto n = static_cast<std::uint_fast32_t>(k % 4U);

      dec51_t r = x - (pi_half<dec51_t>() * k);

      const bool is_neg =  (n > 1U);
      const bool is_cos = ((n == 1U) || (n == 3U));

      auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

      static const dec51_t two_tenths = dec51_t(2U) / 10U;

      // Reduce the argument with factors of three until it is less than 2/10.
      while(r > two_tenths)
      {
        r /= 3U;

        ++n_angle_identity;
      }

      s = (is_cos ? cos_pade(r) : sin_pade(r));

      // Rescale the result with the triple angle identity for sine.
      for(auto t = static_cast<std::uint_fast32_t>(0U); t < n_angle_identity; ++t)
      {
        s = (s * 3U) - (((s * s) * s) * 4U);
      }

      s = fabs(s);

      if(is_neg)
      {
        s = -s;
      }
    }
    else
    {
      s = 0;
    }

    return s;
  }

  auto cos(const dec51_t& x) -> dec51_t // NOLINT(misc-no-recursion)
  {
    dec51_t c;

    if(x < 0)
    {
      c = cos(-x);
    }
    else if(x > 0)
    {
      // Perform argument reduction and subsequent scaling of the result.

      // Given x = k * (pi/2) + r, compute n = (k % 4).

      // | n |  sin(x) |  cos(x) |  sin(x)/cos(x) |
      // |----------------------------------------|
      // | 0 |  sin(r) |  cos(r) |  sin(r)/cos(r) |
      // | 1 |  cos(r) | -sin(r) | -cos(r)/sin(r) |
      // | 2 | -sin(r) | -cos(r) |  sin(r)/cos(r) |
      // | 3 | -cos(r) |  sin(r) | -cos(r)/sin(r) |

      const auto k = static_cast<std::uint32_t>     (x / pi_half<dec51_t>());
      const auto n = static_cast<std::uint_fast32_t>(k % 4U);

      dec51_t r = x - (pi_half<dec51_t>() * k);

      const bool is_neg = ((n == 1U) || (n == 2U));
      const bool is_sin = ((n == 1U) || (n == 3U));

      auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

      static const dec51_t two_tenths = dec51_t(2U) / 10U;

      // Reduce the argument with factors of three until it is less than 2/10.
      while(r > two_tenths)
      {
        r /= 3U;

        ++n_angle_identity;
      }

      c = (is_sin ? sin_pade(r) : cos_pade(r));

      // Rescale the result with the triple angle identity for cosine.
      for(auto t = static_cast<std::uint_fast32_t>(0U); t < n_angle_identity; ++t)
      {
        c = (((c * c) * c) * 4U) - (c * 3U);
      }

      c = fabs(c);

      if(is_neg)
      {
        c = -c;
      }
    }
    else
    {
      c = dec51_t(1U);
    }

    return c;
  }

  template<typename real_value_type,
           typename real_function_type>
  auto integral(const real_value_type&   a,
                const real_value_type&   b,
                const real_value_type&   tol,
                      real_function_type real_function) -> real_value_type
  {
    auto n2 = static_cast<std::uint_fast32_t>(1);

    real_value_type step = ((b - a) / 2U);

    real_value_type result = (real_function(a) + real_function(b)) * step;

    const auto k_max = static_cast<std::uint_fast8_t>(32U);

    for(auto k = static_cast<std::uint_fast8_t>(0U); k < k_max; ++k)
    {
      real_value_type sum(0);

      for(auto j = static_cast<std::uint_fast32_t>(0U); j < n2; ++j)
      {
        const auto two_j_plus_one =
          static_cast<std::uint_fast32_t>
          (
            static_cast<std::uint_fast32_t>(j * UINT32_C(2)) + UINT32_C(1)
          );

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

  template<typename float_type>
  auto cyl_bessel_j(      std::uint_fast8_t n,
                    const float_type&       x) -> float_type
  {
    const float_type epsilon = std::numeric_limits<float_type>::epsilon();

    using std::cos;
    using std::sin;
    using std::sqrt;

    using example011_trig::cos;
    using example011_trig::sin;

    const float_type tol = sqrt(epsilon);

    const float_type integration_result = integral(float_type(0),
                                                   pi<float_type>(),
                                                   tol,
                                                   [&x, &n](const float_type& t) -> float_type
                                                   {
                                                     return cos(x * sin(t) - (t * n));
                                                   });

    return integration_result / pi<float_type>();
  }
} // namespace example011_trig

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example011_trig_trapezoid_integral() -> bool
#else
auto math::wide_decimal::example011_trig_trapezoid_integral() -> bool
#endif
{
  using example011_trig::dec51_t;

  const dec51_t j2 = example011_trig::cyl_bessel_j(2U, dec51_t(123U) / 100U);
  const dec51_t j3 = example011_trig::cyl_bessel_j(3U, dec51_t(456U) / 100U);
  const dec51_t j4 = example011_trig::cyl_bessel_j(4U, dec51_t(789U) / 100U);

  using std::fabs;

  // N[BesselJ[2, 123/100], 60]
  const dec51_t control2 = dec51_t("0.166369383786814073512678524315131594371033482453328555149562");

  // N[BesselJ[3, 456/100], 60]
  const dec51_t control3 = dec51_t("0.420388204867652161626134623430784757427481712020578485761744");

  // N[BesselJ[4, 789/100], 60]
  const dec51_t control4 = dec51_t("-0.0785068635721274384104855203288065696173271865920904171127153");

  using std::fabs;

  const dec51_t closeness2 = fabs(1 - (j2 / control2));
  const dec51_t closeness3 = fabs(1 - (j3 / control3));
  const dec51_t closeness4 = fabs(1 - (j4 / control4));

  const dec51_t tol = std::numeric_limits<dec51_t>::epsilon() * 10U;

  const bool result_is_ok = (   (closeness2 < tol)
                             && (closeness3 < tol)
                             && (closeness4 < tol));

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example011_trig_trapezoid_integral();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
