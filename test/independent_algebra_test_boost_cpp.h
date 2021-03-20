///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_
  #define INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_

  #include <algorithm>
  #include <iomanip>
  #include <sstream>

  #include <math/wide_decimal/decwide_t.h>
  #include <test/independent_algebra_test_base.h>

  #if (BOOST_VERSION <= 107500)
  #include <boost/serialization/nvp.hpp>
  #endif
  #include <boost/multiprecision/cpp_bin_float.hpp>
  #include <boost/math/constants/constants.hpp>

  namespace test { namespace independent_algebra {

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  class independent_algebra_test_boost_cpp : public independent_algebra_test_base
  {
  public:
    using other_decwide_t_type = math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>;

    using local_float_type =
      boost::multiprecision::number<boost::multiprecision::cpp_bin_float<std::numeric_limits<other_decwide_t_type>::digits10 + 3>,
                                    boost::multiprecision::et_off>;

    local_float_type my_cpp_bin_float;

    independent_algebra_test_boost_cpp() : my_cpp_bin_float() { }

    independent_algebra_test_boost_cpp(const char* str)
      : my_cpp_bin_float(str) { }

    virtual ~independent_algebra_test_boost_cpp() { }

    virtual void get_string(std::string& str) const
    {
      std::stringstream ss;

      ss << std::scientific
         << std::uppercase
         << std::setprecision(std::streamsize(std::numeric_limits<local_float_type>::digits10 + 1))
         << my_cpp_bin_float;

      str = ss.str();
    }

    static local_float_type my_log(local_float_type x)
    {
      using floating_point_type = local_float_type;

      // For values less than 1 invert the argument and
      // remember (in this case) to negate the result below.
      const bool b_negate = (x < 1);

      const floating_point_type xx((b_negate == false) ? x : 1 / x);

      // Use an AGM method to compute the logarithm of x.
      // Set a0 = 1
      // Set b0 = 4 / (x * 2^m)
      //        = 1 / (x * 2^(m - 2))

      floating_point_type ak(1U);

      constexpr float n_times_factor = ((float) std::numeric_limits<floating_point_type>::digits10) * 1.67F;

      // Extract lg_xx = Log[mantissa * radix^ib]
      //               = Log[mantissa] + ib * Log[radix]

      using std::ilogb;
      using std::log;
      using std::scalbn;

      const std::int32_t        ib    = ilogb(xx);
      const floating_point_type man   = xx / scalbn(floating_point_type(1U), (int) ib);
      const float               lg_xx =   log((float) man)
                                        + ((float) ib * log((float) std::numeric_limits<floating_point_type>::radix));

      // Ensure that the resulting power is non-negative.
      // Also enforce that m >= 3.
      const std::int32_t m =
        (std::max)((std::int32_t) (n_times_factor - (float) (lg_xx / log(2.0F))), (std::int32_t) 3);

      using std::ldexp;

      floating_point_type bk =
        ldexp(floating_point_type(1U), (std::int32_t) (2 - m)) / xx;

      // TBD: Tolerance should have the log of the argument added to it (usually negligible).
      constexpr std::uint32_t digits10_iteration_goal =
          (std::uint32_t) ((std::numeric_limits<floating_point_type>::digits10 + 1) / 2)
        + (std::uint32_t) 9U;

      const std::uint32_t digits10_scale =
        (std::uint32_t) (0.5F + (1000.0F * log((float) std::numeric_limits<floating_point_type>::radix)) / log(10.0F));

      for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
      {
        // Check for the number of significant digits to be
        // at least half of the requested digits. If at least
        // half of the requested digits have been achieved,
        // then break after the upcoming iteration.

        const std::int32_t ilogb_of_ak_minus_bk = (std::max)(std::int32_t(0), -ilogb(ak - bk));

        const floating_point_type ak_tmp(ak);

        ak += bk;

        if(k > 4)
        {
          const std::uint32_t digits10_of_iteration =
            (std::uint32_t) ((std::uint64_t) ((std::uint64_t) ilogb_of_ak_minus_bk * digits10_scale) / 1000U);

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
               boost::math::constants::pi<floating_point_type>() / ak
        - (boost::math::constants::ln_two<floating_point_type>() * m);

      return ((b_negate == true) ? -result : result);
    }
  };

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_add(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float + b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_sub(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float - b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_mul(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float * b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_div(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float / b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_sqrt(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                 const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a)
  {
    result.my_cpp_bin_float = sqrt(a.my_cpp_bin_float);
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType, typename ExponentType>
  void eval_log(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>& a)
  {
    using boost_multiprecision_type =
      typename independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::local_float_type;

    const boost_multiprecision_type lg_a =
      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>::my_log(a.my_cpp_bin_float);

    result.my_cpp_bin_float = lg_a;
  }

  } }

#endif // INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_
