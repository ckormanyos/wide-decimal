///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <math/wide_decimal/decwide_t.h>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/cbrt.hpp>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;
}

namespace boost { namespace math { namespace policies {

// Specialization of the precision structure.
template<typename ThisPolicy>
struct precision<dec1001_t, ThisPolicy>
{
  using precision_type = typename ThisPolicy::precision_type;

  using local_digits2_type = digits2<(((long long) std::numeric_limits<dec1001_t>::digits10 + 1LL) * 1000LL) / 301LL>;

  using type = typename std::conditional<false, local_digits2_type, precision_type>::type;
};

} } } // namespaces boost::math::policies


namespace boost{ namespace math{ namespace constants{ namespace detail{

template<>
template<int N>
inline dec1001_t constant_pi<dec1001_t>::compute(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC((boost::integral_constant<int, N>)))
{
  BOOST_MATH_STD_USING

  dec1001_t result;
  dec1001_t a = 1;
  dec1001_t b;
  dec1001_t A(a);
  dec1001_t B = 0.5F;
  dec1001_t D = 0.25F;

  dec1001_t lim;
  lim = boost::math::tools::epsilon<dec1001_t>();

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

} } } }

bool math::wide_decimal::example009_boost_math_standalone()
{
  const dec1001_t x = dec1001_t(UINT32_C(123456789)) / 100U;

  const dec1001_t c       = cbrt(x / math::wide_decimal::pi<wide_decimal_digits10, typename dec1001_t::limb_type, std::allocator<void>, double>());

  const dec1001_t control = boost::math::cbrt(x / boost::math::constants::pi<dec1001_t>());

  const dec1001_t closeness = fabs(1 - (c / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example009_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
