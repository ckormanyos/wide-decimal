///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <boost/version.hpp>

#if !defined(BOOST_VERSION)
#error BOOST_VERSION is not defined. Ensure that <boost/version.hpp> is properly included.
#endif

#if !defined(BOOST_MATH_STANDALONE)
#define BOOST_NO_EXCEPTIONS
#endif

#if (BOOST_VERSION >= 107700)
#if !defined(BOOST_MATH_STANDALONE)
#define BOOST_MATH_STANDALONE
#endif
#endif

#if ((BOOST_VERSION >= 107900) && !defined(BOOST_MP_STANDALONE))
#define BOOST_MP_STANDALONE
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#if (BOOST_VERSION < 108000)
#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif
#endif

#include <math/wide_decimal/decwide_t.h>
#include <test/independent_algebra_test_decwide_t.h>
#include <test/test_decwide_t_algebra.h>

#if defined(__clang__)
  #if defined __has_feature && (__has_feature(thread_sanitizer) || __has_feature(address_sanitizer))
  #define DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH
  #endif
#elif defined(__GNUC__)
  #if defined(__SANITIZE_THREAD__) || defined(__SANITIZE_ADDRESS__)
  #define DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH
  #endif
#endif

namespace test_decwide_t
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(10001);

  #if !defined(DECWIDE_T_TEST_OPTION_REDUCE_TEST_DEPTH)
  constexpr std::uint32_t independent_algebra_test_decwide_t_count         = UINT32_C(128);
  constexpr std::uint32_t independent_algebra_test_decwide_t_count_for_log = UINT32_C(32);
  constexpr std::uint32_t independent_algebra_test_decwide_t_round         = UINT32_C(4);
  #else
  constexpr std::uint32_t independent_algebra_test_decwide_t_count         = UINT32_C(32);
  constexpr std::uint32_t independent_algebra_test_decwide_t_count_for_log = UINT32_C(8);
  constexpr std::uint32_t independent_algebra_test_decwide_t_round         = UINT32_C(1);
  #endif
} // namespace test_decwide_t

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_add_____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_add_____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_add_<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_sub_____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_sub_____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_sub_<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_mul_____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_mul_____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_mul_<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_div_____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_div_____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  #if defined(WIDE_DECIMAL_NAMESPACE)
  using namespace WIDE_DECIMAL_NAMESPACE;
  #else
  #endif

  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_div_<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_sqrt____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_sqrt____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_sqrt<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_log_____() -> bool // NOLINT(readability-identifier-naming)
#else
auto test_decwide_t_algebra_log_____() -> bool // NOLINT(readability-identifier-naming)
#endif
{
  using independent_algebra_test_decwide_t_boost_cpp_type =
    test::independent_algebra::independent_algebra_test_decwide_t_boost_cpp<test_decwide_t::wide_decimal_digits10,
                                                                            test_decwide_t::local_limb_type,
                                                                            std::allocator<void>,
                                                                            double,
                                                                            std::int32_t,
                                                                            double>;

  const auto result_is_ok =
    test::independent_algebra::independent_algebra_test_decwide_t_log_<test_decwide_t::wide_decimal_digits10,
                                                                       test_decwide_t::local_limb_type,
                                                                       std::allocator<void>,
                                                                       double,
                                                                       std::int32_t,
                                                                       double,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_count_for_log,
                                                                       test_decwide_t::independent_algebra_test_decwide_t_round,
                                                                       independent_algebra_test_decwide_t_boost_cpp_type>();

  return result_is_ok;
}

#if (BOOST_VERSION < 108000)
#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic pop
#endif
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
