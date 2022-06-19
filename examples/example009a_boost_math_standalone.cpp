///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <boost/version.hpp>

#if !defined(BOOST_VERSION)
#error BOOST_VERSION is not defined. Ensure that <boost/version.hpp> is properly included.
#endif

#if (BOOST_VERSION >= 108000)
#if !defined(BOOST_NO_EXCEPTIONS)
#define BOOST_NO_EXCEPTIONS
#endif
#if !defined(BOOST_NO_RTTI)
#define BOOST_NO_RTTI
#endif
#endif

#if ((BOOST_VERSION >= 107700) && !defined(BOOST_MATH_STANDALONE))
#if (defined(_MSC_VER) && (_MSC_VER < 1920))
#else
#define BOOST_MATH_STANDALONE
#endif
#endif

#if ((BOOST_VERSION >= 107900) && !defined(BOOST_MP_STANDALONE))
#define BOOST_MP_STANDALONE
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif

#if (BOOST_VERSION < 107900)
#include <boost/math/policies/error_handling.hpp>
#include <boost/throw_exception.hpp>
#endif

#include <boost/math/bindings/decwide_t.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <examples/example_decwide_t.h>

namespace example009a_boost
{
  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1001);

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using dec1001_t = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #else
  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;
  #endif
} // namespace example009a_boost

WIDE_DECIMAL_NAMESPACE_BEGIN

#if(__cplusplus >= 201703L)
namespace math::wide_decimal {
#else
namespace math { namespace wide_decimal { // NOLINT(modernize-concat-nested-namespaces)
#endif

namespace detail {

template<typename FloatingPointType>
auto sin_series(const FloatingPointType& x) -> FloatingPointType
{
  using floating_point_type = FloatingPointType;

        auto term        = x;
  const auto x2          = x * x;
        auto sum         = x;
        auto term_is_neg = true;
  const auto tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

  for(auto k = static_cast<std::uint32_t>(UINT32_C(3));
           k < static_cast<std::uint32_t>(UINT32_C(10000));
           k = static_cast<std::uint32_t>(k + static_cast<std::uint32_t>(UINT8_C(2))))
  {
    const auto k_times_k_minus_one =
      static_cast<std::uint32_t>
      (
        k * static_cast<std::uint32_t>(k - static_cast<std::uint32_t>(UINT8_C(1)))
      );

    term *= x2;
    term /= k_times_k_minus_one;

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
auto cos_series(const FloatingPointType& x) -> FloatingPointType
{
  using floating_point_type = FloatingPointType;

  const auto x2          = x * x;
        auto term        = x2 / 2U;
        auto sum         = term;
        auto term_is_neg = true;
  const auto tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

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

} // namespace detail

using example009a_boost::dec1001_t;

auto sin(const dec1001_t& x) -> dec1001_t // NOLINT(misc-no-recursion)
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

    const auto k = static_cast<std::uint32_t>     (x / boost::math::constants::half_pi<dec1001_t>());
    const auto n = static_cast<std::uint_fast32_t>(k % 4U);

    dec1001_t r = x - (::boost::math::constants::half_pi<dec1001_t>() * k);

    const auto is_neg =  (n > 1U);
    const auto is_cos = ((n == 1U) || (n == 3U));

    auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

    static const dec1001_t one_tenth = dec1001_t(1U) / 10U;

    // Reduce the argument with factors of three until it is less than 1/10.
    while(r > one_tenth)
    {
      r /= 3U;

      ++n_angle_identity;
    }

    s = (is_cos ? detail::cos_series(r) : detail::sin_series(r));

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

auto cos(const dec1001_t& x) -> dec1001_t // NOLINT(misc-no-recursion)
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

    const auto k = static_cast<std::uint32_t>     (x / ::boost::math::constants::half_pi<dec1001_t>());
    const auto n = static_cast<std::uint_fast32_t>(k % 4U);

    dec1001_t r = x - (::boost::math::constants::half_pi<dec1001_t>() * k);

    const auto is_neg = ((n == 1U) || (n == 2U));
    const auto is_sin = ((n == 1U) || (n == 3U));

    auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

    static const dec1001_t one_tenth = dec1001_t(1U) / 10U;

    // Reduce the argument with factors of three until it is less than 1/10.
    while(r > one_tenth)
    {
      r /= 3U;

      ++n_angle_identity;
    }

    c = (is_sin ? detail::sin_series(r) : detail::cos_series(r));

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
    c = dec1001_t(1U);
  }

  return c;
}

auto hypergeometric_2f1(const dec1001_t& a, // NOLINT(bugprone-easily-swappable-parameters)
                        const dec1001_t& b,
                        const dec1001_t& c,
                        const dec1001_t& x) -> dec1001_t
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
  for(auto n = static_cast<std::int32_t>(2); n < INT32_C(100000); ++n)
  {
    x_pow_n_div_n_fact *= x;
    x_pow_n_div_n_fact /= n;

    pochham_a *= ++ap;
    pochham_b *= ++bp;
    pochham_c *= ++cp;

    const dec1001_t term = ((pochham_a * pochham_b) / pochham_c) * x_pow_n_div_n_fact;

    if((n > static_cast<std::int32_t>(INT32_C(11))) && (fabs(term) < tol))
    {
      break;
    }

    h2f1 += term;
  }

