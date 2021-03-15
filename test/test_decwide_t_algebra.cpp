///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <math/wide_decimal/decwide_t.h>
#include <test/independent_algebra_test.h>
#include <test/test_decwide_t_algebra.h>

#if defined(__clang__)
  #if defined __has_feature && __has_feature(thread_sanitizer)
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#elif defined(__GNUC__)
  #if defined(__SANITIZE_THREAD__)
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#endif

namespace
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(10001);

  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  constexpr std::uint32_t independent_algebra_test_count         = UINT32_C(512);
  constexpr std::uint32_t independent_algebra_test_count_for_log = UINT32_C(32);
  #else
  constexpr std::uint32_t independent_algebra_test_count         = UINT32_C(128);
  constexpr std::uint32_t independent_algebra_test_count_for_log = UINT32_C(8);
  #endif

  constexpr std::uint32_t independent_algebra_test_round         = UINT32_C(4);
  constexpr std::uint32_t independent_algebra_test_round_for_log = UINT32_C(4);
}

bool test_decwide_t_algebra_add_____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_add_<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count,
                                                             independent_algebra_test_round,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}

bool test_decwide_t_algebra_sub_____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_sub_<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count,
                                                             independent_algebra_test_round,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}

bool test_decwide_t_algebra_mul_____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_mul_<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count,
                                                             independent_algebra_test_round,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}

bool test_decwide_t_algebra_div_____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_div_<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count,
                                                             independent_algebra_test_round,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}

bool test_decwide_t_algebra_sqrt____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_sqrt<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count,
                                                             independent_algebra_test_round,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}

bool test_decwide_t_algebra_log_____()
{
  using independent_algebra_test_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10,
                                                                  local_limb_type,
                                                                  std::allocator<void>,
                                                                  double,
                                                                  std::int32_t>;

  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_log_<wide_decimal_digits10,
                                                             local_limb_type,
                                                             std::allocator<void>,
                                                             double,
                                                             std::int32_t,
                                                             independent_algebra_test_count_for_log,
                                                             independent_algebra_test_round_for_log,
                                                             independent_algebra_test_boost_cpp_type>();

  return result_is_ok;
}
