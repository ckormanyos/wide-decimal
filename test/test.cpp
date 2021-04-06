///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

// Build locally for test on MinGW or Strawberry via Git bash.
// cd C:\Users\User\Documents\Ks\PC_Software\NumericalPrograms\ExtendedNumberTypes\wide_decimal
// g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -Wno-cast-function-type -std=c++11 -I. -IC:/boost/boost_1_75_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp -o wide_decimal.exe

#include <chrono>
#include <iomanip>
#include <iostream>

#include <test/test_decwide_t_algebra.h>
#include <test/test_decwide_t_examples.h>

namespace
{
  using clock_type = std::chrono::high_resolution_clock;

  auto wide_decimal_time_start = clock_type::now();
}

int main()
{
  using time_point_type = std::chrono::high_resolution_clock::time_point;

  time_point_type start;
  time_point_type stop;

  start = clock_type::now(); const bool result_test_decwide_t_examples_part1_is_ok = (test_decwide_t_examples_part1__() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_examples_part1_is_ok: " << std::boolalpha << result_test_decwide_t_examples_part1_is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_examples_part2_is_ok = (test_decwide_t_examples_part2__() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_examples_part2_is_ok: " << std::boolalpha << result_test_decwide_t_examples_part2_is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_add____is_ok = (test_decwide_t_algebra_add_____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_add____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_add____is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_sub____is_ok = (test_decwide_t_algebra_sub_____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_sub____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_sub____is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_mul____is_ok = (test_decwide_t_algebra_mul_____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_mul____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_mul____is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_div____is_ok = (test_decwide_t_algebra_div_____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_div____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_div____is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_sqrt___is_ok = (test_decwide_t_algebra_sqrt____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_sqrt___is_ok: " << std::boolalpha << result_test_decwide_t_algebra_sqrt___is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;
  start = clock_type::now(); const bool result_test_decwide_t_algebra_log____is_ok = (test_decwide_t_algebra_log_____() == true); stop = clock_type::now(); std::cout << "result_test_decwide_t_algebra_log____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_log____is_ok << ", time: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.0F << "s" << std::endl;

  const bool result_is_ok = (   result_test_decwide_t_examples_part1_is_ok
                             && result_test_decwide_t_examples_part2_is_ok
                             && result_test_decwide_t_algebra_add____is_ok
                             && result_test_decwide_t_algebra_sub____is_ok
                             && result_test_decwide_t_algebra_mul____is_ok
                             && result_test_decwide_t_algebra_div____is_ok
                             && result_test_decwide_t_algebra_sqrt___is_ok
                             && result_test_decwide_t_algebra_log____is_ok);

  const float sec =
    ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - wide_decimal_time_start).count()) / 1000.0F;

  std::cout << "result_is_ok:                             : "
            << std::boolalpha
            << result_is_ok
            << ", time: "
            << sec
            << "s"
            << std::endl;

  return (result_is_ok ? 0 : -1);
}
