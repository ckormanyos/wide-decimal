///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

// Build locally for test on MinGW or Strawberry via Git bash.
// cd C:\Users\User\Documents\Ks\PC_Software\NumericalPrograms\ExtendedNumberTypes\wide_decimal
// cd C:/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Werror -Wall -Wextra -Wconversion -Wsign-conversion -Wno-cast-function-type -std=c++11 -I. -IC:/boost/boost_1_78_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt examples/example013a_embeddable_agm -o wide_decimal.exe

// On Windows Subsystem for LINUX
// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++-10 and -std=c++20
// g++-10 -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -Wconversion -Wsign-conversion -Wno-cast-function-type -std=c++20 -I. -I/mnt/c/boost/boost_1_78_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt examples/example013a_embeddable_agm -o wide_decimal.exe

// When using g++-10 and -std=c++20 including modular-boost's math/multiprecision submodules
// g++-10 -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Werror -Wall -Wextra -Wconversion -Wsign-conversion -Wno-cast-function-type -std=c++20 -I. -I/mnt/c/boost/modular_boost/boost/libs/math/include -I/mnt/c/boost/modular_boost/boost/libs/multiprecision/include -I/mnt/c/boost/boost_1_78_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt examples/example013a_embeddable_agm -o wide_decimal.exe

// When using clang and -std=c++2a
// clang++ -finline-functions -march=native -mtune=native -O3 -Werror -Wall -Wextra -Wconversion -Wsign-conversion -std=c++2a -I. -I/mnt/c/boost/boost_1_78_0 -pthread -lpthread test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example000a_multiply_pi_squared.cpp examples/example000_multiply_nines.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example001c_roots_sqrt_limb08.cpp examples/example001d_pow2_from_list.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example002d_pi_limb08.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example009a_boost_math_standalone.cpp examples/example009b_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp examples/example012_rational_floor_ceil.cpp examples/example013_embeddable_sqrt examples/example013a_embeddable_agm -o wide_decimal.exe

#include <chrono>
#include <iomanip>
#include <iostream>

#include <test/test_decwide_t_algebra.h>
#include <test/test_decwide_t_examples.h>

auto main() -> int
{
  using local_clock_type = std::chrono::high_resolution_clock;
  using time_point_type  = typename local_clock_type::time_point;

  auto start_all = local_clock_type::now();

  time_point_type stop;
  time_point_type start;

  start = local_clock_type::now(); const bool result_test_decwide_t_examples_part1_is_ok = (test_decwide_t_examples_part1__() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_examples_part1_is_ok: " << std::boolalpha << result_test_decwide_t_examples_part1_is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_examples_part2_is_ok = (test_decwide_t_examples_part2__() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_examples_part2_is_ok: " << std::boolalpha << result_test_decwide_t_examples_part2_is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_add____is_ok = (test_decwide_t_algebra_add_____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_add____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_add____is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_sub____is_ok = (test_decwide_t_algebra_sub_____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_sub____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_sub____is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_mul____is_ok = (test_decwide_t_algebra_mul_____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_mul____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_mul____is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_div____is_ok = (test_decwide_t_algebra_div_____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_div____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_div____is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_sqrt___is_ok = (test_decwide_t_algebra_sqrt____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_sqrt___is_ok: " << std::boolalpha << result_test_decwide_t_algebra_sqrt___is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  start = local_clock_type::now(); const bool result_test_decwide_t_algebra_log____is_ok = (test_decwide_t_algebra_log_____() ); stop = local_clock_type::now(); std::cout << "result_test_decwide_t_algebra_log____is_ok: " << std::boolalpha << result_test_decwide_t_algebra_log____is_ok << ", time: " << (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())) / 1000.0F << "s" << std::endl; // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

  const float t_sec =
    float(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_all).count()) / 1000.0F;

  const bool result_is_ok =
    bool
    (
         (result_test_decwide_t_examples_part1_is_ok )
      && (result_test_decwide_t_examples_part2_is_ok )
      && (result_test_decwide_t_algebra_add____is_ok )
      && (result_test_decwide_t_algebra_sub____is_ok )
      && (result_test_decwide_t_algebra_mul____is_ok )
      && (result_test_decwide_t_algebra_div____is_ok )
      && (result_test_decwide_t_algebra_sqrt___is_ok )
      && (result_test_decwide_t_algebra_log____is_ok )
    );

  // Print the total results.
  std::cout << "result_is_ok:                             : "
            << std::boolalpha
            << result_is_ok
            << ", time: "
            << t_sec
            << "s"
            << std::endl
            ;

  return (result_is_ok ? 0 : -1);
}
