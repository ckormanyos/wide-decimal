///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <math/wide_decimal/decwide_t.h>
#include <test/test_decwide_t_examples.h>

bool test_decwide_t_examples____()
{
  bool result_is_ok = true;

  result_is_ok &= math::wide_decimal::example001_roots_sqrt           ();
  result_is_ok &= math::wide_decimal::example001a_roots_seventh       ();
  result_is_ok &= math::wide_decimal::example002_pi                   ();
  result_is_ok &= math::wide_decimal::example002a_pi_small_limb       ();
  result_is_ok &= math::wide_decimal::example002b_pi_100k             ();
  result_is_ok &= math::wide_decimal::example002c_pi_quintic          ();
  result_is_ok &= math::wide_decimal::example003_zeta                 ();
  result_is_ok &= math::wide_decimal::example004_bessel_recur         ();
  result_is_ok &= math::wide_decimal::example005_polylog_series       ();
  result_is_ok &= math::wide_decimal::example006_logarithm            ();
  result_is_ok &= math::wide_decimal::example007_catalan_series       ();
  result_is_ok &= math::wide_decimal::example008_bernoulli_tgamma     ();
  result_is_ok &= math::wide_decimal::example009_boost_math_standalone();

  return result_is_ok;
}
