///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

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

  template<typename FloatingPointType>
  FloatingPointType ln_two()
  {
    return FloatingPointType(0.69314718055994530941723212145817657L);
  }

  template<>
  wide_decimal_type ln_two()
  {
    return math::wide_decimal::ln_two<wide_decimal_digits10>();
  }
}

namespace local
{
  util::dynamic_array<wide_decimal_type> bernoulli_table((std::uint_fast32_t) ((float) std::numeric_limits<wide_decimal_type>::digits10 * 0.95F));

  template<typename FloatingPointType>
  FloatingPointType hypergeometric_0f0(const FloatingPointType& x)
  {
    using floating_point_type = FloatingPointType;

    using std::fabs;

    // Compute the series representation of Hypergeometric0F0 taken from
    // http://functions.wolfram.com/HypergeometricFunctions/Hypergeometric0F0/06/01/
    // There are no checks on input range or parameter boundaries.

    floating_point_type x_pow_n_div_n_fact(x);

    floating_point_type h0f0 = 1 + x_pow_n_div_n_fact;

    const floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

    // Series expansion of hypergeometric_0f0(; ; x).
    for(std::uint32_t n = 2U; n < UINT32_C(10000000); ++n)
    {
      x_pow_n_div_n_fact *= x;
      x_pow_n_div_n_fact /= n;

      if((n > 4U) && (fabs(x_pow_n_div_n_fact) < tol))
      {
        break;
      }

      h0f0 += x_pow_n_div_n_fact;
    }

    return h0f0;
  }

  template<typename FloatingPointType>
  FloatingPointType exp(const FloatingPointType& x)
  {
    using floating_point_type = FloatingPointType;

    using std::pow;

    // The algorithm for exp has been taken from MPFUN.
    // exp(t) = [ (1 + r + r^2/2! + r^3/3! + r^4/4! ...)^p2 ] * 2^n
    // where p2 is a power of 2 such as 2048, r = t_prime / p2, and
    // t_prime = t - n*ln2, with n chosen to minimize the absolute
    // value of t_prime. In the resulting Taylor series, which is
    // implemented as a hypergeometric function, |r| is bounded by
    // ln2 / p2.

    // Compute the exponential series of the (possibly) scaled argument.
    floating_point_type exp_series;

    // Compute 1 / ln2 as a warm-cached constant value.
    static const floating_point_type ln2 = ln_two<floating_point_type>();

    const std::int32_t nf = (std::int32_t) (x / ln2);

    // The scaling is 2^11 = 2048.
    const std::int32_t p2 = static_cast<std::int32_t>(std::uint32_t(1U) << 11);

    exp_series =   pow(local::hypergeometric_0f0((x - (nf * ln2)) / p2), p2)
                 * pow(floating_point_type(2U), nf);

    return exp_series;
  }

  template<typename FloatingPointType>
  void bernoulli_b(FloatingPointType* bn, const std::uint32_t n)
  {
    using floating_point_type = FloatingPointType;

    // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
    // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
    // Cambridge University Press, 2010, 237 pp.

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

        tangent_numbers[j] = (tangent_numbers[j - 1] * j_minus_k) + (tangent_numbers[j] * (j_minus_k + 2U));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(std::uint32_t i = 1U; i < static_cast<std::uint32_t>(n / 2U); ++i)
    {
      const std::uint32_t two_i = 2U * i;

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const bool  b_neg = ((two_i % 4U) == 0U);

      bn[two_i] = ((!b_neg) ? b : -b);

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
    const floating_point_type one_over_x2                    = one_over_x_pow_two_n_minus_one * one_over_x_pow_two_n_minus_one;
          floating_point_type sum                            = (bernoulli_table[2U] / 2U) * one_over_x_pow_two_n_minus_one;

    floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

    if(xx > 8U)
    {
      // In the following code sequence, we extract the approximate logarithm
      // of the argument x and use the leading term of Stirling's approximation,
      // which is Log[Gamma[x]] aprox. (x (Log[x] - 1)) in order to scale
      // the tolerance. In order to do this, we find the built-in floating point
      // approximation of (x (Log[x] - 1)).

      using std::ilogb;
      using std::log;
      using std::pow;

      // Extract lgx = Log[mantissa * radix^ib]
      //             = Log[mantissa] + ib * Log[radix]

      const std::int32_t ib       = (std::int32_t) ilogb(xx);
      const float        mantissa = (float) (xx / pow(floating_point_type(std::numeric_limits<floating_point_type>::radix), ib));

      const float lg_xx =   log(mantissa)
                          + ((float) ib * log((float) std::numeric_limits<floating_point_type>::radix));

      tol *= (xx * floating_point_type(lg_xx - 1.0F));
    }

    // Perform the Bernoulli series expansion.
    for(std::uint32_t n2 = 4U; n2 < (std::uint32_t) bernoulli_table.size(); n2 += 2U)
    {
      one_over_x_pow_two_n_minus_one *= one_over_x2;

      const floating_point_type term =
          (bernoulli_table[n2] * one_over_x_pow_two_n_minus_one)
        / (std::uint64_t) (n2 * (std::uint64_t) (n2 - 1U));

      if((n2 > 4U) && (fabs(term) < tol))
      {
        break;
      }

      sum += term;
    }

    using ::pi;
    using local::exp;
    using std::exp;
    using std::log;

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
  local::bernoulli_b(local::bernoulli_table.data(), (std::uint32_t) local::bernoulli_table.size());

  // Set x to 23/2, where the intent is to calculate
  // Gamma[n + 1/2] with n=11.

  const wide_decimal_type x = wide_decimal_type(23U) / 2U;

  const wide_decimal_type g = local::tgamma(x);

  // Consider the control value.
  //                                    (2n)!
  // Use the relation Gamma[1/2 + n] = -------- * Sqrt[Pi].
  //                                   (4^n) n!
  //
  //                  13749310575
  // This results in ------------ * Sqrt[Pi] for n=11.
  //                     2048

  using ::pi;

  const wide_decimal_type control = (sqrt(pi<wide_decimal_type>()) * UINT64_C(13749310575)) / 2048U;

  const wide_decimal_type closeness = fabs(1 - (g / control));

  const bool result_is_ok = (closeness < (std::numeric_limits<wide_decimal_type>::epsilon() * UINT32_C(100000)));

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
