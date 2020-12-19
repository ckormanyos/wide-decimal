///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2020.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

// cd C:\Users\User\Documents\Ks\PC_Software\NumericalPrograms\ExtendedNumberTypes\wide_decimal
// g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -pedantic -Wno-cast-function-type -std=c++11 -I. -IC:/boost/boost_1_75_0 test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp -o wide_decimal.exe

// C:\Strawberry\c\bin\g++ -finline-functions -finline-limit=32 -march=native -mtune=native -O3 -Wall -Wextra -Wno-cast-function-type -std=c++11 -I. -ID:/boost/boost_1_75_0 test/test.cpp test/test_decwide_t_algebra.cpp test/test_decwide_t_examples.cpp examples/example001_roots_sqrt.cpp examples/example001a_roots_seventh.cpp examples/example002_pi.cpp examples/example002a_pi_small_limb.cpp examples/example002b_pi_100k.cpp examples/example002c_pi_quintic.cpp examples/example003_zeta.cpp examples/example004_bessel_recur.cpp examples/example005_polylog_series.cpp examples/example006_logarithm.cpp examples/example007_catalan_series.cpp examples/example008_bernoulli_tgamma.cpp examples/example009_boost_math_standalone.cpp -o wide_decimal.exe

#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE test_decwide_t
#include <boost/test/included/unit_test.hpp>

#include <test/test_decwide_t_algebra.h>
#include <test/test_decwide_t_examples.h>

namespace
{
  void set_log_level()
  {
    const auto level = boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_test_units);

    (void) level;
  }
}

BOOST_AUTO_TEST_CASE(test_decwide_t_examples_part1___tag) { set_log_level(); BOOST_CHECK(test_decwide_t_examples_part1__() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_examples_part2___tag) { set_log_level(); BOOST_CHECK(test_decwide_t_examples_part2__() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_add______tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_add_____() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_sub______tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_sub_____() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_mul______tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_mul_____() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_div______tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_div_____() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_sqrt_____tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_sqrt____() == true); }
BOOST_AUTO_TEST_CASE(test_decwide_t_algebra_log______tag) { set_log_level(); BOOST_CHECK(test_decwide_t_algebra_log_____() == true); }
