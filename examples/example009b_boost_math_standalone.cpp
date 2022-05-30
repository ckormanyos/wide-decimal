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

  for(auto k = static_cast<std::uint32_t>(UINT32_C(4));
           k < static_cast<std::uint32_t>(UINT32_C(10000));
           k = static_cast<std::uint32_t>(k + static_cast<std::uint32_t>(UINT8_C(2))))
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

template<typename FloatingPointType>
auto sin(const FloatingPointType& x) -> FloatingPointType // NOLINT(misc-no-recursion)
{
  using floating_point_type = FloatingPointType;

  floating_point_type s;

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

    const auto k = static_cast<std::uint32_t>     (x / boost::math::constants::half_pi<floating_point_type>());
    const auto n = static_cast<std::uint_fast32_t>(k % 4U);

    floating_point_type r = x - (boost::math::constants::half_pi<floating_point_type>() * k);

    const auto is_neg =  (n > 1U);
    const auto is_cos = ((n == 1U) || (n == 3U));

    auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

    static const floating_point_type one_tenth = floating_point_type(1U) / 10U;

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

#if(__cplusplus >= 201703L)
} // namespace math::wide_decimal
#else
} // namespace wide_decimal
} // namespace math
#endif

WIDE_DECIMAL_NAMESPACE_END

