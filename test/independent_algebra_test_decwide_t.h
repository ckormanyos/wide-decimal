///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H
  #define INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H

  #include <atomic>
  #include <iostream>
  #include <random>

  #include <math/wide_decimal/decwide_t.h>

  #include <test/independent_algebra_test_decwide_t_boost_cpp.h>
  #include <test/independent_algebra_test_decwide_t_wide_decimal.h>
  #include <test/parallel_for.h>

  #if !defined(WIDE_DECIMAL_NAMESPACE_BEGIN)
  #error WIDE_DECIMAL_NAMESPACE_BEGIN is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  #if !defined(WIDE_DECIMAL_NAMESPACE_END)
  #error WIDE_DECIMAL_NAMESPACE_END is not defined. Ensure that <decwide_t_detail_namespace.h> is properly included.
  #endif

  WIDE_DECIMAL_NAMESPACE_BEGIN

  #if(__cplusplus >= 201703L)
  namespace test::independent_algebra {
  #else
  namespace test { namespace independent_algebra { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  template<const std::int32_t MyDigitsBase10,
           typename LimbType = std::uint32_t,
           typename AllocatorType = std::allocator<void>,
           typename InternalFloatType = double,
           typename ExponentType = std::int64_t,
           typename FftFloatType = double>
  struct control;

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType>
  struct control
  {
  private:
    using local_wide_decimal_type =
      math::wide_decimal::decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

  public:
    static constexpr auto my_tol() -> local_wide_decimal_type
    {
      return std::numeric_limits<local_wide_decimal_type>::epsilon() * 1000U;
    }

    static auto eval_eq(const independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                        const independent_algebra_test_decwide_t_boost_cpp<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b) -> bool
    {
      std::string str_b;

      b.get_string(str_b);

      const local_wide_decimal_type decwide_t_b(str_b.c_str());

      const local_wide_decimal_type ratio = a.my_decwide_t / decwide_t_b;

      const local_wide_decimal_type delta = fabs(1 - fabs(ratio));

      const bool compare_is_ok = (delta < my_tol());

      return compare_is_ok;
    }

    static void get_random_float_string(      std::string& str,
                                        const bool         do_seed_random_generators,
                                        const bool         value_is_unsigned = false)
    {
      if(do_seed_random_generators)
      {
        const std::clock_t s = std::clock();

        eng_sign.seed    (s);
        eng_exp.seed     (s);
        eng_mantissa.seed(s);
      }

      str = std::string();

      std::stringstream ss;

      std::uint32_t u { };

      while(str.length() < std::string::size_type(std::numeric_limits<local_wide_decimal_type>::digits10))
      {
        u = dst_mantissa(eng_mantissa);

        ss << std::setw(8) << std::setfill('0') << u;

        str += ss.str();

        ss.rdbuf()->str(std::string(""));
      }

      std::uint32_t u_sign = dst_sign(eng_sign);

      if(u_sign == 0U)
      {
        str += "E+";
      }
      else
      {
        str += "E-";
      }

      std::uint32_t u_exp = dst_exp(eng_exp);

      ss << u_exp;

      str += ss.str();

      ss.rdbuf()->str(std::string(""));

      u = dst_mantissa(eng_mantissa);

      ss << u;

      u_sign = dst_sign(eng_sign);

      str = (value_is_unsigned ? "" : ((u_sign == 0U) ? "" : "-")) + ss.str() + "." + str;
    }

  private:
    static std::ranlux24    eng_sign;     // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static std::minstd_rand eng_exp;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static std::mt19937     eng_mantissa; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    static std::uniform_int_distribution<std::uint32_t> dst_sign;     // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static std::uniform_int_distribution<std::uint32_t> dst_exp;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static std::uniform_int_distribution<std::uint32_t> dst_mantissa; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  };

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::ranlux24 control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_sign; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp)

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::minstd_rand control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_exp; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp)

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::mt19937 control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_mantissa; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp)

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_sign(UINT32_C(0), UINT32_C(1)); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_exp(UINT32_C(0), static_cast<std::uint32_t>((static_cast<unsigned long long>(math::wide_decimal::decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType>::decwide_t_digits10) * 6ULL) / 10ULL)); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)

  template<const std::int32_t MyDigitsBase10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_mantissa(UINT32_C(0), UINT32_C(99999999)); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_add_() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_add(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_add(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_sub_() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_sub(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_sub(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_mul_() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_mul(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_mul(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_div_() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_div(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_div(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_sqrt() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)), true);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_sqrt(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()));
          eval_sqrt(result_ef,   independent_algebra_decwide_type               (str_a.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t MyDigitsBase10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_log_() -> bool
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<MyDigitsBase10, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == UINT32_C(0)), true);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type      result_ef;

          eval_log(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()));
          eval_log(result_ef,   independent_algebra_decwide_type               (str_a.c_str()));

          const bool b_ok = independent_algebra_test_decwide_t_control_struct::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const bool result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  #if(__cplusplus >= 201703L)
  } // namespace test::independent_algebra
  #else
  } // namespace independent_algebra
  } // namespace test
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H
