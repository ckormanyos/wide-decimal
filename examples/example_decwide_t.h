///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021 - 2024.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef EXAMPLE_DECWIDE_T_2021_04_13_H // NOLINT(llvm-header-guard)
  #define EXAMPLE_DECWIDE_T_2021_04_13_H

  // This file contains function prototypes of the decwide_t examples.

  #include <math/wide_decimal/decwide_t_detail_namespace.h>

  #if !defined(WIDE_DECIMAL_NAMESPACE_BEGIN)
  #error WIDE_DECIMAL_NAMESPACE_BEGIN is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  #if !defined(WIDE_DECIMAL_NAMESPACE_END)
  #error WIDE_DECIMAL_NAMESPACE_END is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  WIDE_DECIMAL_NAMESPACE_BEGIN

  #if(__cplusplus >= 201703L)
  namespace math::wide_decimal {
  #else
  namespace math { namespace wide_decimal { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  auto example000_multiply_nines         () -> bool;
  auto example000a_multiply_pi_squared   () -> bool;
  auto example001_roots_sqrt             () -> bool;
  auto example001a_roots_seventh         () -> bool;
  auto example001b_roots_almost_integer  () -> bool;
  auto example001c_roots_sqrt_limb08     () -> bool;
  auto example001d_pow2_from_list        () -> bool;
  auto example001e_algebra_and_constexpr () -> bool;
  auto example002_pi                     () -> bool;
  auto example002a_pi_small_limb         () -> bool;
  auto example002b_pi_100k               () -> bool;
  auto example002c_pi_quintic            () -> bool;
  auto example002d_pi_limb08             () -> bool;
  auto example003_zeta                   () -> bool;
  auto example004_bessel_recur           () -> bool;
  auto example005_polylog_series         () -> bool;
  auto example006_logarithm              () -> bool;
  auto example007_catalan_series         () -> bool;
  auto example008_bernoulli_tgamma       () -> bool;
  auto example009_boost_math_standalone  () -> bool;
  auto example009a_boost_math_standalone () -> bool;
  auto example009b_boost_math_standalone () -> bool;
  auto example010_hypergeometric_2f1     () -> bool;
  auto example010a_hypergeometric_1f1    () -> bool;
  auto example011_trig_trapezoid_integral() -> bool;
  auto example012_rational_floor_ceil    () -> bool;
  auto example013_embeddable_sqrt        () -> bool;
  auto example013a_embeddable_agm        () -> bool;

  #if(__cplusplus >= 201703L)
  } // namespace math::wide_decimal
  #else
  } // namespace wide_decimal
  } // namespace math
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // EXAMPLE_DECWIDE_T_2021_04_13_H
