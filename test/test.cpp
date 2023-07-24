///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

// On Windows Subsystem for LINUX
// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++-12 and -std=c++20 (also with -DWIDE_DECIMAL_NAMESPACE=ckormanyos)
// g++-12 -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wunused-parameter -Wuninitialized -Wunreachable-code -Winit-self -Wzero-as-null-pointer-constant -std=c++20 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_81_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_algebra_edge.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt.cpp examples/example013a_embeddable_agm.cpp -o wide_decimal.exe

// cd .tidy/make
// make prepare -f make_tidy_01_generic.gmk MY_BOOST_ROOT=/mnt/c/boost/boost_1_81_0
// make tidy -f make_tidy_01_generic.gmk --jobs=8 MY_BOOST_ROOT=/mnt/c/boost/boost_1_81_0

// cd .gcov/make
// make clean -f make_gcov_01_generic.gmk MY_CC=g++ MY_ALL_COV=0 MY_BOOST_ROOT=/mnt/c/boost/boost_1_81_0
// make prepare -f make_gcov_01_generic.gmk MY_CC=g++ MY_ALL_COV=0 MY_BOOST_ROOT=/mnt/c/boost/boost_1_81_0
// make gcov -f make_gcov_01_generic.gmk --jobs=8 MY_CC=g++ MY_ALL_COV=0 MY_BOOST_ROOT=/mnt/c/boost/boost_1_81_0

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// PATH=/home/chris/local/coverity/cov-analysis-linux64-2021.12.1/bin:$PATH
// cov-build --dir cov-int g++ -fno-rtti -fno-exceptions -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wunused-parameter -Wuninitialized -Wunreachable-code -Winit-self -Wzero-as-null-pointer-constant -std=c++14 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/modular_boost/boost/libs/config/include -I/mnt/c/boost/modular_boost/boost/libs/multiprecision/include -I/mnt/c/boost/modular_boost/boost/libs/math/include -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_algebra_edge.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt.cpp examples/example013a_embeddable_agm.cpp -o wide_decimal.exe
// tar caf wide-decimal.bz2 cov-int

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>

#include <test/test_decwide_t_algebra.h>
#include <test/test_decwide_t_examples.h>

namespace local
{
  auto run() -> bool;

  auto pfn_runner(const std::function<bool(void)>& pfn_test, const char* p_msg) -> bool
  {
    using local_clock_type = std::chrono::high_resolution_clock;

    const auto start = local_clock_type::now();

    const auto result_test_is_ok = pfn_test();

    const auto stop = local_clock_type::now();

    const auto elapsed =
      static_cast<float>
      (
          static_cast<float>
          (
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
          )
        / static_cast<float>(UINT16_C(1000))
      );

    const auto flg = std::cout.flags();

    std::cout << p_msg << std::boolalpha << result_test_is_ok << ", time: " << elapsed << "s" << std::endl;

    std::cout.flags(flg);

    return result_test_is_ok;
  }
} // namespace local

auto local::run() -> bool
{
  #if defined(WIDE_DECIMAL_NAMESPACE)
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_examples_part1__;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_examples_part2__;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_edge____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_add_____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_sub_____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_mul_____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_div_____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_sqrt____;
  using WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_log_____;
  #endif

  const auto result_test_decwide_t_examples_part1_is_ok = local::pfn_runner(std::function(test_decwide_t_examples_part1__), "result_test_decwide_t_examples_part1_is_ok: ");
  const auto result_test_decwide_t_examples_part2_is_ok = local::pfn_runner(std::function(test_decwide_t_examples_part2__), "result_test_decwide_t_examples_part2_is_ok: ");
  const auto result_test_decwide_t_algebra_edge___is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_edge____), "result_test_decwide_t_algebra_edge___is_ok: ");
  const auto result_test_decwide_t_algebra_add____is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_add_____), "result_test_decwide_t_algebra_add____is_ok: ");
  const auto result_test_decwide_t_algebra_sub____is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_sub_____), "result_test_decwide_t_algebra_sub____is_ok: ");
  const auto result_test_decwide_t_algebra_mul____is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_mul_____), "result_test_decwide_t_algebra_mul____is_ok: ");
  const auto result_test_decwide_t_algebra_div____is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_div_____), "result_test_decwide_t_algebra_div____is_ok: ");
  const auto result_test_decwide_t_algebra_sqrt___is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_sqrt____), "result_test_decwide_t_algebra_sqrt___is_ok: ");
  const auto result_test_decwide_t_algebra_log____is_ok = local::pfn_runner(std::function(test_decwide_t_algebra_log_____), "result_test_decwide_t_algebra_log____is_ok: ");

  const auto result_is_ok =
  (
       result_test_decwide_t_examples_part1_is_ok
    && result_test_decwide_t_examples_part2_is_ok
    && result_test_decwide_t_algebra_edge___is_ok
    && result_test_decwide_t_algebra_add____is_ok
    && result_test_decwide_t_algebra_sub____is_ok
    && result_test_decwide_t_algebra_mul____is_ok
    && result_test_decwide_t_algebra_div____is_ok
    && result_test_decwide_t_algebra_sqrt___is_ok
    && result_test_decwide_t_algebra_log____is_ok
  );

  return result_is_ok;
}

auto main() -> int
{
  using local_clock_type = std::chrono::high_resolution_clock;

  const auto start = local_clock_type::now();

  const auto result_is_ok = local::run();

  const auto stop = local_clock_type::now();

  const auto flg = std::cout.flags();

  {
    constexpr auto one_thousand_milliseconds = static_cast<float>(UINT16_C(1000));

    const auto test_duration_sec =
      static_cast<float>
      (
        static_cast<float>
        (
          std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
        )
        / one_thousand_milliseconds
      );

    std::cout << "result_is_ok:                             : "
              << std::boolalpha
              << result_is_ok
              << ", time: "
              << test_duration_sec
              << "s"
              << std::endl
              ;
  }

  const auto result_of_main = (result_is_ok ? 0 : -1);

  std::cout << "result_of_main: " << result_of_main << std::endl;

  std::cout.flags(flg);

  return result_of_main;
}
