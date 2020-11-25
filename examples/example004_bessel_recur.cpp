///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  template<typename FloatingPointType>
  static FloatingPointType bisect(FloatingPointType (*const func)(const FloatingPointType),
                                  const FloatingPointType x_lo,
                                  const FloatingPointType x_hi)
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type f_lo = func(x_lo);
    const floating_point_type f_hi = func(x_hi);

    const bool f_lo_is_neg = (f_lo < floating_point_type(0.0L));
    const bool f_hi_is_neg = (f_hi < floating_point_type(0.0L));

    // Make sure that there is at least one root in the interval.
    if(f_lo_is_neg == f_hi_is_neg)
    {
      std::cerr << "Error: Bisection interval has no root or it has multiple roots!" << std::endl;

      return floating_point_type(0);
    }

    // Orient the search such that f > 0 lies at x + dx.
    floating_point_type dx;
    floating_point_type rtb;

    if(f_lo_is_neg)
    {
      dx  = x_hi - x_lo;
      rtb = x_lo;
    }
    else
    {
      dx  = x_lo - x_hi;
      rtb = x_hi;
    }

    // Bisection iteration loop, maximum 64 times.
    for(std::uint_fast8_t i = 0U; i < 64U; ++i)
    {
      dx /= 2;

      const floating_point_type xmid = rtb + dx;
      const floating_point_type fmid = func(xmid);

      if(fmid <= floating_point_type(0.0L))
      {
        rtb = xmid;
      }

      // Test for convergence.
      using std::fabs;

      if(   (fabs(dx)   < floating_point_type(0.5L))
         || (fabs(fmid) < std::numeric_limits<floating_point_type>::epsilon()))
      {
        // Return root.
        return rtb;
      }
    }

    // Bisection iteration did not converge.
    std::cerr << "Error: Bisection iteration did not converge!" << std::endl;

    return floating_point_type(0);
  }

  struct Jn_algo
  {
    static constexpr double e_half() { return double(1.359140914229522617680143735676331248879L); }
    static constexpr double two_pi() { return double(6.283185307179586476925286766559005768394L); }

    static double       m_z;
    static double       m_n;
    static std::int32_t m_p;

    static double fn_mstart1(const double m)
    {
      // This equation corresponds to | Jm(z) | = 10^(-p)

      //                          1             e * z
      // Jm asymptotic --> ----------------  * [-----]^m = 10^-p
      //                   sqrt(2 * pi * m)     2 * m
      //
      // The resulting root equation is:
      // p + log10(Jnu asymptotic) = 0

      return   static_cast<double>(m_p)
             - (0.5 * std::log10(two_pi() * m))
             + (  m * std::log10(e_half() * m_z / m));
    }

    static double fn_mstart2(const double m)
    {
      // This equation corresponds to | Jm(z) | = 10^(-p / 2) * | Jn(z)|

      //                          1             e * z                         1             e * z
      // Jm asymptotic --> ----------------  * [-----]^m = 10^(-p/2) * ----------------  * [-----]^n
      //                   sqrt(2 * pi * m)     2 * m                  sqrt(2 * pi * n)     2 * n
      //
      // The resulting root equation is:
      // p + log10(Jnu asymptotic) = 0

      return   (static_cast<double>(m_p) / 2.0)
             - (0.5 * std::log10(two_pi() * m))
             + (  m * std::log10(e_half() * m_z / m))
             + (0.5 * std::log10(two_pi() * m_n))
             - (m_n * std::log10(e_half() * m_z / m_n));
    }

    static std::uint32_t mstart1(const double x, const std::uint32_t digits)
    {
      m_z = ((x < 0.1) ? 0.1 : x);
      m_p = digits;

      // Get the starting order for recursion.
      const double        d_order = bisect(fn_mstart1, 0.1, x + 100000.0);
      const std::uint32_t n_order = static_cast<std::uint32_t>(d_order);

      // Make sure the return value is an odd integer.
      return ((((n_order % 2) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
    }

    static std::uint32_t mstart2(const double x, const double dn, const std::uint32_t digits)
    {
      m_z = ((x < 0.1) ? 0.1 : x);
      m_n = dn;
      m_p = digits;

      // Get the starting order for recursion.
      const double        d_order = bisect(fn_mstart2, 0.1, x + 100000.0);
      const std::uint32_t n_order = static_cast<std::uint32_t>(d_order);

      // Make sure the return value is an odd integer.
      return ((((n_order % 2U) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
    }
  };

  double        Jn_algo::m_z;
  double        Jn_algo::m_n;

  std::int32_t  Jn_algo::m_p;

  template<typename FloatingPointType>
  FloatingPointType cyl_bessel_j(const std::int32_t n, const FloatingPointType x)
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type one_over_x = floating_point_type(1U) / x;

    floating_point_type my_jn_result(0U);

    // Start recursion using two J's, Jn+2 and Jn+1.
    // Arbitrarily set Jn+2 = 0 and Jn+1 = 1.
    floating_point_type jn_p2        (0U);
    floating_point_type jn_p1        (1U);
    floating_point_type normalization(2U);

    const std::uint32_t d10 = static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::digits10);

    // Get the starting order for recursive calculations.
    const std::int32_t n_start1 = Jn_algo::mstart1((double) x,        d10);
    const std::int32_t n_start2 = Jn_algo::mstart2((double) x, n - 1, d10);

    const std::int32_t n_start = ((n_start2 > n_start1) ? n_start2 : n_start1);

    // Do the recursion. The direction of the recursion is downward.
    for(std::int32_t m = n_start; m >= 0; --m)
    {
      //                                 Jn+1(x)
      // Downward recursion is:  Jn(x) = ------- * [2 * (m + 1)] - Jn+2(x)
      //                                    x 
      const floating_point_type jn = ((jn_p1 * one_over_x) * (2 * (m + 1))) - jn_p2;

      jn_p2 = jn_p1;
      jn_p1 = jn;

      // Normalization.
      // Here we are using the Neumann expansion which is:
      // 1 = J_0(x) + 2 * J_2(x) + 2 * J_4(x) + 2 * J_6(x) + ...
      if((m % 2) == 0)
      {
        normalization += ((m == 0) ? jn : jn * 2);
      }

      // Store the requested value of jn in the result.
      if(m == n)
      {
        my_jn_result = jn;
      }
    }

    // Divide by the normalization in order to get the scaled result.
    my_jn_result /= normalization;

    return my_jn_result;
  }
}

bool math::wide_decimal::example004_bessel_recur()
{
  // Calculate 1,001 decimal digits of the value
  // of BesselJ[11, 123456 / 10000].

  using local_limb_type = std::uint32_t;

  using dec1001_t = math::wide_decimal::decwide_t<1001U, local_limb_type>;

  // Downward recursion and simultaneous summation of
  // the normalization factor.
  const dec1001_t x(dec1001_t(123456UL) / 10000UL);

  const dec1001_t j11 = cyl_bessel_j(11, x);

  // N[BesselJ[11, 123456/10000], 1001]
  const dec1001_t control
  {
    "0."
    "2854648779524616535183536588658952665496110230319775271653953982988373045782875722005257812943076413"
    "5219919031852548921057823117511572363285931857956532603136154612501901803627994485366625392801445968"
    "0179064377369504730701064123207974143266514980294280981695316245988486923803620776576493284780487935"
    "6897938167846362845392168866601195036145853710502910150057897836163936564591134207959570361640355604"
    "9637548707216344075770888546774787939183202305240737451548878990025949853858657726554398111070195291"
    "5342804986080979558947833829271245593843970212885326505332336197917360981510793642913321447564236429"
    "6391523955159317700006587129511149503235699915565022231095930063757028967259898462554837626548114947"
    "4753883428529025644272705182907981744985178020055447408873185113432724375943351347508264382142242942"
    "6728096886862931667462496751087265806415745631155270478523125080498284822173824022367675073886613450"
    "7710871867223914623961422295329821061959789802564439250529695504489840832821875204223800346981148185"
  };

  const dec1001_t delta_relative = fabs(1 - (j11 / control));

  std::cout << delta_relative << std::endl;

  const bool result_is_ok = delta_relative < (std::numeric_limits<dec1001_t>::epsilon() * 100);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example004_bessel_recur();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif

