///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef TEST_DECWIDE_T_EXAMPLES_2020_11_05_H
  #define TEST_DECWIDE_T_EXAMPLES_2020_11_05_H

  #include <math/wide_decimal/decwide_t_detail_namespace.h>

  #if !defined(WIDE_DECIMAL_NAMESPACE_BEGIN)
  #error WIDE_DECIMAL_NAMESPACE_BEGIN is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  #if !defined(WIDE_DECIMAL_NAMESPACE_END)
  #error WIDE_DECIMAL_NAMESPACE_END is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  WIDE_DECIMAL_NAMESPACE_BEGIN

  auto test_decwide_t_examples_part1__() -> bool; // NOLINT(readability-identifier-naming,bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
  auto test_decwide_t_examples_part2__() -> bool; // NOLINT(readability-identifier-naming,bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

  WIDE_DECIMAL_NAMESPACE_END

#endif // TEST_DECWIDE_T_EXAMPLES_2020_11_05_H
