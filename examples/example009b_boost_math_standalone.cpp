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

#if (BOOST_VERSION >= 107700)
#if !defined(BOOST_MATH_STANDALONE)
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

        floating_point_type term        = x;
  const floating_point_type x2          = x * x;
        floating_point_type sum         = x;
        bool              term_is_neg = true;
  const floating_point_type tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

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

  const floating_point_type x2          = x * x;
        floating_point_type term        = x2 / 2U;
        floating_point_type sum         = term;
        bool              term_is_neg = true;
  const floating_point_type tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

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

template<typename FloatingPointType>
auto cos(const FloatingPointType& x) -> FloatingPointType // NOLINT(misc-no-recursion)
{
  using floating_point_type = FloatingPointType;

  floating_point_type c;

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

    const auto k = static_cast<std::uint32_t>     (x / boost::math::constants::half_pi<floating_point_type>());
    const auto n = static_cast<std::uint_fast32_t>(k % 4U);

    floating_point_type r = x - (boost::math::constants::half_pi<floating_point_type>() * k);

    const auto is_neg = ((n == 1U) || (n == 2U));
    const auto is_sin = ((n == 1U) || (n == 3U));

    auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

    static const floating_point_type one_tenth = floating_point_type(1U) / 10U;

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
    c = floating_point_type(1U);
  }

  return c;
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
     // N[Gamma[5/2], 120]
     const T control_tgamma_2_and_half("1.32934038817913702047362561250585888709816209209179034616035584238968346344327413603121299255390849906217011771821192800");

     // N[Gamma[501/2], 120]
     const T control_tgamma_250_and_half("2.04361576378676793274281040858168765479902044174168184223161039072956296132709215283453484593278166826360563422419505374E491");

     const T tgamma_2_and_half   = ::boost::math::tgamma(T(T(5)   / 2));
     const T tgamma_250_and_half = ::boost::math::tgamma(T(T(501) / 2));

     using std::fabs;

     const T closeness_2_and_half   = fabs(1 - fabs(tgamma_2_and_half   / control_tgamma_2_and_half));
     const T closeness_250_and_half = fabs(1 - fabs(tgamma_250_and_half / control_tgamma_250_and_half));

     const T tol = std::numeric_limits<T>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000));

     const auto result_is_ok_2_and_half   = (closeness_2_and_half   < tol);
     const auto result_is_ok_250_and_half = (closeness_250_and_half < tol);

     const auto result_is_ok = (result_is_ok_2_and_half && result_is_ok_250_and_half);

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

  #if (BOOST_VERSION < 107900)
  using boost_wrapexcept_round_type  = ::boost::wrapexcept<::boost::math::rounding_error>;
  using boost_wrapexcept_domain_type = ::boost::wrapexcept<std::domain_error>;
  #endif

  auto result_is_ok = false;

  try
  {
  const auto result_010_is_ok = example009b_boost::test_tgamma<wide_decimal_010_type>();
  const auto result_035_is_ok = example009b_boost::test_tgamma<wide_decimal_035_type>();
  const auto result_105_is_ok = example009b_boost::test_tgamma<wide_decimal_105_type>();

  result_is_ok = (   result_010_is_ok
                  && result_035_is_ok
                  && result_105_is_ok);
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
