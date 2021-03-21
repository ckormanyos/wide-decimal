///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_
  #define INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_

  #include <atomic>
  #include <iostream>
  #include <random>

  #include <math/wide_decimal/decwide_t.h>

  #include <test/independent_algebra_test_wide_decimal.h>
  #include <test/independent_algebra_test_boost_cpp.h>
  #include <test/parallel_for.h>

  namespace test { namespace independent_algebra {

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType>
  struct control
  {
  private:
    using local_wide_decimal_type =
      math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>;

  public:
    static constexpr local_wide_decimal_type my_tol()
    {
      return std::numeric_limits<local_wide_decimal_type>::epsilon() * 1000U;
    }

    static bool eval_eq(const independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a,
                        const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& b)
    {
      std::string str_b;

      b.get_string(str_b);

      const local_wide_decimal_type decwide_t_b(str_b);

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

      std::uint32_t u;

      while(str.length() < std::string::size_type(std::numeric_limits<local_wide_decimal_type>::digits10))
      {
        u = dst_mantissa(eng_mantissa);

        ss << std::setw(8) << std::setfill('0') << u;

        str += ss.str();

        ss.rdbuf()->str(std::string(""));
      }

      std::uint32_t u_sign = dst_sign(eng_sign);

      str += ((u_sign == 0U) ? "E+" : "E-");

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
    static std::ranlux24    eng_sign;
    static std::minstd_rand eng_exp;
    static std::mt19937     eng_mantissa;

    static std::uniform_int_distribution<std::uint32_t> dst_sign;
    static std::uniform_int_distribution<std::uint32_t> dst_exp;
    static std::uniform_int_distribution<std::uint32_t> dst_mantissa;
  };

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::ranlux24 control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eng_sign;

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::minstd_rand control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eng_exp;

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::mt19937 control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eng_mantissa;

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::dst_sign(UINT32_C(0), UINT32_C(1));

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::dst_exp(UINT32_C(0), (std::uint32_t) (((unsigned long long) math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::decwide_t_digits10 * 6ULL) / 10ULL));

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  std::uniform_int_distribution<std::uint32_t> control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::dst_mantissa(UINT32_C(0), UINT32_C(99999999));

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_add_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U);
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_b, false);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                               a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          const independent_algebra_test_control_type                                                                                               b_ctrl(str_b.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> b_ef  (str_b.c_str());

          independent_algebra_test_control_type                                                                                               result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_add(result_ctrl, a_ctrl, b_ctrl);
          eval_add(result_ef, a_ef, b_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_sub_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U);
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_b, false);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                 a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          const independent_algebra_test_control_type                                                                                 b_ctrl(str_b.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> b_ef  (str_b.c_str());

          independent_algebra_test_control_type                                                                                 result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_sub(result_ctrl, a_ctrl, b_ctrl);
          eval_sub(result_ef, a_ef, b_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_mul_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U);
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_b, false);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                 a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          const independent_algebra_test_control_type                                                                                 b_ctrl(str_b.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> b_ef  (str_b.c_str());

          independent_algebra_test_control_type                                                                                 result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_mul(result_ctrl, a_ctrl, b_ctrl);
          eval_mul(result_ef, a_ef, b_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_div_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U);
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_b, false);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                 a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          const independent_algebra_test_control_type                                                                                 b_ctrl(str_b.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> b_ef  (str_b.c_str());

          independent_algebra_test_control_type                                                                                 result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_div(result_ctrl, a_ctrl, b_ctrl);
          eval_div(result_ef, a_ef, b_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_sqrt()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U, true);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                 a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          independent_algebra_test_control_type                                                                                 result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_sqrt(result_ctrl, a_ctrl);
          eval_sqrt(result_ef, a_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_log_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag log_algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        std::size_t(0U),
        std::size_t(count),
        [&result_is_ok, &log_algebra_test_lock](std::size_t j)
        {
          std::string str_a;

          while(log_algebra_test_lock.test_and_set()) { ; }
          test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::get_random_float_string(str_a, j == 0U, true);
          log_algebra_test_lock.clear();

          const independent_algebra_test_control_type                                                                                 a_ctrl(str_a.c_str());
          const test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> a_ef  (str_a.c_str());

          independent_algebra_test_control_type                                                                                 result_ctrl;
          test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType> result_ef;

          eval_log(result_ctrl, a_ctrl);
          eval_log(result_ef, a_ef);

          const bool b_ok = test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::eval_eq(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    return result_is_ok.load();
  }

  } }

#endif // INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_
