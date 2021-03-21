///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/bindings/decwide_t.hpp>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;
}

namespace math { namespace wide_decimal {

namespace detail {

template<typename FloatingPointType>
FloatingPointType sin_series(const FloatingPointType& x)
{
        FloatingPointType term        = x;
  const FloatingPointType x2          = x * x;
        FloatingPointType sum         = x;
        bool              term_is_neg = true;
  const FloatingPointType tol         = std::numeric_limits<FloatingPointType>::epsilon() * x;

  for(std::uint32_t k = 3U; k < UINT32_C(0xFFFF); k += 2U)
  {
    term *= x2;
    term /= std::uint32_t(k * std::uint32_t(k - 1U));

    if(term < tol)
    {
      break;
    }

    term_is_neg ? sum -= term : sum += term;

    term_is_neg = (!term_is_neg);
  }

  return sum;
}

template<typename FloatingPointType>
FloatingPointType cos_series(const FloatingPointType& x)
{
  const FloatingPointType x2          = x * x;
        FloatingPointType term        = x2 / 2U;
        FloatingPointType sum         = term;
        bool              term_is_neg = true;
  const FloatingPointType tol         = std::numeric_limits<FloatingPointType>::epsilon() * x;

  for(std::uint32_t k = 4U; k < UINT32_C(0xFFFF); k += 2U)
  {
    term *= x2;
    term /= std::uint32_t(k * std::uint32_t(k - 1U));

    if(term < tol)
    {
      break;
    }

    term_is_neg ? sum -= term : sum += term;

    term_is_neg = (!term_is_neg);
  }

  return 1U - sum;
}

}

dec1001_t sin(const dec1001_t& x)
{
  dec1001_t s;

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

    const std::uint32_t      k = (std::uint32_t) (x / boost::math::constants::half_pi<dec1001_t>());
    const std::uint_fast32_t n = (std::uint_fast32_t) (k % 4U);

    dec1001_t r = x - (boost::math::constants::half_pi<dec1001_t>() * k);

    const bool is_neg =  (n > 1U);
    const bool is_cos = ((n == 1U) || (n == 3U));

    std::uint_fast32_t n_angle_identity = 0U;

    static const dec1001_t one_tenth = dec1001_t(1U) / 10U;

    // Reduce the argument with factors of three until it is less than 1/10.
    while(r > one_tenth)
    {
      r /= 3U;

      ++n_angle_identity;
    }

    s = (is_cos ? detail::cos_series(r) : detail::sin_series(r));

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

dec1001_t cos(const dec1001_t& x)
{
  dec1001_t c;

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

    const std::uint32_t      k = (std::uint32_t) (x / boost::math::constants::half_pi<dec1001_t>());
    const std::uint_fast32_t n = (std::uint_fast32_t) (k % 4U);

    dec1001_t r = x - (boost::math::constants::half_pi<dec1001_t>() * k);

    const bool is_neg = ((n == 1U) || (n == 2U));
    const bool is_sin = ((n == 1U) || (n == 3U));

    std::uint_fast32_t n_angle_identity = 0U;

    static const dec1001_t one_tenth = dec1001_t(1U) / 10U;

    // Reduce the argument with factors of three until it is less than 1/10.
    while(r > one_tenth)
    {
      r /= 3U;

      ++n_angle_identity;
    }

    c = (is_sin ? detail::sin_series(r) : detail::cos_series(r));

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
    c = dec1001_t(1U);
  }

  return c;
}

dec1001_t tan(const dec1001_t& x)
{
  return sin(x) / cos(x);
}

dec1001_t hypergeometric_2f1(const dec1001_t& a, const dec1001_t& b, const dec1001_t& c, const dec1001_t& x)
{
  // Compute the series representation of hypergeometric_2f1
  // taken from Abramowitz and Stegun 15.1.1.
  // There are no checks on input range or parameter boundaries.

  dec1001_t x_pow_n_div_n_fact(x);
  dec1001_t pochham_a         (a);
  dec1001_t pochham_b         (b);
  dec1001_t pochham_c         (c);
  dec1001_t ap                (a);
  dec1001_t bp                (b);
  dec1001_t cp                (c);

  dec1001_t h2f1 = 1 + (((pochham_a * pochham_b) / pochham_c) * x_pow_n_div_n_fact);

  const dec1001_t tol = std::numeric_limits<dec1001_t>::epsilon() * x;

  // Series expansion of hyperg_2f1(a, b; c; x).
  for(std::int32_t n = INT32_C(2); n < INT32_C(100000); ++n)
  {
    x_pow_n_div_n_fact *= x;
    x_pow_n_div_n_fact /= n;

    pochham_a *= ++ap;
    pochham_b *= ++bp;
    pochham_c *= ++cp;

    const dec1001_t term = ((pochham_a * pochham_b) / pochham_c) * x_pow_n_div_n_fact;

    if((n > 11) && (fabs(term) < tol))
    {
      break;
    }

    h2f1 += term;
  }

  return h2f1;
}

dec1001_t hypergeometric_2f1_regularized(const dec1001_t& a, const dec1001_t& b, const dec1001_t& c, const dec1001_t& x)
{
  return hypergeometric_2f1(a, b, c, x) / boost::math::tgamma(c);
}

dec1001_t pochhammer(const dec1001_t& x, const dec1001_t& a)
{
  return boost::math::tgamma(x + a) / boost::math::tgamma(x);
}

} }