  return h2f1;
}

auto hypergeometric_2f1_regularized(const dec1001_t& a,
                                    const dec1001_t& b,
                                    const dec1001_t& c,
                                    const dec1001_t& x) -> dec1001_t
{
  return hypergeometric_2f1(a, b, c, x) / ::boost::math::tgamma(c);
}

auto pochhammer(const dec1001_t& x, const dec1001_t& a) -> dec1001_t
{
  return ::boost::math::tgamma(x + a) / ::boost::math::tgamma(x);
}

#if(__cplusplus >= 201703L)
} // namespace math::wide_decimal
#else
} // namespace wide_decimal
} // namespace math
#endif

WIDE_DECIMAL_NAMESPACE_END

namespace example009a_boost
{
  template<typename FloatingPointType>
  auto legendre_pvu(const FloatingPointType& v, // NOLINT(bugprone-easily-swappable-parameters)
                    const FloatingPointType& u,
                    const FloatingPointType& x) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    using std::pow;

    // See also the third series representation provided in:
    // https://functions.wolfram.com/HypergeometricFunctions/LegendreP2General/06/01/04/

    const floating_point_type u_half       = u / 2U;
    const floating_point_type one_minus_x  = 1U - x;
    const floating_point_type one_minus_mu = 1U - u;

    const floating_point_type h2f1_reg_term = hypergeometric_2f1_regularized(-v,
                                                                             1U + v,
                                                                             one_minus_mu,
                                                                             one_minus_x / 2U);

