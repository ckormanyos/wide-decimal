///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cmath>
#include <cstdint>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>

namespace example004_bessel
{
  template<typename FloatingPointType>
  auto bisect(      FloatingPointType (*pfn)(FloatingPointType),
              const FloatingPointType x_lo,
              const FloatingPointType x_hi) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type f_lo = pfn(x_lo);
    const floating_point_type f_hi = pfn(x_hi);

    const bool f_lo_is_neg = (f_lo < static_cast<floating_point_type>(0.0L));
    const bool f_hi_is_neg = (f_hi < static_cast<floating_point_type>(0.0L));

    // Make sure that there is at least one root in the interval.
    if(f_lo_is_neg == f_hi_is_neg)
    {
      // Bisection interval has no root or it has multiple roots!

      return static_cast<floating_point_type>(0);
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
    for(auto i = static_cast<std::uint_fast8_t>(0U); i < UINT8_C(64); ++i) // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    {
      dx /= 2;

      const floating_point_type xmid = rtb + dx;
      const floating_point_type fmid = pfn(xmid);

      if(fmid <= static_cast<floating_point_type>(0.0L))
      {
        rtb = xmid;
      }

      // Test for convergence.
      using std::fabs;

      if(   (fabs(dx)   < static_cast<floating_point_type>(0.5L)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
         || (fabs(fmid) < std::numeric_limits<floating_point_type>::epsilon()))
      {
        // Return root.
        return rtb;
      }
    }

    // Bisection iteration did not converge.
    return floating_point_type(0);
  }

