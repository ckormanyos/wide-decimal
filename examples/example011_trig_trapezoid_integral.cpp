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

namespace local
{
  using dec51_t = math::wide_decimal::decwide_t<51U, std::uint32_t, void>;

  dec51_t sin(dec51_t x);
  dec51_t cos(dec51_t x);

  template<typename FloatType>
  FloatType pi() { return FloatType(); }

  template<>
  float pi() { return (float) 3.14159265358979323846264338327950288419716939937510582097L; }

  template<>
  double pi() { return (double) 3.14159265358979323846264338327950288419716939937510582097L; }

  template<>
  long double pi() { return (long double) 3.14159265358979323846264338327950288419716939937510582097L; }

  template<>
  dec51_t pi() { return dec51_t( { 3UL, 14159265UL, 35897932UL, 38462643UL, 38327950UL, 28841971UL, 69399375UL, 10582097UL } ); }

  template<typename FloatType>
  FloatType pi_half() { return FloatType(); }

  template<>
  float pi_half() { return (float) 1.57079632679489661923132169163975144209858469968755291049L; }

  template<>
  double pi_half() { return (double) 1.57079632679489661923132169163975144209858469968755291049L; }

  template<>
  long double pi_half() { return (long double) 1.57079632679489661923132169163975144209858469968755291049L; }

  template<>
  dec51_t pi_half() { return dec51_t( { 1UL, 57079632UL, 67948966UL, 19231321UL, 69163975UL, 14420985UL, 84699687UL, 55291049UL } ); }

  dec51_t sin_pade(dec51_t x)
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

    static const dec51_t coef_sin_top_0( {    57828UL, 92079564UL, 35639754UL, 99605577UL, 89652272UL, 18945166UL, 95040000UL }, 48U );
    static const dec51_t coef_sin_top_1( {     9043UL, 39147227UL, 52697807UL, 14387149UL, 29637016UL, 98135463UL, 83360000UL }, 48U, true );
    static const dec51_t coef_sin_top_2( {      385UL, 84634311UL, 40849087UL, 61955730UL, 26248684UL, 87944519UL, 53152000UL }, 48U );
    static const dec51_t coef_sin_top_3( {        7UL,  1815238UL, 80556594UL, 89803429UL, 45057678UL, 41267236UL, 21632000UL }, 48U, true );
    static const dec51_t coef_sin_top_4( {              6518670UL, 52853116UL,  1343595UL,  1345538UL, 36321980UL,   736000UL }, 40U );
    static const dec51_t coef_sin_top_5( {                33531UL, 31107865UL, 11542878UL, 12668334UL, 47869597UL, 95547520UL }, 40U, true );
    static const dec51_t coef_sin_top_6( {                   97UL, 23858280UL, 35106690UL, 71700713UL, 45246530UL, 36979360UL }, 40U );
    static const dec51_t coef_sin_top_7( {                         15026890UL, 94832213UL, 43593604UL, 60501216UL, 49006736UL }, 32U, true );
    static const dec51_t coef_sin_top_8( {                             9754UL, 27620658UL,  9384882UL, 73091394UL, 14158497UL }, 32U );

    static const dec51_t coef_sin_bot_0( {      377UL, 96680258UL, 59056468UL, 98690232UL, 53527139UL,  3391798UL, 47680000UL }, 48U );
    static const dec51_t coef_sin_bot_1( {        3UL, 88733329UL, 19302236UL, 28772639UL,  8727483UL, 84564414UL, 36160000UL }, 48U );
    static const dec51_t coef_sin_bot_2( {              2003705UL,  7272935UL, 39214561UL, 96408839UL, 35081781UL, 32480000UL }, 40U );
    static const dec51_t coef_sin_bot_3( {                 6820UL,  5205041UL, 87115173UL, 83286233UL, 23898656UL,  8960000UL }, 40U );
    static const dec51_t coef_sin_bot_4( {                   16UL, 93325680UL, 24472667UL, 78416104UL, 58477897UL, 31936000UL }, 40U );
    static const dec51_t coef_sin_bot_5( {                          3176263UL, 54038315UL, 11637764UL, 57072751UL, 32261760UL }, 32U );
    static const dec51_t coef_sin_bot_6( {                             4453UL, 50801787UL, 80537080UL, 55369475UL, 89710880UL }, 32U );
    static const dec51_t coef_sin_bot_7( {                                4UL, 33338433UL, 63845605UL, 88322462UL, 68252880UL }, 32U );
    static const dec51_t coef_sin_bot_8( {                                       228136UL, 24782316UL, 17485673UL,  3716089UL }, 24U );

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

  dec51_t cos_pade(dec51_t x)
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

