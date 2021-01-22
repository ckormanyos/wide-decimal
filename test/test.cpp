///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2020.                 //
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
// g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -pedantic -Wno-cast-function-type -std=c++2a -I. -IC:/boost/boost_1_75_0 test/test.cpp -pthread -lpthread test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp -o wide_decimal.exe

// Build locally for test on a GCC-10
// set GCCTENBIN=D:\winlibs-x86_64-posix-seh-gcc-10.2.0-llvm-10.0.1-mingw-w64-7.0.0-r4\mingw64\bin
// %GCCTENBIN%\g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -pedantic -Wno-cast-function-type -std=c++20 -I. -IC:/boost/boost_1_75_0 test/test.cpp -pthread -lpthread test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example001b_roots_almost_integer.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp examples/example010_hypergeometric_2f1.cpp examples/example010a_hypergeometric_1f1.cpp examples/example011_trig_trapezoid_integral.cpp -o wide_decimal.exe
// Then:
// d:
// cd d:\winlibs-x86_64-posix-seh-gcc-10.2.0-llvm-10.0.1-mingw-w64-7.0.0-r4\mingw64\bin>

#include <chrono>
#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE test_decwide_t
#include <boost/test/included/unit_test.hpp>

#include <test/test_decwide_t_algebra.h>
#include <test/test_decwide_t_examples.h>

namespace
{
  using clock_type = std::chrono::high_resolution_clock;

  auto wide_decimal_time_start = clock_type::now();
}

BOOST_AUTO_TEST_CASE(test_decwide_t_examples_part1___tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_examples_part1__() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_examples_part1___: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_examples_part2___tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_examples_part2__() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_examples_part2___: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_add______tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_add_____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_add______: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_sub______tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_sub_____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_sub______: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_mul______tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_mul_____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_mul______: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_div______tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_div_____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_div______: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_sqrt_____tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_sqrt____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_sqrt_____: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl; }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_log______tag) { auto start = clock_type::now(); BOOST_CHECK(test_decwide_t_algebra_log_____() == true); auto stop = clock_type::now(); std::cout << "time test_decwide_t_algebra_log______: " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())                   / 1000.0F << "s" << std::endl;
                                                                                                                                                                                   std::cout << "time total:                            " << ((float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - wide_decimal_time_start).count()) / 1000.0F << "s" << std::endl; }
