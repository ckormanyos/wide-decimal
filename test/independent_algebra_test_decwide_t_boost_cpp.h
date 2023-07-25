///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BOOST_CPP_2020_10_17_H // NOLINT(llvm-header-guard)
  #define INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BOOST_CPP_2020_10_17_H

  #include <iomanip>
  #include <sstream>

  #include <boost/version.hpp>

  #if !defined(BOOST_VERSION)
  #error BOOST_VERSION is not defined. Ensure that <boost/version.hpp> is properly included.
  #endif

  #if (BOOST_VERSION >= 108000)
  #if !defined(BOOST_NO_EXCEPTIONS)
  #define BOOST_NO_EXCEPTIONS
  #endif
  #if !defined(BOOST_NO_RTTI)
  #define BOOST_NO_RTTI
  #endif
  #endif

  #if ((BOOST_VERSION >= 107700) && !defined(BOOST_MATH_STANDALONE))
  #if (defined(_MSC_VER) && (_MSC_VER < 1920))
  #else
  #define BOOST_MATH_STANDALONE
  #endif
  #endif

  #if ((BOOST_VERSION >= 107900) && !defined(BOOST_MP_STANDALONE))
  #define BOOST_MP_STANDALONE
  #endif

  #include <math/wide_decimal/decwide_t.h>
  #include <test/independent_algebra_test_decwide_t_base.h>
  #include <test/independent_algebra_test_decwide_t_constants.h>

  #if defined(DECWIDE_T_TEST_OPTION_TEST_CPP_DEC_FLOAT)
  #include <boost/multiprecision/cpp_dec_float.hpp>
  #else
  #include <boost/multiprecision/cpp_bin_float.hpp>
  #endif

  #if defined(BOOST_VERSION) && (BOOST_VERSION <= 107500)
  #include <boost/serialization/nvp.hpp>
  #endif

  WIDE_DECIMAL_NAMESPACE_BEGIN

  #if(__cplusplus >= 201703L)
  namespace test::independent_algebra {
  #else
  namespace test { namespace independent_algebra { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  class independent_algebra_test_decwide_t_boost_cpp : public independent_algebra_test_decwide_t_base
  {
  public:
    #if defined(WIDE_DECIMAL_NAMESPACE)
    using other_decwide_t_type =
      WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;
    #else
    using other_decwide_t_type =
      ::math::wide_decimal::decwide_t<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>;
    #endif

    #if defined(DECWIDE_T_TEST_OPTION_TEST_CPP_DEC_FLOAT)
    using local_float_backend_type =
      boost::multiprecision::cpp_dec_float<std::numeric_limits<other_decwide_t_type>::digits10 + 3,
                                           std::int64_t,
                                           std::allocator<std::uint32_t>>;

    using local_float_type =
      boost::multiprecision::number<local_float_backend_type, boost::multiprecision::et_off>;
    #else
    using local_float_backend_type =
      boost::multiprecision::cpp_bin_float<std::numeric_limits<other_decwide_t_type>::digits10 + 3,
                                           boost::multiprecision::backends::digit_base_10,
                                           std::allocator<boost::multiprecision::limb_type>>;

    using local_float_type =
      boost::multiprecision::number<local_float_backend_type,
                                    boost::multiprecision::et_off>;
    #endif

    local_float_type my_cpp_boost_float; // NOLINT(misc-non-private-member-variables-in-classes)

    independent_algebra_test_decwide_t_boost_cpp() = default;

    explicit independent_algebra_test_decwide_t_boost_cpp(const char* str)
      : my_cpp_boost_float(str) { }

    independent_algebra_test_decwide_t_boost_cpp(const independent_algebra_test_decwide_t_boost_cpp& other)
      : my_cpp_boost_float(other.my_cpp_boost_float) { }

    independent_algebra_test_decwide_t_boost_cpp(independent_algebra_test_decwide_t_boost_cpp&& other) noexcept
      : my_cpp_boost_float(other.my_cpp_boost_float) { }

    ~independent_algebra_test_decwide_t_boost_cpp() override = default;

    auto operator=(const independent_algebra_test_decwide_t_boost_cpp& other) -> independent_algebra_test_decwide_t_boost_cpp&
    {
      if(this != &other)
      {
        my_cpp_boost_float = other.my_cpp_boost_float;
      }

      return *this;
    }

    auto operator=(independent_algebra_test_decwide_t_boost_cpp&& other) noexcept -> independent_algebra_test_decwide_t_boost_cpp&
    {
      my_cpp_boost_float = other.my_cpp_boost_float;

      return *this;
    }

    auto get_string(std::string& str, bool use_fixed) const -> void override
    {
      std::stringstream ss;

      ss << ((!use_fixed) ? std::scientific : std::fixed)
         << std::uppercase
         << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<local_float_type>::digits10 + 1))
         << my_cpp_boost_float;

      str = ss.str();
    }

    static auto my_local_log(const local_float_type& x) -> local_float_type
    {
      using floating_point_type = local_float_type;

      // For values less than 1 invert the argument and
      // remember (in this case) to negate the result below.
      const auto b_negate = (x < 1);

      const floating_point_type xx((!b_negate) ? x : 1 / x);

      // Use an AGM method to compute the logarithm of x.
      // Set a0 = 1
      // Set b0 = 4 / (x * 2^m)
      //        = 1 / (x * 2^(m - 2))

      floating_point_type ak(1U);

      constexpr auto n_times_factor =
        static_cast<float>
        (
          static_cast<float>(std::numeric_limits<floating_point_type>::digits10) * 1.67F
        );

      // Extract lg_xx = Log[mantissa * radix^ib]
      //               = Log[mantissa] + ib * Log[radix]

      using std::ilogb;
      using std::log;
      using std::scalbn;

      const auto                ib    = static_cast<std::int32_t>(ilogb(xx));
      const floating_point_type man   = xx / scalbn(floating_point_type(1U), static_cast<int>(ib));
      const auto                lg_xx =
        static_cast<float>
        (
            log(static_cast<float>(man))
          + static_cast<float>(static_cast<float>(ib) * log(static_cast<float>(std::numeric_limits<floating_point_type>::radix)))
        );

      // Ensure that the resulting power is non-negative.
      // Also enforce that m >= 3.
      const auto m =
        static_cast<std::int32_t>
        (
          (std::max)(static_cast<std::int32_t>(n_times_factor - static_cast<float>(lg_xx / log(2.0F))),
                     static_cast<std::int32_t>(3))
        );

      using std::ldexp;

      floating_point_type bk =
        ldexp(floating_point_type(1U), static_cast<std::int32_t>(2 - m)) / xx;

      // TBD: Tolerance should have the log of the argument added to it (usually negligible).
      constexpr auto digits10_iteration_goal =
        static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>((std::numeric_limits<floating_point_type>::digits10 + 1) / 2)
          + static_cast<std::uint32_t>(9U)
        );

      using std::lround;

      const auto digits10_scale =
        static_cast<std::uint32_t>
        (
          lround
          (
            static_cast<float>(1000.0F * log(static_cast<float>(std::numeric_limits<floating_point_type>::radix))) / log(10.0F)
          )
        );

      for(auto k = static_cast<std::int32_t>(INT8_C(0)); k < static_cast<std::int32_t>(INT8_C(64)); ++k) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      {
        // Check for the number of significant digits to be
        // at least half of the requested digits. If at least
        // half of the requested digits have been achieved,
        // then break after the upcoming iteration.

        const auto ilogb_of_ak_minus_bk = (std::max)(static_cast<std::int32_t>(INT8_C(0)),
                                                     static_cast<std::int32_t>(-ilogb(ak - bk)));

        const floating_point_type ak_tmp(ak);

        ak += bk;

        if(k > 4)
        {
          const auto digits10_of_iteration =
            static_cast<std::uint32_t>
            (
              static_cast<std::uint64_t>(static_cast<std::uint64_t>(ilogb_of_ak_minus_bk) * digits10_scale) / UINT32_C(1000)
            );

          if(digits10_of_iteration > digits10_iteration_goal)
          {
            break;
          }
        }

        using std::sqrt;

        ak /= 2;
        bk  = sqrt(bk * ak_tmp);
      }

      // We are now finished with the AGM iteration for log(x).
      // Compute log(x) = {pi / [2 * AGM(1, 4 / 2^m)]} - (m * ln2)
      // Note at this time that (ak = bk) = AGM(...)
      // Retrieve the value of pi, divide by (2 * a) and subtract (m * ln2).

      const floating_point_type result =
          (constants<floating_point_type>::my_value_pi    () / ak)
        - (constants<floating_point_type>::my_value_ln_two() * m);

      return (b_negate ? -result : result);
    }
  };

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_add(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b) -> void
  {
    result.my_cpp_boost_float = a.my_cpp_boost_float + b.my_cpp_boost_float;
  }

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_sub(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b) -> void
  {
    result.my_cpp_boost_float = a.my_cpp_boost_float - b.my_cpp_boost_float;
  }

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_mul(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b) -> void
  {
    result.my_cpp_boost_float = a.my_cpp_boost_float * b.my_cpp_boost_float;
  }

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_div(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& b) -> void
  {
    result.my_cpp_boost_float = a.my_cpp_boost_float / b.my_cpp_boost_float;
  }

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_sqrt(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                 const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a) -> void
  {
    result.my_cpp_boost_float = sqrt(a.my_cpp_boost_float);
  }

  template<const std::int32_t ParamDigitsBaseTen, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType, typename FftFloatType>
  auto eval_log(      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& result,
                const independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>& a) -> void
  {
    using boost_multiprecision_type =
      typename independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::local_float_type;

    const boost_multiprecision_type lg_a =
      independent_algebra_test_decwide_t_boost_cpp<ParamDigitsBaseTen, LimbType, AllocatorType, InternalFloatType, ExponentType, FftFloatType>::my_local_log(a.my_cpp_boost_float);

    result.my_cpp_boost_float = lg_a;
  }

  #if(__cplusplus >= 201703L)
  } // namespace test::independent_algebra
  #else
  } // namespace independent_algebra
  } // namespace test
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BOOST_CPP_2020_10_17_H