namespace local
{
  template<typename FloatingPointType>
  FloatingPointType legendre_pvu(const FloatingPointType& v,
                                 const FloatingPointType& u,
                                 const FloatingPointType& x)
  {
    using std::pow;

    // See also the third series representation provided in:
    // https://functions.wolfram.com/HypergeometricFunctions/LegendreP2General/06/01/04/

    const FloatingPointType u_half       = u / 2U;
    const FloatingPointType one_minus_x  = 1U - x;
    const FloatingPointType one_minus_mu = 1U - u;

    const FloatingPointType tgamma_term   = boost::math::tgamma(one_minus_mu);
    const FloatingPointType h2f1_reg_term = hypergeometric_2f1_regularized(-v,
                                                                           1U + v,
                                                                           one_minus_mu,
                                                                           one_minus_x / 2U);

    return (pow(1U + x, u_half) * h2f1_reg_term) / pow(one_minus_x, u_half);
  }

  template<typename FloatingPointType>
  FloatingPointType legendre_qvu(const FloatingPointType& v,
                                 const FloatingPointType& u,
                                 const FloatingPointType& x)
  {
    using std::cos;
    using std::pow;
    using std::sin;

    // See also the third series representation provided in:
    // https://functions.wolfram.com/HypergeometricFunctions/LegendreQ2General/06/01/02/

    const FloatingPointType u_pi     = u * boost::math::constants::pi<FloatingPointType>();
    const FloatingPointType sin_u_pi = sin(u_pi);
    const FloatingPointType cos_u_pi = cos(u_pi);

    const FloatingPointType one_minus_x          = 1U - x;
    const FloatingPointType one_plus_x           = 1U + x;
    const FloatingPointType u_half               = u / 2U;
    const FloatingPointType one_minus_x_over_two = one_minus_x / 2U;

    const FloatingPointType one_plus_x_over_one_minus_x_pow_u_half = pow(one_plus_x / one_minus_x, u_half);

    const FloatingPointType v_plus_one =  v + 1U;

    const FloatingPointType h2f1_1 = hypergeometric_2f1_regularized(-v, v_plus_one, 1U - u, one_minus_x_over_two);
    const FloatingPointType h2f1_2 = hypergeometric_2f1_regularized(-v, v_plus_one, 1U + u, one_minus_x_over_two);

    const FloatingPointType term1 = (h2f1_1 * one_plus_x_over_one_minus_x_pow_u_half) * cos_u_pi;
    const FloatingPointType term2 = (h2f1_2 / one_plus_x_over_one_minus_x_pow_u_half) * pochhammer(v_plus_one - u, u * 2U);

    return (boost::math::constants::half_pi<FloatingPointType>() * (term1 - term2)) / sin_u_pi;
  }
}

bool math::wide_decimal::example009a_boost_math_standalone()
{
  using std::fabs;

  const dec1001_t x = dec1001_t(UINT32_C(789)) / 1000U;

  // Compute some values of the Legendre function of the second kind
  // on the real axis within the unit circle.

  const dec1001_t lpvu = local::legendre_pvu(dec1001_t(1U) / 3, dec1001_t(1U) / 7, x);
  const dec1001_t lqvu = local::legendre_qvu(dec1001_t(1U) / 3, dec1001_t(1U) / 7, x);

  // N[LegendreP[1/3, 1/7, 2, 789/1000], 1001]
  const dec1001_t control_lpvu
  {
    "0."
    "9931591854934064572568089793337657296924109412673643417874724597677037521767383011114922218212896908"
    "0027097864963936168565931417802571392659902755985572332367496479113814794086569608406498358078841079"
    "6614332253952321909228583950735229742762075393962082193481956571473030793638066743365463314212686774"
    "9705846436214183229248546560118160013959435929087793393518594318714950812356650788732887603501474515"
    "6004025852431146299389135092485894348077166693965037523229349437595551471389905323765440198747406384"
    "7110644718744990985775015222792213207021714039394865333620745229521299594761662471248344570118744500"
    "7704200859337570117525726782130267734112267915875216713886079342015849430715707275265907079075801589"
    "9475349854755219148506164974284035858053125225329876755631039303090095663330665771069643631805565017"
    "9727332815465053842209475384208231035618687598506479237119775461739092129167925542731334863321783844"
    "1556064262945029582348726229003376197479146725615623608519444682192209137686438989212000029759855669"
    "1"
  };

  // N[LegendreQ[1/3, 1/7, 2, 789/1000], 1001]
  const dec1001_t control_lqvu
  {
    "0."
    "1802701358635473503357654947586116081212814896218637834466278197869512252395895222740695429982146035"
    "6031050553694633844449903989916722532336371811084898600594152868857308967282179462211522993788162867"
    "7937940705666514125775695969967978227378780279613276198008930643396707125474811188759254517278724120"
    "7389289773410911722431603383521650557365445713405684637195534839239774206409352127340544908988632105"
    "4776247480393326238840576035618210568727854433323584609583906187077896326821742487572480458213064013"
    "1294415389805610364101254712548823884952831764415986558963073042187229383073285433144958849261339379"
    "0888456281955232772521261719386944579027738990521656069899209895510911292249112861615412603542600625"
    "5493560671059547162388837704126463700356368628825425175294509942750482619888824124287573395907950466"
    "7777749042472348446167661381721631967592025204419250011417080752961739993679046744726634374246832558"
    "5282958111218866577533906371773555762994157451873992840942126875958383079095536901373567200448533247"
    "7"
  };

  const dec1001_t closeness_lpvu = fabs(1 - (lpvu / control_lpvu));
  const dec1001_t closeness_lqvu = fabs(1 - (lqvu / control_lqvu));

  const bool result_lpvu_is_ok = closeness_lpvu < (std::numeric_limits<dec1001_t>::epsilon() * UINT32_C(1000000));
  const bool result_lqvu_is_ok = closeness_lqvu < (std::numeric_limits<dec1001_t>::epsilon() * UINT32_C(1000000));

  const bool result_is_ok = (result_lpvu_is_ok && result_lqvu_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example009a_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
