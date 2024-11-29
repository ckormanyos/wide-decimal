///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H // NOLINT(llvm-header-guard)
  #define INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H

  #include <math/wide_decimal/decwide_t.h>

  #include <test/independent_algebra_test_decwide_t_boost_cpp.h>
  #include <test/independent_algebra_test_decwide_t_wide_decimal.h>
  #include <test/parallel_for.h>

  #include <util/utility/util_pseudorandom_time_point_seed.h>

  #include <atomic>
  #include <random>
  #include <sstream>
  #include <string>

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

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType = std::uint32_t,
           typename AllocatorType = std::allocator<void>,
           typename InternalFloatType = double,
           typename ExponentType = std::int64_t,
           typename FftFloatType = double>
  struct control;

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType>
  struct control
  {
  private:
    #if defined(WIDE_DECIMAL_NAMESPACE)
    using local_wide_decimal_type =
      WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;
    #else
    using local_wide_decimal_type =
      ::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;
    #endif

    using local_eng_sgn_type = std::ranlux24;
    using local_eng_exp_type = std::minstd_rand;
    using local_eng_man_type = std::mt19937;

    using local_dst_type = std::uniform_int_distribution<std::uint32_t>;

    template<typename MyControlFloatType>
    static auto my_tol_ctrl() -> const MyControlFloatType&
    {
      using local_float_control_type = MyControlFloatType;

      static const auto my_local_control_tol =
        []() // NOLINT(modernize-use-trailing-return-type)
        {
          using std::log;
          using std::lround;

          const auto digits10_scale =
            static_cast<std::uint32_t>
            (
              lround
              (
                static_cast<float>
                (
                    static_cast<float>
                    (
                      1000.0F * log(static_cast<float>(std::numeric_limits<local_float_control_type>::radix))
                    )
                  / log(10.0F)
                )
              )
            );

          using std::ilogb;

          const auto approx_ilogb_of_tol =
            static_cast<std::intmax_t>
            (
              ilogb(my_tol()) + INTMAX_C(1)
            );

          const auto approx_digits10_of_tol =
            static_cast<std::uint32_t>
            (
                static_cast<std::uint64_t>(static_cast<std::uint64_t>(approx_ilogb_of_tol) * digits10_scale)
              / static_cast<std::uint32_t>(UINT16_C(1000))
            );

          auto str_approx_digits10_of_tol = std::to_string(approx_digits10_of_tol);

          str_approx_digits10_of_tol.insert(str_approx_digits10_of_tol.begin(), 'E');
          str_approx_digits10_of_tol.insert(str_approx_digits10_of_tol.begin(), '1');

          return local_float_control_type(str_approx_digits10_of_tol);
        }();

      return my_local_control_tol;
    }

  public:
    static constexpr auto my_tol() -> local_wide_decimal_type
    {
      return   std::numeric_limits<local_wide_decimal_type>::epsilon()
             * static_cast<std::uint32_t>(UINT32_C(1000));
    }

    static auto eval_equal(const independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                           const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b,
                           const bool use_fixed = false) -> bool
    {
      auto compare_is_ok = true;

      const auto compare_b_is_ok =
        [&a, &b, &use_fixed]() // NOLINT(modernize-use-trailing-return-type)
        {
          std::string str_b;

          b.get_string(str_b, use_fixed);

          const local_wide_decimal_type decwide_t_b(str_b.c_str());

          const local_wide_decimal_type ratio = a.my_decwide_t / decwide_t_b;

          const local_wide_decimal_type delta = fabs(1 - fabs(ratio));

          return (delta < my_tol());
        }();

      using local_float_control_type =
        typename independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_float_type;

      const auto compare_a_is_ok =
        [&a, &b, &use_fixed]() // NOLINT(modernize-use-trailing-return-type)
        {
          std::string str_a;

          a.get_string(str_a, use_fixed);

          const local_float_control_type control_type_a(str_a.c_str());

          const local_float_control_type ratio = b.my_cpp_boost_float / control_type_a;

          const local_float_control_type delta = fabs(1 - fabs(ratio));

          return (delta < my_tol_ctrl<local_float_control_type>());
        }();

      compare_is_ok = (compare_is_ok && (compare_a_is_ok && compare_b_is_ok));

      return compare_is_ok;
    }

    static void get_random_float_string(      std::string& str, // NOLINT(google-runtime-references)
                                        const bool         do_seed_random_generators,
                                        const bool         value_is_unsigned = false)
    {
      if(do_seed_random_generators)
      {
        eng_sgn.seed(util::util_pseudorandom_time_point_seed::value<typename local_eng_sgn_type::result_type>());
        eng_exp.seed(util::util_pseudorandom_time_point_seed::value<typename local_eng_exp_type::result_type>());
        eng_man.seed(util::util_pseudorandom_time_point_seed::value<typename local_eng_man_type::result_type>());
      }

      str = std::string { };

      std::stringstream strm { };

      std::uint32_t u { };

      while(str.length() < static_cast<std::string::size_type>(std::numeric_limits<local_wide_decimal_type>::digits10))
      {
        u = dst_man(eng_man);

        constexpr std::streamsize
          wd
          {
            static_cast<std::streamsize>
            (
              local_wide_decimal_type::decwide_t_elem_digits10
            )
          };

        strm << std::setw(wd) << std::setfill('0') << u;

        str += strm.str();

        strm.rdbuf()->str(std::string { });
      }

      std::uint32_t u_sign = dst_sgn(eng_sgn);

      if(u_sign == 0U)
      {
        str += "E+";
      }
      else
      {
        str += "E-";
      }

      const std::uint32_t u_exp { dst_exp(eng_exp) };

      strm << u_exp;

      str += strm.str();

      strm.rdbuf()->str(std::string { });

      u = dst_man(eng_man);

      strm << u;

      u_sign = dst_sgn(eng_sgn);

      str = strm.str() + "." + str;

      if((!value_is_unsigned) && (u_sign != static_cast<std::uint32_t>(UINT8_C(0))))
      {
        str.insert(str.begin(), '-');
      }
    }

  private:
    static local_eng_sgn_type eng_sgn; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static local_eng_exp_type eng_exp; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static local_eng_man_type eng_man; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    static local_dst_type     dst_sgn; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static local_dst_type     dst_exp; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static local_dst_type     dst_man; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  };

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_eng_sgn_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_sgn; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_eng_exp_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_exp; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_eng_man_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::eng_man; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,cert-msc32-c,cert-msc51-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_dst_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_sgn(static_cast<std::uint32_t>(UINT8_C(0)), static_cast<std::uint32_t>(UINT8_C(1))); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)

  #if defined(WIDE_DECIMAL_NAMESPACE)
  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_dst_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_exp(static_cast<std::uint32_t>(UINT8_C(0)), static_cast<std::uint32_t>((static_cast<std::uintmax_t>(WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType>::decwide_t_digits10) * UINTMAX_C(6)) / UINTMAX_C(10))); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)
  #else
  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_dst_type control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_exp(static_cast<std::uint32_t>(UINT8_C(0)), static_cast<std::uint32_t>((static_cast<std::uintmax_t>(::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType>::decwide_t_digits10) * UINTMAX_C(6)) / UINTMAX_C(10))); // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)
  #endif

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  typename control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_dst_type // NOLINT(hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)
    control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::dst_man               // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp,hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)
    (
      static_cast<std::uint32_t>(UINT8_C(0)),
      static_cast<std::uint32_t>
      (
          control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_wide_decimal_type::decwide_t_elem_mask // NOLINT(hicpp-uppercase-literal-suffix,readability-uppercase-literal-suffix)
        - static_cast<std::int32_t>(INT8_C(1))
      )
    );

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_add_() -> bool // NOLINT(readability-identifier-naming)
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        static_cast<std::size_t>(0U),
        static_cast<std::size_t>(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type                result_ef;

          eval_add(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_add(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const auto result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_sub_() -> bool // NOLINT(readability-identifier-naming)
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        static_cast<std::size_t>(0U),
        static_cast<std::size_t>(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type                result_ef;

          eval_sub(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_sub(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const auto result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_mul_() -> bool // NOLINT(readability-identifier-naming)
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        static_cast<std::size_t>(0U),
        static_cast<std::size_t>(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type                result_ef;

          eval_mul(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_mul(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const auto result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_div_() -> bool // NOLINT(readability-identifier-naming)
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        static_cast<std::size_t>(0U),
        static_cast<std::size_t>(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;
          std::string str_b;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))));
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_b, false);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type                result_ef;

          eval_div(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()), independent_algebra_test_decwide_t_control_type(str_b.c_str()));
          eval_div(result_ef,   independent_algebra_decwide_type               (str_a.c_str()), independent_algebra_decwide_type     (str_b.c_str()));

          const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const auto result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t ParamDigitsBaseTen,
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
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    constexpr std::uint32_t count = CountN;
    constexpr std::uint32_t round = RoundN;

    std::atomic_bool result_is_ok;
    result_is_ok.store(true);

    for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
    {
      std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        static_cast<std::size_t>(0U),
        static_cast<std::size_t>(count),
        [&result_is_ok, &algebra_test_lock](std::size_t j)
        {
          std::string str_a;

          while(algebra_test_lock.test_and_set()) { ; }
          independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))), true);
          algebra_test_lock.clear();

          independent_algebra_test_decwide_t_control_type result_ctrl;
          independent_algebra_decwide_type                result_ef;

          eval_sqrt(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()));
          eval_sqrt(result_ef,   independent_algebra_decwide_type               (str_a.c_str()));

          const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl);

          result_is_ok.store(b_ok);
        }
      );
    }

    const auto result_test_is_ok = result_is_ok.load();

    return result_test_is_ok;
  }

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  struct independent_algebra_test_decwide_t_log_holder
  {
    using independent_algebra_test_decwide_t_control_type = IndependentAlgebraTestControlType;

    using independent_algebra_decwide_type =
      test::independent_algebra::independent_algebra_test_decwide_t_decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    using independent_algebra_test_decwide_t_control_struct =
      test::independent_algebra::control<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;

    static auto do_test() -> bool
    {
      constexpr std::uint32_t count = CountN;
      constexpr std::uint32_t round = RoundN;

      std::atomic_bool result_is_ok;
      result_is_ok.store(true);

      for(std::uint32_t i = 0U; i < round && result_is_ok; ++i)
      {
        std::atomic_flag algebra_test_lock = ATOMIC_FLAG_INIT;

        my_concurrency::parallel_for
        (
          static_cast<std::size_t>(0U),
          static_cast<std::size_t>(count),
          [&result_is_ok, &algebra_test_lock](std::size_t j)
          {
            std::string str_a;

            while(algebra_test_lock.test_and_set()) { ; }
            independent_algebra_test_decwide_t_control_struct::get_random_float_string(str_a, (j == static_cast<std::size_t>(UINT8_C(0))), true);
            algebra_test_lock.clear();

            independent_algebra_test_decwide_t_control_type result_ctrl;
            independent_algebra_decwide_type                result_ef;

            eval_log(result_ctrl, independent_algebra_test_decwide_t_control_type(str_a.c_str()));
            eval_log(result_ef,   independent_algebra_decwide_type               (str_a.c_str()));

            const auto b_ok = independent_algebra_test_decwide_t_control_struct::eval_equal(result_ef, result_ctrl, true);

            result_is_ok.store(b_ok);
          }
        );
      }

      const auto result_test_is_ok = result_is_ok.load();

      return result_test_is_ok;
    }
  };

  template<const std::int32_t ParamDigitsBaseTen,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename FftFloatType,
           const std::uint32_t CountN,
           const std::uint32_t RoundN,
           typename IndependentAlgebraTestControlType>
  auto independent_algebra_test_decwide_t_log_() -> bool // NOLINT(readability-identifier-naming)
  {
    using independent_algebra_test_decwide_t_log_holder_type =
      independent_algebra_test_decwide_t_log_holder
        <ParamDigitsBaseTen,
         LimbType,
         AllocatorType,
         InternalFloatType,
         ExponentType,
         FftFloatType,
         CountN,
         RoundN,
         IndependentAlgebraTestControlType>;

    return independent_algebra_test_decwide_t_log_holder_type::do_test();
  }

  #if(__cplusplus >= 201703L)
  } // namespace test::independent_algebra
  #else
  } // namespace independent_algebra
  } // namespace test
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_2020_10_17_H
