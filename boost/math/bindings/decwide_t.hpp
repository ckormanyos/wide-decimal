///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef DECWIDE_T_2021_02_24_HPP_
  #define DECWIDE_T_2021_02_24_HPP_

  #include <boost/math/policies/policy.hpp>
  #include <math/wide_decimal/decwide_t.h>

  namespace boost { namespace math {

  namespace policies {

  // Specialization of the precision structure.
  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType,
           typename ThisPolicy>
  struct precision<::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>, ThisPolicy>
  {
    using local_wide_decimal_type =
      ::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>;

    using precision_type = typename ThisPolicy::precision_type;

    using local_digits_2 = digits2<(((long long) std::numeric_limits<local_wide_decimal_type>::digits10 + 1LL) * 1000LL) / 301LL>;

    using type = typename std::conditional<((local_digits_2::value <= precision_type::value) || (precision_type::value <= 0)),
                                            local_digits_2,
                                            precision_type>::type;
  };

  } // namespace policies

  namespace constants { namespace detail {

  template <class T>
  struct constant_pi;

  template<const std::int32_t MyDigits10,
           typename LimbType,
           typename AllocatorType,
           typename InternalFloatType,
           typename ExponentType>
  struct constant_pi<::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>>
  {
  public:
    using result_type =
      ::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType, ExponentType>;

  private:
    static result_type my_compute()
    {
      // Adapted from Boost.Math.Constants (see file calculate_constants.hpp).
      using std::ldexp;
      using std::sqrt;

      result_type result;
      result_type a = 1;
      result_type b;
      result_type A(a);
      result_type B = 0.5F;
      result_type D = 0.25F;

      result_type lim = std::numeric_limits<result_type>::epsilon();

      unsigned k = 1;

      do
      {
        result = A + B;
        result = ldexp(result, -2);
        b = sqrt(B);
        a += b;
        a = ldexp(a, -1);
        A = a * a;
        B = A - result;
        B = ldexp(B, 1);
        result = A - B;
        bool neg = boost::math::sign(result) < 0;
        if(neg)
           result = -result;
        if(result <= lim)
           break;
        if(neg)
           result = -result;
        result = ldexp(result, k - 1);
        D -= result;
        ++k;
        lim = ldexp(lim, 1);
      }
      while(true);

      result = B / D;
      return result;
    }

  public:
    #if (BOOST_VERSION <= 107200)
    template<int N> static const result_type& get(const mpl::int_<N>&)
    #elif (BOOST_VERSION <= 107500)
    template<int N> static const result_type& get(const boost::integral_constant<int, N>&)
    #else
    template<int N> static const result_type& get(const std::integral_constant<int, N>&)
    #endif
    {
      static result_type result;
      static bool        has_init = false;

      if(has_init == false)
      {
        has_init = true;

        result = my_compute();
      }

      return result;
    }

    #if (BOOST_VERSION <= 107200)
    static inline const result_type get(const mpl::int_<0>&)
    #elif (BOOST_VERSION <= 107500)
    static inline const result_type get(const boost::integral_constant<int, 0>&)
    #else
    static inline const result_type get(const std::integral_constant<int, 0>&)
    #endif
    {
      return my_compute();
    }
  };

  } } // namespace constants::detail

  } } // namespace boost::math

#endif // DECWIDE_T_2021_02_24_HPP_