  struct Jn_algo
  {
    static constexpr auto e_half() -> float { return static_cast<float>(1.359140914229522617680143735676331248879L); } // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    static constexpr auto two_pi() -> float { return static_cast<float>(6.283185307179586476925286766559005768394L); } // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    static float        m_z; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static float        m_n; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static std::int32_t m_p; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    static auto fn_mstart1(float m) -> float
    {
      // This equation corresponds to | Jm(z) | = 10^(-p)

      //                          1             e * z
      // Jm asymptotic --> ----------------  * [-----]^m = 10^-p
      //                   sqrt(2 * pi * m)     2 * m
      //
      // The resulting root equation is:
      // p + log10(Jnu asymptotic) = 0

      using std::log10;

      return   static_cast<float>(m_p)
             - (0.5F * log10(two_pi() * m)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
             + (  m  * log10(e_half() * m_z / m));
    }

    static auto fn_mstart2(float m) -> float
    {
      // This equation corresponds to | Jm(z) | = 10^(-p / 2) * | Jn(z)|

      //                          1             e * z                         1             e * z
      // Jm asymptotic --> ----------------  * [-----]^m = 10^(-p/2) * ----------------  * [-----]^n
      //                   sqrt(2 * pi * m)     2 * m                  sqrt(2 * pi * n)     2 * n
      //
      // The resulting root equation is:
      // p + log10(Jnu asymptotic) = 0

      using std::log10;

      return   (static_cast<float>(m_p) / 2.0F)      // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
             - (0.5F * log10(two_pi() * m))          // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
             + (  m  * log10(e_half() * m_z / m))
             + (0.5F * log10(two_pi() * m_n))        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
             - (m_n  * log10(e_half() * m_z / m_n));
    }

    static auto mstart1(float x, std::uint32_t digits) -> std::uint32_t
    {
      m_z = (std::max)(x, 0.1F); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      m_p = static_cast<std::int32_t>(digits);

      // Get the starting order for recursion.
      const auto f_order = bisect(fn_mstart1, 0.1F, x + 100000.0F);
      const auto n_order = static_cast<std::uint32_t>(f_order);

      // Make sure the return value is an odd integer.
      return ((((n_order % 2U) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
    }

    static auto mstart2(float x, float dn, std::uint32_t digits) -> std::uint32_t
    {
      m_z = (std::max)(x, 0.1F); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      m_n = dn;
      m_p = static_cast<std::int32_t>(digits);

      // Get the starting order for recursion.
      const auto f_order = bisect(fn_mstart2, 0.1F, x + 100000.0F);
      const auto n_order = static_cast<std::uint32_t>(f_order);

      // Make sure the return value is an odd integer.
      return ((((n_order % 2U) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
    }
  };

  float         Jn_algo::m_z; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  float         Jn_algo::m_n; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  std::int32_t  Jn_algo::m_p; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  template<typename FloatingPointType>
  auto cyl_bessel_j(std::int32_t n, FloatingPointType x) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    const floating_point_type one_over_x = floating_point_type(1U) / x;

    floating_point_type my_jn_result(0U);

    // Start recursion using two J's, Jn+2 and Jn+1.
    // Arbitrarily set Jn+2 = 0 and Jn+1 = 1.
    floating_point_type jn_p2    (0U);
    floating_point_type jn_p1    (1U);
    floating_point_type norm_half(1U);

    const auto d10 = static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::digits10);

    // Get the candidates for the starting order(s) for recursive calculations.
    const auto n_start1 =
      static_cast<std::int32_t>
      (
        Jn_algo::mstart1(static_cast<float>(x), d10)
      );

    const auto n_start2 =
      static_cast<std::int32_t>
      (
        Jn_algo::mstart2(static_cast<float>(x), static_cast<float>(n - 1), d10)
      );

    const auto n_start = (std::max)(n_start2, n_start1);

    // Do the recursion. The direction of the recursion is downward.
    for(auto m = n_start; m >= static_cast<std::int32_t>(0); --m)
    {
      //                                 Jn+1(x)
      // Downward recursion is:  Jn(x) = ------- * [2 * (m + 1)] - Jn+2(x)
      //                                    x
      const floating_point_type jn = ((jn_p1 * one_over_x) * (2L * (m + 1L))) - jn_p2;

      jn_p2 = jn_p1;
      jn_p1 = jn;

      // For Normalization use a Neumann expansion of the form
      // 1 = J_0(x) + 2 * J_2(x) + 2 * J_4(x) + 2 * J_6(x) + ...

      if((m % 2) == 0)
      {
        norm_half += ((m == 0) ? jn / 2 : jn);
      }

      // Store the requested value of jn in the result.
      if(m == n)
      {
        my_jn_result = jn;
      }
    }

    // Divide by the normalization in order to get the scaled result.

    return my_jn_result / (norm_half * 2);
  }
} // namespace example004_bessel

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example004_bessel_recur() -> bool
#else
auto math::wide_decimal::example004_bessel_recur() -> bool
#endif
{
  // Calculate 1,001 decimal digits of the value of a cylindrical Bessel function.
  // N[BesselJ[9, 123456789/10000000], 1001]

  using dec1001_t = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(1001)), std::uint32_t, void>;

  // Downward recursion and simultaneous summation of
  // the normalization factor.
  const dec1001_t x(dec1001_t(123456789UL) / 10000000UL);

  const dec1001_t j9 = example004_bessel::cyl_bessel_j(9, x);

  const dec1001_t control
  {
    "0."
    "1811741425937681762466579147691209001166013424259623578868028285653121488673875310835208966563908818"
    "8421084510181588629377838992836875130419314676290262920135855070054667577369180115354001461345693462"
    "3182922129763217285631360243046549470307101764787462261457659309595240398566923511257271711608054330"
    "5056700529740700032867389718239950740636572946461190181951446862216631994941332735738268085023857232"
    "6582261877606006252052682721630912936214800203637504645062905404207401123744186197817168396663104119"
    "9050723136375135227537136776124605690390480900970872013439959523346613509186845811635745574795057856"
    "3807491356318608233670107033347145409667275608327874165772454484451645273190107230684397251693317786"
    "2531833854482410147115475646942614345129243525028533993270631293810294366156969745188156448674296887"
    "5638917593838992394513006147403026845798841322971959209492125959307304070719478331526845657028115231"
    "6922538034120880586019099361579383438884902342972624055145047004844365733067555661014592505246942704"
    "5"
  };

  const dec1001_t closeness = fabs(1 - (j9 / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec1001_t>::epsilon() * 100);

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

