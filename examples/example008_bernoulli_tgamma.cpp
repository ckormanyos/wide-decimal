///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#if (defined(__GNUC__) && !defined(__clang__) && defined(__arm__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

#include <array>
#include <cstdint>
#include <utility>

#if (defined(__GNUC__) && defined(__arm__))
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#endif
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS
#define WIDE_DECIMAL_DISABLE_USE_STD_FUNCTION

#if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
#include <chrono>
#endif

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>
#include <util/utility/util_dynamic_array.h>

namespace example008_bernoulli
{
  #if (defined(__GNUC__) && defined(__arm__))
  constexpr std::int32_t wide_decimal_digits10 = INT32_C(101);
  #else
  constexpr std::int32_t wide_decimal_digits10 = INT32_C(1001);
  #endif

  using wide_decimal_limb_type = std::uint32_t;

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using wide_decimal_digits_helper_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, wide_decimal_limb_type>;
  #else
  using wide_decimal_digits_helper_type = ::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, wide_decimal_limb_type>;
  #endif

  // For n decimal digits, the following slot counts are needed.
  // Decimal Digits  Slots needed
  //     1001            1536
  //      251             512
  //      101             256

  using wide_decimal_allocator_type =
    util::n_slot_array_allocator<wide_decimal_limb_type,
                                 wide_decimal_digits_helper_type::elem_number,
                                 #if (defined(__GNUC__) && defined(__arm__))
                                 static_cast<std::size_t>(UINT32_C(256)
                                 #else
                                 static_cast<std::size_t>(UINT32_C(1536)
                                 #endif
                                 )>;

  #if defined(__cpp_constexpr_dynamic_alloc)
  #if (defined(__GNUC__) && defined(__arm__))
  static_assert(std::allocator_traits<wide_decimal_allocator_type>::max_size(wide_decimal_allocator_type()) > static_cast<std::size_t>(UINT32_C(200)),
  #else
  static_assert(std::allocator_traits<wide_decimal_allocator_type>::max_size(wide_decimal_allocator_type()) > static_cast<std::size_t>(UINT32_C(1500)),
  #endif
                "Error: Not enough slots available for tgamma calculation");
  #endif

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using wide_decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10, wide_decimal_limb_type, wide_decimal_allocator_type>;
  #else
  using wide_decimal_type = ::math::wide_decimal::decwide_t<wide_decimal_digits10, wide_decimal_limb_type, wide_decimal_allocator_type>;
  #endif

  template<typename FloatingPointType>
  auto pi() -> FloatingPointType
  {
    return static_cast<FloatingPointType>(3.1415926535897932384626433832795029L); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  template<>
  auto pi() -> wide_decimal_type // LCOV_EXCL_LINE
  {
    #if defined(WIDE_DECIMAL_NAMESPACE)
    return WIDE_DECIMAL_NAMESPACE::math::wide_decimal::pi<wide_decimal_digits10, wide_decimal_limb_type, wide_decimal_allocator_type>();
    #else
    return ::math::wide_decimal::pi<wide_decimal_digits10, wide_decimal_limb_type, wide_decimal_allocator_type>();
    #endif
  }

  auto bernoulli_table() -> util::dynamic_array<wide_decimal_type>&
  {
    using bernoulli_table_array_type = util::dynamic_array<wide_decimal_type>;
    using local_size_type            = typename bernoulli_table_array_type::size_type;

    constexpr auto bernoulli_table_size =
      static_cast<local_size_type>
      (
        static_cast<float>
        (
          static_cast<long double>(std::numeric_limits<wide_decimal_type>::digits10) * 0.95L
        )
      );

    static bernoulli_table_array_type bernoulli_table(bernoulli_table_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,cppcoreguidelines-avoid-non-const-global-variables,readability-magic-numbers,cert-err58-cpp)

    return bernoulli_table;
  }

  template<typename FloatingPointType>
  auto bernoulli_b(FloatingPointType* bn, std::uint32_t n) -> void
  {
    using floating_point_type = FloatingPointType;

    // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
    // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
    // Cambridge University Press, 2010, p. 237.

    using tangent_numbers_array_type = util::dynamic_array<floating_point_type>;
    using local_size_type = typename tangent_numbers_array_type::size_type;

    const auto m          = static_cast<local_size_type>(static_cast<local_size_type>(n) / static_cast<local_size_type>(UINT8_C(2)));
    const auto m_plus_one = static_cast<local_size_type>(m + static_cast<local_size_type>(UINT8_C(1)));

    util::dynamic_array<floating_point_type> tangent_numbers(m_plus_one);

    tangent_numbers[static_cast<local_size_type>(UINT8_C(0))] = 0U;
    tangent_numbers[static_cast<local_size_type>(UINT8_C(1))] = 1U;

    for(auto k = static_cast<local_size_type>(UINT8_C(1)); k < m; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      const auto k_plus_one = static_cast<local_size_type>(k + static_cast<local_size_type>(UINT8_C(1)));

      tangent_numbers[k_plus_one] = tangent_numbers[k] * k;
    }

    for(auto k = static_cast<local_size_type>(UINT8_C(2)); k <= m; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      for(auto j = k; j <= m; ++j) // NOLINT(altera-id-dependent-backward-branch)
      {
        const auto j_minus_k = static_cast<local_size_type>(j - k);

        tangent_numbers[j] =   (tangent_numbers[j - 1] *  j_minus_k)
                             + (tangent_numbers[j]     * (j_minus_k + 2U));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(auto   i = static_cast<local_size_type>(UINT8_C(1));
               i < static_cast<local_size_type>(n / static_cast<local_size_type>(UINT8_C(2)));
             ++i)
    {
      const auto two_i = static_cast<local_size_type>(static_cast<local_size_type>(UINT8_C(2)) * i);

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const auto b_neg =
        (static_cast<local_size_type>(two_i % static_cast<local_size_type>(UINT8_C(4))) == static_cast<local_size_type>(UINT8_C(0)));

      bn[two_i] = ((!b_neg) ? b : -b); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

      two_pow_two_m *= 4U;
    }

    bn[0U] =  1U;                          // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    bn[1U] = floating_point_type(-1) / 2U; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }

  template<typename FloatingPointType>
  auto tgamma(const FloatingPointType& x) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    // Check if the argument should be scaled up for the Bernoulli series expansion.
    static const auto min_arg_n =
      static_cast<std::int32_t>
      (
        static_cast<float>(static_cast<float>(std::numeric_limits<floating_point_type>::digits10) * 0.8F)
      );

    static const floating_point_type min_arg_x = floating_point_type(min_arg_n);

    const auto n_recur =
      static_cast<std::uint32_t>
      (
        (x < min_arg_x) ? static_cast<std::uint32_t>(static_cast<std::uint32_t>(min_arg_n - static_cast<std::int32_t>(x)) + 1U)
                        : static_cast<std::uint32_t>(0U)
      );

    floating_point_type xx(x);

    // Scale the argument up and use downward recursion later for the final result.
    if(n_recur != static_cast<std::uint32_t>(UINT8_C(0)))
    {
      xx += n_recur;
    }

          floating_point_type one_over_x_pow_two_n_minus_one = 1 / xx;
    const floating_point_type one_over_x2                    =  one_over_x_pow_two_n_minus_one * one_over_x_pow_two_n_minus_one;
          floating_point_type sum                            = (one_over_x_pow_two_n_minus_one * bernoulli_table()[2U]) / 2U;

    floating_point_type tol = std::numeric_limits<floating_point_type>::epsilon();

    using std::log;

    if(xx > 8U) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {
      // In the following code sequence, we extract the approximate logarithm
      // of the argument x and use the leading term of Stirling's approximation,
      // which is Log[Gamma[x]] aprox. (x (Log[x] - 1)) in order to scale
      // the tolerance. In order to do this, we find the built-in floating point
      // approximation of (x (Log[x] - 1)).

      // Limit fx to the range 8 <= fx <= 10^16, where 8 is chosen to
      // ensure that (log(fx) - 1.0F) remains positive and 10^16 is
      // selected arbitrarily, yet ensured to be rather large.
      auto fx_max = (std::max)(static_cast<floating_point_type>(8U), xx); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      auto fx = (std::min)(fx_max, static_cast<floating_point_type>(UINT64_C(10000000000000000)));

      tol *= static_cast<float>(fx * (log(fx) - 1.0F));
    }

    // Perform the Bernoulli series expansion.
    for(auto n2 = static_cast<std::uint32_t>(4U); n2 < static_cast<std::uint32_t>(bernoulli_table().size()); n2 += 2U)
    {
      one_over_x_pow_two_n_minus_one *= one_over_x2;

      const floating_point_type term =
          (one_over_x_pow_two_n_minus_one * bernoulli_table()[n2])
        / static_cast<std::uint64_t>(static_cast<std::uint64_t>(n2) * static_cast<std::uint32_t>(n2 - 1U));

      using std::fabs;

      if((n2 > 6U) && (fabs(term) < tol)) // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      {
        break;
      }

      sum += term;
    }

    using example008_bernoulli::pi;
    using std::exp;

    static const floating_point_type half           = floating_point_type(1U) / 2U;
    static const floating_point_type half_ln_two_pi = log(pi<floating_point_type>() * 2U) / 2U;

    floating_point_type g = exp(((((xx - half) * log(xx)) - xx) + half_ln_two_pi) + sum);

    // Rescale the result using downward recursion if necessary.
    for(auto k = static_cast<std::uint32_t>(0U); k < n_recur; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      g /= --xx;
    }

    return g;
  }

  auto example008_bernoulli_tgamma_run() -> bool
  {
    // Initialize the table of Bernoulli numbers.
    bernoulli_b
    (
      bernoulli_table().data(),
      static_cast<std::uint32_t>(bernoulli_table().size())
    );

    // In this example, we compute values of Gamma[1/2 + n].

    // We will make use of the relation
    //                     (2n)!
    //   Gamma[1/2 + n] = -------- * Sqrt[Pi].
    //                    (4^n) n!

    // Table[Factorial[2 n]/((4^n) Factorial[n]), {n, 0, 17, 1}]
    constexpr std::array<std::pair<std::uint64_t, std::uint32_t>, 18U> ratios =
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

    const wide_decimal_type tol (std::numeric_limits<wide_decimal_type>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000)));
    const wide_decimal_type half(0.5F);

    for(auto i = static_cast<std::size_t>(0U); i < ratios.size(); ++i)
    {
      // Calculate Gamma[1/2 + i]

      using example008_bernoulli::tgamma;
      using std::tgamma;

      const wide_decimal_type g = tgamma(half + i);

      // Calculate the control value.

      using example008_bernoulli::pi;
      using std::fabs;
      using std::sqrt;

      const wide_decimal_type control = (sqrt(pi<wide_decimal_type>()) * ratios[i].first) / ratios[i].second; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

      const wide_decimal_type closeness = fabs(1 - (g / control));

      result_is_ok &= (closeness < tol);
    }

    return result_is_ok;
  }
} // namespace example008_bernoulli

#if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
#include <iomanip>
#include <iostream>
#endif

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example008_bernoulli_tgamma() -> bool
#else
auto ::math::wide_decimal::example008_bernoulli_tgamma() -> bool
#endif
{
  #if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
  using local_clock_type = std::chrono::high_resolution_clock;

  const auto start = local_clock_type::now();
  #endif

  const auto result_is_ok = example008_bernoulli::example008_bernoulli_tgamma_run();

  #if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
  const auto stop = local_clock_type::now();
  #endif

  #if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
  const auto elapsed =
    static_cast<float>
    (
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())
      / static_cast<float>(1000.0F)
    );

  std::cout << "Time example008_bernoulli_tgamma()  : "
            << elapsed
            << std::endl;
  #endif

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE008_BERNOULLI_TGAMMA)

constexpr auto app_benchmark_standalone_foodcafe = static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE));

