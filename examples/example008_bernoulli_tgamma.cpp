///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <array>
#include <cstdint>
#include <ctime>
#include <utility>

#include <math/wide_decimal/decwide_t.h>
#include <util/utility/util_dynamic_array.h>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using wide_decimal_type = math::wide_decimal::decwide_t<wide_decimal_digits10>;

  template<typename FloatingPointType>
  FloatingPointType pi()
  {
    return FloatingPointType(3.1415926535897932384626433832795029L);
  }

  template<>
  wide_decimal_type pi()
  {
    return math::wide_decimal::pi<wide_decimal_digits10>();
  }
}

namespace local
{
  util::dynamic_array<wide_decimal_type> bernoulli_table((std::uint_fast32_t) ((float) std::numeric_limits<wide_decimal_type>::digits10 * 0.95F));

  template<typename FloatingPointType>
  void bernoulli_b(FloatingPointType* bn, const std::uint32_t n)
  {
    using floating_point_type = FloatingPointType;

    // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
    // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
    // Cambridge University Press, 2010, p. 237.

    const std::uint32_t m = n / 2U;

    util::dynamic_array<floating_point_type> tangent_numbers(m + 1U);

    tangent_numbers[0U] = 0U;
    tangent_numbers[1U] = 1U;

    for(std::uint32_t k = 1U; k < m; ++k)
    {
      tangent_numbers[k + 1U] = tangent_numbers[k] * k;
    }

    for(std::uint32_t k = 2U; k <= m; ++k)
    {
      for(std::uint32_t j = k; j <= m; ++j)
      {
        const std::uint32_t j_minus_k = j - k;

        tangent_numbers[j] =   (tangent_numbers[j - 1] *  j_minus_k)
                             + (tangent_numbers[j]     * (j_minus_k + 2U));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(std::uint32_t i = 1U; i < static_cast<std::uint32_t>(n / 2U); ++i)
    {
      const std::uint32_t two_i = 2U * i;

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const bool  b_neg = ((two_i % 4U) == 0U);

      bn[two_i] = ((b_neg == false) ? b : -b);

      two_pow_two_m *= 4U;
    }

    bn[0U] =  1U;
    bn[1U] = floating_point_type(-1) / 2U;
  }

  template<typename FloatingPointType>
  FloatingPointType tgamma(const FloatingPointType& x)
  {
    using floating_point_type = FloatingPointType;

    // Check if the argument should be scaled up for the Bernoulli series expansion.
    static const std::int32_t        min_arg_n = (std::int32_t) ((float) std::numeric_limits<floating_point_type>::digits10 * 0.8F);
    static const floating_point_type min_arg_x = floating_point_type(min_arg_n);

    const std::uint32_t n_recur = ((x < min_arg_x) ? ((std::uint32_t) (min_arg_n - (std::int32_t) x) + 1U)
                                                  :   (std::uint32_t) 0U);

    floating_point_type xx(x);

    // Scale the argument up and use downward recursion later for the final result.
    if(n_recur != 0U)
    {
      xx += n_recur;
    }

          floating_point_type one_over_x_pow_two_n_minus_one = 1 / xx;
    const floating_point_type one_over_x2                    =  one_over_x_pow_two_n_minus_one * one_over_x_pow_two_n_minus_one;
          floating_point_type sum                            = (one_over_x_pow_two_n_minus_one * bernoulli_table[2U]) / 2U;

    floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

    using std::log;

    if(xx > 8U)
    {
      // In the following code sequence, we extract the approximate logarithm
      // of the argument x and use the leading term of Stirling's approximation,
      // which is Log[Gamma[x]] aprox. (x (Log[x] - 1)) in order to scale
      // the tolerance. In order to do this, we find the built-in floating point
      // approximation of (x (Log[x] - 1)).

      // Limit fx to the range 8 <= fx <= 10^16, where 8 is chosen to
      // ensure that (log(fx) - 1.0F) remains positive and 10^16 is
      // selected arbitrarily, yet ensured to be rather large.
      const float fx = (float) (std::min)((std::max)(floating_point_type(8U), xx),
                                          floating_point_type(UINT64_C(10000000000000000)));

      tol *= (float) (fx * (log(fx) - 1.0F));
    }

    // Perform the Bernoulli series expansion.
    for(std::uint32_t n2 = 4U; n2 < (std::uint32_t) bernoulli_table.size(); n2 += 2U)
    {
      one_over_x_pow_two_n_minus_one *= one_over_x2;

      const floating_point_type term =
          (one_over_x_pow_two_n_minus_one * bernoulli_table[n2])
        / (std::uint64_t) ((std::uint64_t) n2 * (n2 - 1U));

      using std::fabs;

      if((n2 > 6U) && (fabs(term) < tol))
      {
        break;
      }

      sum += term;
    }

    using ::pi;
    using std::exp;

    static const floating_point_type half           = floating_point_type(1U) / 2U;
    static const floating_point_type half_ln_two_pi = log(pi<floating_point_type>() * 2U) / 2U;

    floating_point_type g = exp(((((xx - half) * log(xx)) - xx) + half_ln_two_pi) + sum);

    // Rescale the result using downward recursion if necessary.
    for(std::uint32_t k = 0U; k < n_recur; ++k)
    {
      g /= --xx;
    }

    return g;
  }
}

bool math::wide_decimal::example008_bernoulli_tgamma()
{
  const std::clock_t start = std::clock();

  // Initialize the table of Bernoulli numbers.
  local::bernoulli_b(local::bernoulli_table.data(), (std::uint32_t) local::bernoulli_table.size());

  // In this example, we compute values of Gamma[1/2 + n].

  // We will make use of the relation
  //                     (2n)!
  //   Gamma[1/2 + n] = -------- * Sqrt[Pi].
  //                    (4^n) n!

  // Table[Factorial[2 n]/((4^n) Factorial[n]), {n, 0, 17, 1}]
  const std::array<std::pair<std::uint64_t, std::uint32_t>, 18U> ratios =
  {{
    { UINT64_C(                  1), UINT32_C(     1) },
    { UINT64_C(                  1), UINT32_C(     2) },
    { UINT64_C(                  3), UINT32_C(     4) },
    { UINT64_C(                 15), UINT32_C(     8) },
    { UINT64_C(                105), UINT32_C(    16) },
    { UINT64_C(                945), UINT32_C(    32) },
    { UINT64_C(              10395), UINT32_C(    64) },
    { UINT64_C(             135135), UINT32_C(   128) },
    { UINT64_C(            2027025), UINT32_C(   256) },
    { UINT64_C(           34459425), UINT32_C(   512) },
    { UINT64_C(          654729075), UINT32_C(  1024) },
    { UINT64_C(        13749310575), UINT32_C(  2048) },
    { UINT64_C(       316234143225), UINT32_C(  4096) },
    { UINT64_C(      7905853580625), UINT32_C(  8192) },
    { UINT64_C(    213458046676875), UINT32_C( 16384) },
    { UINT64_C(   6190283353629375), UINT32_C( 32768) },
    { UINT64_C( 191898783962510625), UINT32_C( 65536) },
    { UINT64_C(6332659870762850625), UINT32_C(131072) }
  }};

  bool result_is_ok = true;

  const wide_decimal_type tol (std::numeric_limits<wide_decimal_type>::epsilon() * UINT32_C(100000));
  const wide_decimal_type half(0.5F);

  for(auto i = 0U; i < ratios.size(); ++i)
  {
    // Calculate Gamma[1/2 + i]

    const wide_decimal_type g = local::tgamma(half + i);

    // Calculate the control value.

    using ::pi;
    using std::fabs;
    using std::sqrt;

    const wide_decimal_type control = (sqrt(pi<wide_decimal_type>()) * ratios[i].first) / ratios[i].second;

    const wide_decimal_type closeness = fabs(1 - (g / control));

    result_is_ok &= (closeness < tol);
  }

  const std::clock_t stop = std::clock();

  std::cout << "Time example008_bernoulli_tgamma(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example008_bernoulli_tgamma();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
