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
           typename ThisPolicy>
  struct precision<::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>, ThisPolicy>
  {
    using local_wide_decimal_type =
      ::math::wide_decimal::decwide_t<MyDigits10, LimbType, AllocatorType, InternalFloatType>;

    using precision_type = typename ThisPolicy::precision_type;

    using local_digits_2 = digits2<(((long long) std::numeric_limits<local_wide_decimal_type>::digits10 + 1LL) * 1000LL) / 301LL>;

    using type = typename std::conditional<((local_digits_2::value <= precision_type::value) || (precision_type::value <= 0)),
                                            local_digits_2,
                                            precision_type>::type;
  };

  } // namespace policies

  } } // namespace boost::math

#endif // DECWIDE_T_2021_02_24_HPP_
