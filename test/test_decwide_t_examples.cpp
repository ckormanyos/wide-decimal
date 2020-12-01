#include <math/wide_decimal/decwide_t.h>
#include <test/test_decwide_t_examples.h>

bool test_decwide_t_examples____()
{
  bool result_is_ok = true;

  result_is_ok &= math::wide_decimal::example001_sqrt          ();
  result_is_ok &= math::wide_decimal::example002_pi            ();
  result_is_ok &= math::wide_decimal::example002a_pi_small_limb();
  result_is_ok &= math::wide_decimal::example002b_pi_100k      ();
  result_is_ok &= math::wide_decimal::example003_zeta          ();
  result_is_ok &= math::wide_decimal::example004_bessel_recur  ();
  result_is_ok &= math::wide_decimal::example005_polylog_series();
  result_is_ok &= math::wide_decimal::example006_logarithm     ();
  result_is_ok &= math::wide_decimal::example007_catalan_series();

  return result_is_ok;
}
