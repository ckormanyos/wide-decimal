///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif

#include <boost/math/bindings/decwide_t.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <math/wide_decimal/decwide_t_examples.h>

WIDE_DECIMAL_NAMESPACE_BEGIN

namespace math { namespace wide_decimal {

namespace detail {

template<typename FloatingPointType>
auto sin_series(const FloatingPointType& x) -> FloatingPointType
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
auto cos_series(const FloatingPointType& x) -> FloatingPointType
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

    const bool is_neg =  (n > 1U);
    const bool is_cos = ((n == 1U) || (n == 3U));

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

    const bool is_neg = ((n == 1U) || (n == 2U));
    const bool is_sin = ((n == 1U) || (n == 3U));

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

} // namespace wide_decimal
} // namespace math

WIDE_DECIMAL_NAMESPACE_END

namespace example009b_boost
{
  template<class T>
  auto test_tgamma() -> bool
  {
     // N[Gamma[5/2], 120]
     const T control_tgamma_2_and_half("1.32934038817913702047362561250585888709816209209179034616035584238968346344327413603121299255390849906217011771821192800");

     // N[Gamma[5000/2], 120]
     const T control_tgamma_2_fifty   ("1.29314250436364309292832582080974738839793748706951226669917697084512949902204448379552716614841127978037140294127577317E490");

     const T tgamma_2_and_half = boost::math::tgamma(T(T(5)   / 2));
     const T tgamma_2_fifty    = boost::math::tgamma(T(T(500) / 2));

     const T closeness_2_and_half = fabs(1 - fabs(tgamma_2_and_half / control_tgamma_2_and_half));
     const T closeness_2_fifty    = fabs(1 - fabs(tgamma_2_fifty    / control_tgamma_2_fifty));

     const bool result_is_ok_2_and_half = (closeness_2_and_half < std::numeric_limits<T>::epsilon() * T(1.0E5L));
     const bool result_is_ok_2_fifty    = (closeness_2_fifty    < std::numeric_limits<T>::epsilon() * T(1.0E5L));

     const bool result_is_ok = (result_is_ok_2_and_half && result_is_ok_2_fifty);

     return result_is_ok;
  }
} // namespace example009b_boost

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example009b_boost_math_standalone() -> bool
#else
auto math::wide_decimal::example009b_boost_math_standalone() -> bool
#endif
{
  using wide_decimal_010_type = math::wide_decimal::decwide_t< 10U, std::uint32_t, void>;
  using wide_decimal_035_type = math::wide_decimal::decwide_t< 35U, std::uint32_t, void>;
  using wide_decimal_105_type = math::wide_decimal::decwide_t<105U, std::uint32_t, void>;

  const bool result_010_is_ok = example009b_boost::test_tgamma<wide_decimal_010_type>();
  const bool result_035_is_ok = example009b_boost::test_tgamma<wide_decimal_035_type>();
  const bool result_105_is_ok = example009b_boost::test_tgamma<wide_decimal_105_type>();

  const bool result_is_ok = (   result_010_is_ok
                             && result_035_is_ok
                             && result_105_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example009b_boost_math_standalone();

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
#endif
