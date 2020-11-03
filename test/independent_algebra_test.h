///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_
  #define INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_

  #include <iostream>
  #include <random>

  #include <math/wide_decimal/decwide_t.h>

  #include <test/independent_algebra_test_wide_decimal.h>
  #include <test/independent_algebra_test_boost_cpp.h>

  namespace test { namespace independent_algebra {

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  struct control
  {
    static bool eval_eq(const independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>&   a,
                        const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& b)
    {
      std::string str_b;

      b.get_string(str_b);

      const math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> decwide_t_b(str_b);

      const math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> ratio = fabs(1 - fabs(a.my_decwide_t / decwide_t_b));

      static const math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> eps =
        std::numeric_limits<math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>>::epsilon() * 10;

      const bool compare_is_ok = (ratio < eps);

      return compare_is_ok;
    }

    static void get_random_float_string(std::string& str,
                                        const bool do_seed_random_generators = false,
                                        const bool value_is_unsigned = false)
    {
      static std::ranlux24    eng_sign;
      static std::minstd_rand eng_exp;
      static std::mt19937     eng_mantissa;

      static std::uniform_int_distribution<std::uint32_t> dst_sign    (UINT32_C(0), UINT32_C(1));
      static std::uniform_int_distribution<std::uint32_t> dst_exp     (UINT32_C(0), (std::uint32_t) (((unsigned long long) math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>::decwide_t_digits10 * 6ULL) / 10ULL));
      static std::uniform_int_distribution<std::uint32_t> dst_mantissa(UINT32_C(0), UINT32_C(99999999));

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

      while(str.length() < std::string::size_type(std::numeric_limits<math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>>::digits10))
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
  };

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_add_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;
    const std::uint32_t count = CountN;
    const std::uint32_t round = RoundN;

    bool result_is_ok = true;

    std::string str_a;
    std::string str_b;

    std::uint32_t i = 0U;
    std::uint32_t j = 0U;

    for( ; i < round && result_is_ok; ++i)
    {
      for(j = 0U; j < count && result_is_ok; ++j)
      {
        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_a, j == 0U);
        independent_algebra_test_control_type                                                                               a_ctrl(str_a.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> a_ef  (str_a.c_str());

        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_b);
        independent_algebra_test_control_type                                                                               b_ctrl(str_b.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> b_ef  (str_b.c_str());

        independent_algebra_test_control_type                                                                               result_ctrl;
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> result_ef;

        eval_add(result_ctrl, a_ctrl, b_ctrl);
        eval_add(result_ef,   a_ef,   b_ef);

        result_is_ok &= test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::eval_eq(result_ef, result_ctrl);
      }
    }

    return ((i == round) && (j == count));
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_sub_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;
    const std::uint32_t count = CountN;
    const std::uint32_t round = RoundN;

    bool result_is_ok = true;

    std::string str_a;
    std::string str_b;

    std::uint32_t i = 0U;
    std::uint32_t j = 0U;

    for( ; i < round && result_is_ok; ++i)
    {
      for(j = 0U; j < count && result_is_ok; ++j)
      {
        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_a, j == 0U);
        independent_algebra_test_control_type                                                                               a_ctrl(str_a.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> a_ef  (str_a.c_str());

        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_b);
        independent_algebra_test_control_type                                                                               b_ctrl(str_b.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> b_ef  (str_b.c_str());

        independent_algebra_test_control_type                                                                               result_ctrl;
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> result_ef;

        eval_sub(result_ctrl, a_ctrl, b_ctrl);
        eval_sub(result_ef,   a_ef,   b_ef);

        result_is_ok &= test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::eval_eq(result_ef, result_ctrl);
      }
    }

    return ((i == round) && (j == count));
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_mul_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;
    const std::uint32_t count = CountN;
    const std::uint32_t round = RoundN;

    bool result_is_ok = true;

    std::string str_a;
    std::string str_b;

    std::uint32_t i = 0U;
    std::uint32_t j = 0U;

    for( ; i < round && result_is_ok; ++i)
    {
      for(j = 0U; j < count && result_is_ok; ++j)
      {
        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_a, j == 0U);
        independent_algebra_test_control_type                                                                               a_ctrl(str_a.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> a_ef  (str_a.c_str());

        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_b);
        independent_algebra_test_control_type                                                                               b_ctrl(str_b.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> b_ef  (str_b.c_str());

        independent_algebra_test_control_type                                                                               result_ctrl;
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> result_ef;

        eval_mul(result_ctrl, a_ctrl, b_ctrl);
        eval_mul(result_ef,   a_ef,   b_ef);

        result_is_ok &= test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::eval_eq(result_ef, result_ctrl);
      }
    }

    return ((i == round) && (j == count));
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_div_()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;
    const std::uint32_t count = CountN;
    const std::uint32_t round = RoundN;

    bool result_is_ok = true;

    std::string str_a;
    std::string str_b;

    std::uint32_t i = 0U;
    std::uint32_t j = 0U;

    for( ; i < round && result_is_ok; ++i)
    {
      for(j = 0U; j < count && result_is_ok; ++j)
      {
        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_a, j == 0U);
        independent_algebra_test_control_type                                                                               a_ctrl(str_a.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> a_ef  (str_a.c_str());

        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_b);
        independent_algebra_test_control_type                                                                               b_ctrl(str_b.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> b_ef  (str_b.c_str());

        independent_algebra_test_control_type                                                                               result_ctrl;
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> result_ef;

        eval_div(result_ctrl, a_ctrl, b_ctrl);
        eval_div(result_ef,   a_ef,   b_ef);

        result_is_ok &= test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::eval_eq(result_ef, result_ctrl);
      }
    }

    return ((i == round) && (j == count));
  }

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  bool independent_algebra_test_sqrt()
  {
    using independent_algebra_test_control_type = IndependentAlgebraTestControlType;
    const std::uint32_t count = CountN;
    const std::uint32_t round = RoundN;

    bool result_is_ok = true;

    std::string str_a;
    std::string str_b;

    std::uint32_t i = 0U;
    std::uint32_t j = 0U;

    for( ; i < round && result_is_ok; ++i)
    {
      for(j = 0U; j < count && result_is_ok; ++j)
      {
        test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::get_random_float_string(str_a, j == 0U, true);
        independent_algebra_test_control_type                                                                               a_ctrl(str_a.c_str());
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> a_ef  (str_a.c_str());

        independent_algebra_test_control_type                                             result_ctrl;
        test::independent_algebra::independent_algebra_test_decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType> result_ef;

        eval_sqrt(result_ctrl, a_ctrl);
        eval_sqrt(result_ef,   a_ef);

        result_is_ok &= test::independent_algebra::control<MyDigits10, LimbType, AllocatorType, InternalFloatType>::eval_eq(result_ef, result_ctrl);
      }
    }

    return ((i == round) && (j == count));
  }

  } }

#endif // INDEPENDENT_ALGEBRA_TEST_2020_10_17_H_