extern "C"
{
  extern volatile std::uint32_t app_benchmark_standalone_result; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  auto app_benchmark_run_standalone       (void) -> bool;
  auto app_benchmark_get_standalone_result(void) -> bool;

  auto app_benchmark_run_standalone(void) -> bool
  {
    const auto result_is_ok = ::math::wide_decimal::example008_bernoulli_tgamma();

    app_benchmark_standalone_result =
      static_cast<std::uint32_t>
      (
        result_is_ok ? app_benchmark_standalone_foodcafe : static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF))
      );

    return result_is_ok;
  }

  auto app_benchmark_get_standalone_result(void) -> bool
  {
    volatile auto result_is_ok =
      (app_benchmark_standalone_result == static_cast<std::uint32_t>(UINT32_C(0xF00DCAFE)));

    return result_is_ok;
  }
}

auto main() -> int
{
  auto result_is_ok = true;

  result_is_ok = (::app_benchmark_run_standalone       () && result_is_ok);
  result_is_ok = (::app_benchmark_get_standalone_result() && result_is_ok);

  #if !defined(WIDE_DECIMAL_DISABLE_IOSTREAM)
  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
  #endif

  return (result_is_ok ? 0 : -1);
}

extern "C"
{
  volatile std::uint32_t app_benchmark_standalone_result; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
}

#endif

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif

#if (defined(__GNUC__) && !defined(__clang__) && defined(__arm__))
#pragma GCC diagnostic pop
#endif