namespace example009b_boost
{
  template<class T>
  auto test_tgamma() -> bool
  {
     // N[Gamma[5/2], 320]
     const T control_tgamma_002_and_half       ("1.3293403881791370204736256125058588870981620920917903461603558423896834634432741360312129925539084990621701177182119279996771146492933169518938202822020903013465282739898288421374438797717131196716990715344509721001309792615136097903875251426389255139390852308711844802354413316444296623040644993756797988057103001081064");

     // N[Gamma[501/2], 320]
     const T control_tgamma_250_and_half       ("2.0436157637867679327428104085816876547990204417416818422316103907295629613270921528345348459327816682636056342241950537446541535116288177757620180708816682736540856939947792019709829601955492735707432530618999787498280191875334522078610897426020922257309921502998837542955056170017022660969773746509339458003165654615581E491");

     // N[Gamma[-301/3], 320]
     const T control_tgamma_100_and_third_minus("-8.3557703333576718256158073988179208699145415756386069459318008596922002407591322112916720524641859723334916854149321144950007130289318393900505551471648382390592504702784319225368018374357793516541262525400066540804092567079990711749716708828216656334660630599555105263837919721470429933608136568647200819078822477473661E-159");

     // N[Gamma[-1/3], 320]
     const T control_tgamma_001_third_minus    ("-4.0623538182792012508358640844635413565579817981703810951820674038913488962052276250275024438589187333193044708300991765624267846577586354798215734443429685003024677468627525170477770995701556357325740345053104608510715618361049532637992095124677920741526841038682881484510994970415391794339637926102520597719997972596582");

     // N[Gamma[-2/3], 320]
     const T control_tgamma_002_third_minus    ("-4.0184078020616214504835394114620164661930340669359516514256424913856264152516157293114743335617831841287385738001083950035786268542567083899665700515627046522509347345750048275138254833707354127263331749290931765921691619586487426024184787123946997890505913760308709532709148615480237591230740512828607815485081984326040");

     // N[Gamma[-4/3], 320]
     const T control_tgamma_004_third_minus    ("3.0467653637094009381268980633476560174184863486277858213865505529185116721539207187706268328941890499894783531225743824218200884933189766098661800832572263752268508101470643877858328246776167267994305258789828456383036713770787149478494071343508440556145130779012161113383246227811543845754728444576890448289998479447436");

     // N[Gamma[-5/3], 320]
     const T control_tgamma_005_third_minus    ("2.4110446812369728702901236468772098797158204401615709908553854948313758491509694375868846001370699104772431442800650370021471761125540250339799420309376227913505608407450028965082952900224412476357999049574559059553014971751892455614510872274368198734303548256185225719625489169288142554738444307697164689291049190595624");

     const T tgamma_002_and_half        = ::boost::math::tgamma(T(T(   5U) / 2U));
     const T tgamma_250_and_half        = ::boost::math::tgamma(T(T( 501U) / 2U));
     const T tgamma_100_and_third_minus = ::boost::math::tgamma(T(T(-301)  / 3));
     const T tgamma_001_third_minus     = ::boost::math::tgamma(T(T(  -1)  / 3));
     const T tgamma_002_third_minus     = ::boost::math::tgamma(T(T(  -2)  / 3));
     const T tgamma_004_third_minus     = ::boost::math::tgamma(T(T(  -4)  / 3));
     const T tgamma_005_third_minus     = ::boost::math::tgamma(T(T(  -5)  / 3));

     using std::fabs;

     const T closeness_002_and_half        = fabs(1 - fabs(tgamma_002_and_half        / control_tgamma_002_and_half));
     const T closeness_250_and_half        = fabs(1 - fabs(tgamma_250_and_half        / control_tgamma_250_and_half));
     const T closeness_100_and_third_minus = fabs(1 - fabs(tgamma_100_and_third_minus / control_tgamma_100_and_third_minus));
     const T closeness_001_third_minus     = fabs(1 - fabs(tgamma_001_third_minus     / control_tgamma_001_third_minus));
     const T closeness_002_third_minus     = fabs(1 - fabs(tgamma_002_third_minus     / control_tgamma_002_third_minus));
     const T closeness_004_third_minus     = fabs(1 - fabs(tgamma_004_third_minus     / control_tgamma_004_third_minus));
     const T closeness_005_third_minus     = fabs(1 - fabs(tgamma_005_third_minus     / control_tgamma_005_third_minus));

     const T tol = std::numeric_limits<T>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000));

     const auto result_is_ok_002_and_half        = (closeness_002_and_half        < tol);
     const auto result_is_ok_250_and_half        = (closeness_250_and_half        < tol);
     const auto result_is_ok_100_and_third_minus = (closeness_100_and_third_minus < tol);
     const auto result_is_ok_001_third_minus =     (closeness_001_third_minus     < tol);
     const auto result_is_ok_002_third_minus =     (closeness_002_third_minus     < tol);
     const auto result_is_ok_004_third_minus =     (closeness_004_third_minus     < tol);
     const auto result_is_ok_005_third_minus =     (closeness_005_third_minus     < tol);

     const auto result_is_ok =
     (
          result_is_ok_002_and_half
       && result_is_ok_250_and_half
       && result_is_ok_100_and_third_minus
       && result_is_ok_001_third_minus
       && result_is_ok_002_third_minus
       && result_is_ok_004_third_minus
       && result_is_ok_005_third_minus
     );

     return result_is_ok;
  }
} // namespace example009b_boost

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example009b_boost_math_standalone() -> bool
#else
auto math::wide_decimal::example009b_boost_math_standalone() -> bool
#endif
{
  using wide_decimal_010_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C( 10)), std::uint32_t, void>;
  using wide_decimal_035_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C( 35)), std::uint32_t, void>;
  using wide_decimal_105_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(105)), std::uint32_t, void>;
  using wide_decimal_305_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(305)), std::uint32_t, void>;

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
  const auto result_010_is_ok = example009b_boost::test_tgamma<wide_decimal_010_type>();
  const auto result_035_is_ok = example009b_boost::test_tgamma<wide_decimal_035_type>();
  const auto result_105_is_ok = example009b_boost::test_tgamma<wide_decimal_105_type>();
  const auto result_305_is_ok = example009b_boost::test_tgamma<wide_decimal_305_type>();

  result_is_ok = (   result_010_is_ok
                  && result_035_is_ok
                  && result_105_is_ok
                  && result_305_is_ok);
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
  #endif
  #endif

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE009B_BOOST_MATH_STANDALONE)

#include <iomanip>
#include <iostream>

// TBD: Handle exception catching in example009b_boost_math_standalone at a later time.
auto main() -> int // NOLINT(bugprone-exception-escape)
{
  const auto result_is_ok = math::wide_decimal::example009b_boost_math_standalone();

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
