///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <vector>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  using dec1001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;

  template<typename FloatingPointType>
  FloatingPointType pi()
  {
    return FloatingPointType(3.1415926535897932384626433832795029L);
  }

  template<>
  dec1001_t pi()
  {
    return math::wide_decimal::pi<wide_decimal_digits10>();
  }

  template<typename FloatingPointType>
  FloatingPointType ln_two()
  {
    return FloatingPointType(0.69314718055994530941723212145817657L);
  }

  template<>
  dec1001_t ln_two()
  {
    return math::wide_decimal::ln_two<wide_decimal_digits10>();
  }
}

namespace local
{
  std::vector<dec1001_t> bernoulli_table((std::uint_fast32_t) (std::numeric_limits<dec1001_t>::digits10 * 2));

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
    // ln2 / p2. For small arguments, no scaling is done.

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

    const std::int32_t m = n / 2;

    std::vector<floating_point_type> tangent_numbers((std::uint_fast32_t) (m + 1));

    tangent_numbers[0U] = 0U;
    tangent_numbers[1U] = 1U;

    for(std::int32_t k = 2; k <= m; ++k)
    {
      tangent_numbers[k] = (k - 1) * tangent_numbers[k - 1];
    }

    for(std::int32_t k = 2; k <= m; ++k)
    {
      for(std::int32_t j = k; j <= m; ++j)
      {
        tangent_numbers[j] = (tangent_numbers[j - 1] * (j - k)) + (tangent_numbers[j] * (j - k + 2));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(std::int32_t i = 1; i < static_cast<std::int32_t>(n / 2U); ++i)
    {
      const std::int32_t two_i = static_cast<std::int32_t>(static_cast<std::int32_t>(2) * i);

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const bool  b_neg = (static_cast<std::int32_t>(two_i % static_cast<std::int32_t>(4)) == static_cast<std::int32_t>(0));

      bn[2 * i] = ((!b_neg) ? b : -b);

      two_pow_two_m *= 4;
    }

    bn[0U] =  1U;
    bn[1U] = floating_point_type(-1) / 2U;
  }

  template<typename FloatingPointType>
  FloatingPointType tgamma(const FloatingPointType& x)
  {
    using floating_point_type = FloatingPointType;

    // Check if the argument should be scaled up for the Bernoulli series expansion.
    static const std::int32_t        min_arg_n = (std::int32_t) ((float) std::numeric_limits<floating_point_type>::digits10 * 1.2F);
    static const floating_point_type min_arg_x = floating_point_type(min_arg_n);

    const std::int32_t n_recur = ((x < min_arg_x) ? ((std::int32_t) (min_arg_n - (std::int32_t) x) + 1)
                                                  :  (std::int32_t) 0);

    floating_point_type xx(x);

    // Scale the argument up and use downward recursion later for the final result.
    if(n_recur != 0)
    {
      xx += n_recur;
    }

          floating_point_type one_over_x_pow_two_n_minus_one = 1 / xx;
    const floating_point_type one_over_x2                    = one_over_x_pow_two_n_minus_one * one_over_x_pow_two_n_minus_one;
          floating_point_type sum                            = (bernoulli_table[2U] / 2U) * one_over_x_pow_two_n_minus_one;

    const floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

    // Perform the Bernoulli series expansion.
    for(std::int32_t n2 = 4; n2 < (std::int32_t) bernoulli_table.size(); n2 += 2)
    {
      one_over_x_pow_two_n_minus_one *= one_over_x2;

      const floating_point_type term =
          (bernoulli_table[(std::uint32_t) n2] * one_over_x_pow_two_n_minus_one)
        / (std::int64_t) (n2 * (std::int64_t) (n2 - 1));

      if((n2 > 4) && (fabs(term) < tol))
      {
        break;
      }

      sum += term;
    }

    using std::exp;
    using std::log;
    using local::exp;

    static const floating_point_type half           = floating_point_type(1U) / 2U;
    static const floating_point_type half_ln_two_pi = log(pi<floating_point_type>() * 2U) / 2U;

    floating_point_type g = exp(((((xx - half) * log(xx)) - xx) + half_ln_two_pi) + sum);

    // Rescale the result using downward recursion if necessary.
    for(std::int32_t k = 0; k < n_recur; ++k)
    {
      g /= --xx;
    }

    return g;
  }
}

bool math::wide_decimal::example008_bernoulli_tgamma()
{
  local::bernoulli_b(local::bernoulli_table.data(), (std::uint32_t) local::bernoulli_table.size());

  const dec1001_t g = local::tgamma(dec1001_t(1U) / 3U);

  const dec1001_t control
  {
    "2."
    "6789385347077476336556929409746776441286893779573011009504283275904176101677438195409828890411887894"
    "1915904920007226333571908456950447225997771336770846976816728982305000321834255032224715694181755544"
    "9952728784394779441305765828401612319141596466526033727584020580635513943241032015839415382700855240"
    "5210323387989550693638926386839167072816915423096273318811864774965222910556444090780096341646353274"
    "0195630152839860021250692996870571006864544754497067121216435474129286262442530553451675106636988879"
    "5702949499999822904875405769873494540359448646618127601125501081007135774850918287888928269081113039"
    "6508300618399030183890284867938695239815650034312097356725343620921104097360814928866903299993919976"
    "3507245847443710428535667893467156890589821447841213999549940936167692556916329004659357582947195283"
    "6892596342706528426029714811514667312340900520399929293380822039330227609582331037109976015726326523"
    "2911178632250866419355137129992106494762157882417423665957383788601329658550316559885453066009697291"
    "0"
  };

  const dec1001_t closeness = fabs(1 - (g / control));

  const bool result_is_ok = (closeness < (std::numeric_limits<dec1001_t>::epsilon() * UINT32_C(1000000)));

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
