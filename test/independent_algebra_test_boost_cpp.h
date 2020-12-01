///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_
  #define INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_

  #include <iomanip>
  #include <sstream>

  #include <boost/multiprecision/cpp_bin_float.hpp>
  #include <math/wide_decimal/decwide_t.h>
  #include <test/independent_algebra_test_base.h>

  namespace test { namespace independent_algebra {

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  class independent_algebra_test_boost_cpp : public independent_algebra_test_base
  {
  public:
    using other_decwide_t_type = math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>;

    using float_type =
      boost::multiprecision::number<boost::multiprecision::cpp_bin_float<std::numeric_limits<other_decwide_t_type>::digits10 + 3>,
                                    boost::multiprecision::et_off>;

    float_type my_cpp_bin_float;

    independent_algebra_test_boost_cpp() : my_cpp_bin_float() { }

    independent_algebra_test_boost_cpp(const char* str)
      : independent_algebra_test_base(str),
        my_cpp_bin_float(str) { }

    virtual ~independent_algebra_test_boost_cpp() { }

    virtual void get_string(std::string& str) const
    {
      std::stringstream ss;

      ss << std::scientific
         << std::uppercase
         << std::setprecision(std::streamsize(std::numeric_limits<float_type>::digits10 + 1))
         << my_cpp_bin_float;

      str = ss.str();
    }
  };

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_add(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float + b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_sub(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float - b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_mul(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float * b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_div(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& b)
  {
    result.my_cpp_bin_float = a.my_cpp_bin_float / b.my_cpp_bin_float;
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_sqrt(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                 const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a)
  {
    result.my_cpp_bin_float = sqrt(a.my_cpp_bin_float);
  }

  template<const std::int32_t MyDigits10, typename LimbType, typename AllocatorType, typename InternalFloatType>
  void eval_log(      independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& result,
                const independent_algebra_test_boost_cpp<MyDigits10, LimbType, AllocatorType, InternalFloatType>& a)
  {
    result.my_cpp_bin_float = log(a.my_cpp_bin_float);
  }

  } }

#endif // INDEPENDENT_ALGEBRA_TEST_BOOST_CPP_2020_10_17_H_
