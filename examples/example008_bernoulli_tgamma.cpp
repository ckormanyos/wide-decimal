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
  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(2001);

  using dec2001_t = math::wide_decimal::decwide_t<wide_decimal_digits10>;

  template<typename FloatingPointType>
  FloatingPointType pi()
  {
    return FloatingPointType(3.1415926535897932384626433832795029L);
  }

  template<>
  dec2001_t pi()
  {
    return math::wide_decimal::pi<wide_decimal_digits10>();
  }

  template<typename FloatingPointType>
  FloatingPointType ln_two()
  {
    return FloatingPointType(0.69314718055994530941723212145817657L);
  }

  template<>
  dec2001_t ln_two()
  {
    return math::wide_decimal::ln_two<wide_decimal_digits10>();
  }
}

namespace local
{
  std::vector<dec2001_t> bernoulli_table((std::uint_fast32_t) (std::numeric_limits<dec2001_t>::digits10 * 2));

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

    std::vector<floating_point_type> tangent_numbers(m + 1U);

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
    static const std::int32_t        min_arg_n = (std::int32_t) ((float) std::numeric_limits<floating_point_type>::digits10 * 1.2F);
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

    const floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

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

    using std::exp;
    using std::log;
    using local::exp;

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

  const dec2001_t x = dec2001_t(UINT32_C(3456789)) / UINT32_C(10000000);

  const dec2001_t g = local::tgamma(x);

  const dec2001_t control
  {
    "2."
    "5792700634274853567721602504331378637312507250684265769670260445405759391178961482604996184815668756"
    "8564835035250596999345597383446265698132918512614930473325677638107714332958497710269501613725680170"
    "1207048419926349028508375947826766602670824973689466309567097968544545120863295426820157846797078515"
    "3270391593695595325553888998094781117812031903577065059426744317478442627293643562162041133861245545"
    "5622513499502887983759697964339712827017626583067849940594815336039108197564304048170285864914193574"
    "6539534364270304865358361035034470710717568758018266952541409607892517276719473986066043431889561930"
    "8498759039273502823878797301370230697817002245345723860335617187810904209611008643313142837894123581"
    "0204380574243088642193313463490316847304828800342739459922573993969796137360290127111519031294685185"
    "9575424942209577154958875121359025277213900704605966325137247902172904181666603495550337045837078098"
    "1329255339338726902394775356991039595635478839349494883276914599356815269202887419270730947712224664"
    "7888442059790271553237358069313114134629535464499707521391186165968003771536204017063643231157170766"
    "8291701165834588803112562500090912977321345304404117143667755019219481548316844274771883223505023185"
    "3631032476704714230052339785840973254493785151096015134568292497070208911510784135111072326062599629"
    "0103592503677463193939775497227282066710574467640321248364765696246509069511492844211634243810535836"
    "5311972991569881526779378074785470211815077380872663865343267793938165382770375550089387910008444300"
    "7163511872811923369230296749404458568568082061722141777675316484557243668607391204807898599085869169"
    "2386504735470941454860546852532571178734682694320676502312195342261737739866443753806397564819099859"
    "9331942989031725874448974291474222201619146464227954923392563442264787739545126615102594999536948124"
    "9427322986728395061509242760072540343037058966311125074550704404179652896588450427799200629988929136"
    "7684199362266522666617830565032059371026662203130872993471499160234189035693750089708150749100353631"
    "8"
  };

  const dec2001_t closeness = fabs(1 - (g / control));

  const bool result_is_ok = (closeness < (std::numeric_limits<dec2001_t>::epsilon() * UINT32_C(1000000)));

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