    return (pow(1U + x, u_half) * h2f1_reg_term) / pow(one_minus_x, u_half);
  }

  template<typename FloatingPointType>
  auto legendre_qvu(const FloatingPointType& v,                      // NOLINT(bugprone-easily-swappable-parameters)
                    const FloatingPointType& u,                      // NOLINT(bugprone-easily-swappable-parameters)
                    const FloatingPointType& x) -> FloatingPointType // NOLINT(bugprone-easily-swappable-parameters)
  {
    using std::cos;
    using std::pow;
    using std::sin;

    // See also the third series representation provided in:
    // https://functions.wolfram.com/HypergeometricFunctions/LegendreQ2General/06/01/02/

    using floating_point_type = FloatingPointType;

    const auto u_pi     = u * ::boost::math::constants::pi<floating_point_type>();
    const auto sin_u_pi = sin(u_pi);
    const auto cos_u_pi = cos(u_pi);

    const auto one_local              (1U);
    const auto one_minus_x          = one_local - x;
    const auto one_plus_x           = one_local + x;
    const auto u_half               = u / 2U;
    const auto one_minus_x_over_two = one_minus_x / 2U;

    const auto one_plus_x_over_one_minus_x_pow_u_half = pow(one_plus_x / one_minus_x, u_half);

    const auto v_plus_one =  v + one_local;
    const auto minus_v    = -v;

    const auto h2f1_1 = hypergeometric_2f1_regularized(minus_v, v_plus_one, one_local - u, one_minus_x_over_two);
    const auto h2f1_2 = hypergeometric_2f1_regularized(minus_v, v_plus_one, one_local + u, one_minus_x_over_two);

    const auto term1 = (h2f1_1 * one_plus_x_over_one_minus_x_pow_u_half) * cos_u_pi;
    const auto term2 = (h2f1_2 / one_plus_x_over_one_minus_x_pow_u_half) * pochhammer(v_plus_one - u, u * 2U);

    return (::boost::math::constants::half_pi<floating_point_type>() * (term1 - term2)) / sin_u_pi;
  }
} // namespace example009a_boost

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example009a_boost_math_standalone() -> bool
#else
auto math::wide_decimal::example009a_boost_math_standalone() -> bool
#endif
{
  using std::fabs;

  using example009a_boost::dec1001_t;

  #if (BOOST_VERSION < 107900)
  using boost_wrapexcept_round_type  = ::boost::wrapexcept<::boost::math::rounding_error>;
  using boost_wrapexcept_domain_type = ::boost::wrapexcept<std::domain_error>;
  #endif

  auto result_is_ok = false;

  #if (BOOST_VERSION >= 108000)
  #else
  try
  {
  #endif
  const dec1001_t x = dec1001_t(UINT32_C(789)) / 1000U;

  // Compute some values of the Legendre function of the second kind
  // on the real axis within the unit circle.

  const dec1001_t lpvu            = example009a_boost::legendre_pvu(dec1001_t(1U) / 3, dec1001_t(1U) / 7, x);
  const dec1001_t lqvu            = example009a_boost::legendre_qvu(dec1001_t(1U) / 3, dec1001_t(1U) / 7, x);
  const dec1001_t lpvu_negative_u = example009a_boost::legendre_pvu(dec1001_t(1U) / 3, dec1001_t(-1) / 7, x);
  const dec1001_t lqvu_negative_u = example009a_boost::legendre_qvu(dec1001_t(1U) / 3, dec1001_t(-1) / 7, x);

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

  // N[LegendreP[1/3, -1/7, 2, 789/1000], 1001]
  const dec1001_t control_lpvu_negative_u
  {
    "0."
    "8784603450982651787800193995179712668708811457628934597069677439917677235389487601183429873349313572"
    "1740112239597751923750847879370888966990224706823959760139949980471385814793974033995303449488090611"
    "4835091952533811596610218105241362688910341734149671735011558314729990018835764773704843032819536516"
    "3427819614125862752426028897136351753538070819633813965759212017737617248661420825758620777154203107"
    "8529752834189210596448426765785288762304216302213625296924365237041125679365420108990315253666959048"
    "1435010679760671424858403744853181368320817779704621904906683182320616519700118110780346355100939602"
    "3666343052640131368131413282079721988576944822856699190960381767814254150505256223829744577430684348"
    "7615286073485757138919611235447550887660599440881376559989600453238727847889637748958394337404748335"
    "8126213472100260218157672024002607566152107416082112148105488946036130500927944538748674420153147694"
    "3387138845332690854023062463844788180014939062235033447136937086798813402498484392012262146506984403"
    "9"
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

  // N[LegendreQ[1/3, -1/7, 2, 789/1000], 1001]
  const dec1001_t control_lqvu_negative_u
  {
    "0."
    "8725211798058021771020437712630274674510605544936385767210699251990220983760867392035886500465239358"
    "5417166775183662657646854549585852386308610253338303575406726670063416304908174968679863283339896616"
    "7716921817149455344218430163168276810110575001709890380061731701990040371701539625585858777374153674"
    "6570275108580868300613942573513435764291683597723190662341537414213341532020483037211359461130834501"
    "5136614688198495110325698727256447719344118750646683458729348535478679798432192166201638040012463513"
    "4895321901853406483455973972927115251438009637396499519247428768545007861413787813604619784592425204"
    "5279957452771829231458047192383541514732146863129614155264589649487716635801980297565852973957207341"
    "6689032740571607241657154480359873886369995517919732624947780435224341886972623320429349142534543787"
    "2520819207390651864811028584879681945619590803413012623251077541425238440151370427449131127112546451"
    "4843629210732552523268500297159323480073985043542756370904205258560162593093044761251062956759800893"
    "5"
  };

  const dec1001_t closeness_lpvu            = fabs(1 - (lpvu            / control_lpvu));
  const dec1001_t closeness_lqvu            = fabs(1 - (lqvu            / control_lqvu));
  const dec1001_t closeness_lpvu_negative_u = fabs(1 - (lpvu_negative_u / control_lpvu_negative_u));
  const dec1001_t closeness_lqvu_negative_u = fabs(1 - (lqvu_negative_u / control_lqvu_negative_u));

  const auto result_lpvu_is_ok            = (closeness_lpvu            < (std::numeric_limits<dec1001_t>::epsilon() * static_cast<std::uint32_t>(UINT32_C(1000000))));
  const auto result_lqvu_is_ok            = (closeness_lqvu            < (std::numeric_limits<dec1001_t>::epsilon() * static_cast<std::uint32_t>(UINT32_C(1000000))));
  const auto result_lpvu_negative_u_is_ok = (closeness_lpvu_negative_u < (std::numeric_limits<dec1001_t>::epsilon() * static_cast<std::uint32_t>(UINT32_C(1000000))));
  const auto result_lqvu_negative_u_is_ok = (closeness_lqvu_negative_u < (std::numeric_limits<dec1001_t>::epsilon() * static_cast<std::uint32_t>(UINT32_C(1000000))));

  result_is_ok = (
                      result_lpvu_is_ok
                   && result_lqvu_is_ok
                   && result_lpvu_negative_u_is_ok
                   && result_lqvu_negative_u_is_ok
                 );

  #if (BOOST_VERSION >= 108000)
  #else
  }
  #if (BOOST_VERSION < 107900)
  catch(const boost_wrapexcept_round_type& e)
  {
    result_is_ok = false;

    std::cout << "Exception: boost_wrapexcept_round_type: " << e.what() << std::endl;
  }
  catch(const boost_wrapexcept_domain_type& e)
  {
    result_is_ok = false;

    std::cout << "Exception: boost_wrapexcept_domain_type: " << e.what() << std::endl;
  }
  #else
  // LCOV_EXCL_START
  catch(const ::boost::math::rounding_error& e)
  {
    result_is_ok = false;

    std::cout << "Exception: ::boost::math::rounding_error: " << e.what() << std::endl;
  }
  catch(const std::domain_error& e)
  {
    result_is_ok = false;

    std::cout << "Exception: std::domain_error: " << e.what() << std::endl;
  }
  // LCOV_EXCL_STOP
  #endif
  #endif

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE009A_BOOST_MATH_STANDALONE)

#include <iomanip>
#include <iostream>

// TBD: Handle exception catching in example009a_boost_math_standalone at a later time.
auto main() -> int // NOLINT(bugprone-exception-escape)
{
  const auto result_is_ok = math::wide_decimal::example009a_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
