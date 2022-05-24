#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#if !defined(WIDE_DECIMAL_NAMESPACE)
#define WIDE_DECIMAL_NAMESPACE ckormanyos
#endif

#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/mpfr.hpp>

#include <test/parallel_for.h>

#include <math/wide_decimal/decwide_t.h>

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++ and -std=c++11
// g++ -finline-functions -march=native -mtune=native -O3 -Wall -Wextra -std=c++11 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_79_0 test/test_high_precision_exp.cpp -pthread -lpthread -lgmp -lmpfr -o test_high_precision_exp.exe
// ./test_high_precision_exp.exe

namespace test_high_precision_exp
{
  constexpr auto local_digits10 = static_cast<std::int32_t>(INT32_C(100001));

  using wide_decimal_type        = ckormanyos::math::wide_decimal::decwide_t<local_digits10>;
  using boost_float_backend_type = boost::multiprecision::mpfr_float_backend<local_digits10, boost::multiprecision::allocate_dynamic>;
  using boost_float_type         = boost::multiprecision::number<boost_float_backend_type, boost::multiprecision::et_off>;

  std::uniform_int_distribution<std::uint32_t> dst_numer(UINT32_C(900000000), UINT32_C(999999999));
  std::uniform_int_distribution<std::uint32_t> dst_denom(UINT32_C(100000000), UINT32_C(899999999));

  using eng_numer_type = std::mt19937;
  using eng_denom_type = std::minstd_rand0;

  eng_numer_type eng_numer;
  eng_denom_type eng_denom;

  auto do_test() -> bool
  {
    const auto my_pi_w = ckormanyos::math::wide_decimal::pi<local_digits10>();
    const auto my_pi_b = boost::math::constants::pi<boost_float_type>();

    const auto sd = std::clock();

    eng_numer.seed(static_cast<typename eng_numer_type::result_type>(sd));
    eng_denom.seed(static_cast<typename eng_denom_type::result_type>(sd));

    auto result_is_ok = true;

    using std::ilogb;

    const auto ilogb_tol = ilogb(std::numeric_limits<wide_decimal_type>::epsilon());

    std::atomic_flag do_calcs_exp_lock = ATOMIC_FLAG_INIT;

    my_concurrency::parallel_for
    (
      0U,
      48U,
      [&my_pi_w, &my_pi_b, &result_is_ok, &do_calcs_exp_lock, &ilogb_tol](unsigned count)
      {
        while(do_calcs_exp_lock.test_and_set()) { ; }
        const auto numer = dst_numer(eng_numer);
        const auto denom = dst_denom(eng_denom);
        do_calcs_exp_lock.clear();

        const auto yw = wide_decimal_type((wide_decimal_type(numer) * my_pi_w) / denom);
        const auto ew = exp(yw);

        const auto yb = boost_float_type((boost_float_type(numer) * my_pi_b) / denom);
        const auto eb = exp(yb);

        std::stringstream strm_b;

        strm_b << std::setprecision(std::numeric_limits<wide_decimal_type>::digits10 + 3)
               << eb;

        const wide_decimal_type ctrl(strm_b.str().c_str());

        const auto delta = fabs(1 - fabs(ew / ctrl));

        const bool result_exp_is_ok = ((delta == 0) || (ilogb(delta) <= ilogb_tol));

        while(do_calcs_exp_lock.test_and_set()) { ; }
        result_is_ok = (result_is_ok && result_exp_is_ok);

        std::cout << "count: " << count << ", delta: " << delta << ", result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
        do_calcs_exp_lock.clear();
      }
    );

    return result_is_ok;
  }
} // namespace test_high_precision_exp

int main()
{
  const auto start = std::chrono::high_resolution_clock::now();

  const bool result_is_ok = test_high_precision_exp::do_test();

  const auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  const auto duration =
    static_cast<float>
    (
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())
      / 1000.0F
    );

  std::cout << "High-precision calculations took: " << std::fixed << std::setprecision(1) << duration << "s" << std::endl;

  return (result_is_ok ? 0 : -1);
}