    static const dec51_t coef_cos_top_0( {     2864UL, 38669672UL, 11840381UL, 31507642UL, 71487124UL, 48256000UL }, 40U, true );
    static const dec51_t coef_cos_top_1( {      206UL, 87239648UL, 61834717UL, 73770062UL, 47786513UL, 77280000UL }, 40U );
    static const dec51_t coef_cos_top_2( {        5UL, 48253747UL, 39699401UL, 81389360UL, 41830924UL, 80684800UL }, 40U, true );
    static const dec51_t coef_cos_top_3( {              6785676UL, 57127205UL, 41746346UL, 25361461UL, 89008000UL }, 32U );
    static const dec51_t coef_cos_top_4( {                44141UL,  3805950UL, 49222158UL, 75817034UL, 39618320UL }, 32U, true );
    static const dec51_t coef_cos_top_5( {                  156UL, 18617697UL, 34058207UL, 29180580UL, 21733000UL }, 32U );
    static const dec51_t coef_cos_top_6( {                         28694126UL, 54400268UL, 52693647UL, 86894162UL }, 24U, true );
    static const dec51_t coef_cos_top_7( {                            21704UL, 72966090UL, 42898274UL, 92622415UL }, 24U );

    static const dec51_t coef_cos_bot_0( { 23373395UL, 44524486UL, 17511531UL,  2364553UL, 34935777UL, 68960000UL }, 40U );
    static const dec51_t coef_cos_bot_1( {   259704UL, 19844314UL, 80162554UL, 61487226UL, 73292262UL,  9280000UL }, 40U );
    static const dec51_t coef_cos_bot_2( {     1453UL, 42386551UL,  2083704UL, 44798380UL, 76079290UL, 78272000UL }, 40U );
    static const dec51_t coef_cos_bot_3( {        5UL, 40151135UL, 19445709UL, 76526889UL, 92606084UL, 96384000UL }, 40U );
    static const dec51_t coef_cos_bot_4( {              1473644UL, 42364155UL,  6677632UL, 34661531UL, 42457600UL }, 32U );
    static const dec51_t coef_cos_bot_5( {                 3059UL, 38843489UL, 98650196UL, 32416479UL, 84009600UL }, 32U );
    static const dec51_t coef_cos_bot_6( {                    4UL, 78745282UL, 53390421UL, 59067765UL, 62810080UL }, 32U );
    static const dec51_t coef_cos_bot_7( {                           524970UL, 22782648UL, 79571778UL, 63426960UL }, 24U );
    static const dec51_t coef_cos_bot_8( {                              315UL,  7177895UL,  8558851UL, 68016323UL }, 24U );

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

  dec51_t sin(dec51_t x)
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

      const std::uint32_t      k = (std::uint32_t) (x / pi_half<dec51_t>());
      const std::uint_fast32_t n = (std::uint_fast32_t) (k % 4U);

      dec51_t r = x - (pi_half<dec51_t>() * k);

      const bool is_neg =  (n > 1U);
      const bool is_cos = ((n == 1U) || (n == 3U));

      std::uint_fast32_t n_angle_identity = 0U;

      static const dec51_t two_tenths = dec51_t(2U) / 10U;

      // Reduce the argument with factors of three until it is less than 2/10.
      while(r > two_tenths)
      {
        r /= 3U;

        ++n_angle_identity;
      }

      s = (is_cos ? cos_pade(r) : sin_pade(r));

      // Rescale the result with the triple angle identity for sine.
      for(std::uint_fast32_t t = 0U; t < n_angle_identity; ++t)
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

  dec51_t cos(dec51_t x)
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

      const std::uint32_t      k = (std::uint32_t) (x / pi_half<dec51_t>());
      const std::uint_fast32_t n = (std::uint_fast32_t) (k % 4U);

      dec51_t r = x - (pi_half<dec51_t>() * k);

      const bool is_neg = ((n == 1U) || (n == 2U));
      const bool is_sin = ((n == 1U) || (n == 3U));

      std::uint_fast32_t n_angle_identity = 0U;

      static const dec51_t two_tenths = dec51_t(2U) / 10U;

      // Reduce the argument with factors of three until it is less than 2/10.
      while(r > two_tenths)
      {
        r /= 3U;

        ++n_angle_identity;
      }

      c = (is_sin ? sin_pade(r) : cos_pade(r));

      // Rescale the result with the triple angle identity for cosine.
      for(std::uint_fast32_t t = 0U; t < n_angle_identity; ++t)
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

  template<typename float_type>
  float_type cyl_bessel_j(const std::uint_fast8_t n,
                          const float_type& x)
  {
    const float_type epsilon = std::numeric_limits<float_type>::epsilon();

    using std::cos;
    using std::sin;
    using std::sqrt;

    using local::cos;
    using local::sin;

    const float_type tol = sqrt(epsilon);

    const float_type integration_result = integral(float_type(0),
                                                   pi<float_type>(),
                                                   tol,
                                                   [&x, &n](const float_type& t) -> float_type
                                                   {
                                                     return cos(x * sin(t) - (t * n));
                                                   });

    const float_type jn = integration_result / pi<float_type>();

    return jn;
  }
}

bool math::wide_decimal::example011_trig_trapezoid_integral()
{
  using local::dec51_t;

  const dec51_t j2 = local::cyl_bessel_j(2U, dec51_t(123U) / 100U);
  const dec51_t j3 = local::cyl_bessel_j(3U, dec51_t(456U) / 100U);
  const dec51_t j4 = local::cyl_bessel_j(4U, dec51_t(789U) / 100U);

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
