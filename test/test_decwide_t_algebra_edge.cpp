﻿///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2025.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif

#include <algorithm>
#include <cstdint>
#include <random>
#include <sstream>
#include <string>

#include <math/wide_decimal/decwide_t.h>
#include <test/test_decwide_t_algebra.h>
#include <util/utility/util_baselexical_cast.h>
#include <util/utility/util_pseudorandom_time_point_seed.h>

#if defined(__clang__)
  #if defined __has_feature && (__has_feature(thread_sanitizer) || __has_feature(address_sanitizer))
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#elif defined(__GNUC__)
  #if defined(__SANITIZE_THREAD__) || defined(__SANITIZE_ADDRESS__) || defined(WIDE_DECIMAL_HAS_COVERAGE)
  #define DECWIDE_T_REDUCE_TEST_DEPTH
  #endif
#endif

namespace test_decwide_t_algebra_edge {

using local_limb_type = std::uint16_t;

constexpr std::int32_t local_wide_decimal_digits10 = INT32_C(51);

#if defined(WIDE_DECIMAL_NAMESPACE)
using local_wide_decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<local_wide_decimal_digits10, local_limb_type, std::allocator<void>>;
#else
using local_wide_decimal_type = ::math::wide_decimal::decwide_t<local_wide_decimal_digits10, local_limb_type, std::allocator<void>>;
#endif

std::uniform_int_distribution<std::uint32_t> dist_sgn(UINT32_C(   0), UINT32_C(    1)); // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
std::uniform_int_distribution<std::uint32_t> dist_dig(UINT32_C(0x31), UINT32_C( 0x39)); // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)

using eng_sgn_type = std::ranlux24;
using eng_dig_type = std::minstd_rand0;
using eng_exp_type = std::mt19937;

eng_sgn_type eng_sgn; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
eng_dig_type eng_dig; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
eng_dig_type eng_exp; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)

auto local_zero    () -> const local_wide_decimal_type&;
auto local_one     () -> const local_wide_decimal_type&;
auto local_near_one() -> const local_wide_decimal_type&;
auto local_not_one () -> const local_wide_decimal_type&;

template<typename FloatingPointTypeWithStringConstruction>
auto generate_wide_decimal_value(bool is_positive     = false,
                                 int  exp_range       = 10000, // NOLINT(bugprone-easily-swappable-parameters)
                                 int  digits10_to_get = std::numeric_limits<FloatingPointTypeWithStringConstruction>::digits10 - 2) -> FloatingPointTypeWithStringConstruction
{
  using local_floating_point_type = FloatingPointTypeWithStringConstruction;

  static_assert(std::numeric_limits<local_floating_point_type>::digits10 > static_cast<int>(INT8_C(9)),
                "Error: Floating-point type destination does not have enough digits10");

  std::string str_x(static_cast<std::size_t>(digits10_to_get), '0');

  std::generate(str_x.begin(),
                str_x.end(),
                []() // NOLINT(modernize-use-trailing-return-type,-warnings-as-errors)
                {
                  return static_cast<char>(dist_dig(eng_dig));
                });

  if(exp_range != 0)
  {
    std::uniform_int_distribution<std::uint32_t>
      dist_exp
      (
        static_cast<std::uint32_t>(UINT8_C(0)),
        static_cast<std::uint32_t>(exp_range)
      );

    const auto val_exp = dist_exp(eng_exp);

    const auto sgn_exp = (dist_sgn(eng_sgn) != static_cast<std::uint32_t>(UINT8_C(0)));

    char p_str_exp[static_cast<std::size_t>(UINT8_C(32))] = { '\0' }; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    p_str_exp[static_cast<std::size_t>(UINT8_C(0))] = 'E';
    p_str_exp[static_cast<std::size_t>(UINT8_C(1))] = static_cast<char>(sgn_exp ? '-' : '+');

    {
      const char* p_end { util::baselexical_cast(val_exp, &p_str_exp[2U], &p_str_exp[0U] + sizeof(p_str_exp)) }; // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg,cppcoreguidelines-pro-bounds-pointer-arithmetic)

      for(const char* ptr { p_str_exp }; ptr != p_end; ++ptr) // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay,cppcoreguidelines-pro-bounds-pointer-arithmetic,llvm-qualified-auto,readability-qualified-auto,altera-id-dependent-backward-branch)
      {
        const auto len = str_x.length();

        str_x.insert(len, static_cast<std::size_t>(UINT8_C(1)), *ptr);
      }
    }

    // Insert a decimal point.
    str_x.insert(static_cast<std::size_t>(UINT8_C(1)), static_cast<std::size_t>(UINT8_C(1)), '.');
  }

  // Insert either a positive sign or a negative sign
  // (always one or the other) depending on the sign of x.
  const auto sign_char_to_insert =
    static_cast<char>
    (
      is_positive
        ? '+'
        : static_cast<char>((dist_sgn(eng_sgn) != static_cast<std::uint32_t>(UINT8_C(0))) ? '+' : '-')
    );

  str_x.insert(static_cast<std::size_t>(UINT8_C(0)), static_cast<std::size_t>(UINT8_C(1)), sign_char_to_insert);

  return local_floating_point_type(str_x.c_str());
}

auto test_mul_by_one_or_one_minus() -> bool
{
  auto result_is_ok = true;

  const auto one_minus = local_wide_decimal_type(local_one()).negate();

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto left = generate_wide_decimal_value<local_wide_decimal_type>();

    const auto result_plus  = static_cast<local_wide_decimal_type>(left * local_one());
    const auto result_minus = static_cast<local_wide_decimal_type>(left * one_minus);

    const auto mul_with_plus_minus_one_is_ok = ((result_plus == left) && (-result_minus == left));

    result_is_ok = (mul_with_plus_minus_one_is_ok && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto right = generate_wide_decimal_value<local_wide_decimal_type>();

    const auto result_plus  = static_cast<local_wide_decimal_type>(local_one() * right);
    const auto result_minus = static_cast<local_wide_decimal_type>(one_minus   * right);

    const auto mul_plus_minus_one_with_other_is_ok = ((result_plus == right) && (-result_minus == right));

    result_is_ok = (mul_plus_minus_one_with_other_is_ok && result_is_ok);
  }

  result_is_ok =
  (
       (   ((static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))) * static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))) == static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))) * static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1)))) == static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))) * static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))) == static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))) * static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1)))) == static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))))
    && result_is_ok
  );

  return result_is_ok;
}

auto test_div_by_one_or_one_minus() -> bool
{
  auto result_is_ok = true;

  const auto one_minus = local_wide_decimal_type(local_one()).negate();

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto left = generate_wide_decimal_value<local_wide_decimal_type>();

    const auto result_plus  = static_cast<local_wide_decimal_type>(left / local_one());
    const auto result_minus = static_cast<local_wide_decimal_type>(left / one_minus);

    const auto div_with_plus_minus_one_is_ok = ((result_plus == left) && (-result_minus == left));

    result_is_ok = (div_with_plus_minus_one_is_ok && result_is_ok);
  }

  result_is_ok =
  (
       (   ((static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))) / static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))) == static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1))) / static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1)))) == static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))) / static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))) == static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))))
        && ((static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1))) / static_cast<local_wide_decimal_type>(static_cast<  signed>( INT8_C(-1)))) == static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT8_C(+1)))))
    && result_is_ok
  );

  return result_is_ok;
}

auto test_div_by_other_sign_same() -> bool
{
  auto result_is_ok = true;

  eng_sgn.seed(util::util_pseudorandom_time_point_seed::value<typename eng_sgn_type::result_type>());
  eng_dig.seed(util::util_pseudorandom_time_point_seed::value<typename eng_dig_type::result_type>());
  eng_exp.seed(util::util_pseudorandom_time_point_seed::value<typename eng_exp_type::result_type>());

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(1024));
           ++i)
  {
    auto left = generate_wide_decimal_value<local_wide_decimal_type>();

    const local_wide_decimal_type right = -left;

    const local_wide_decimal_type result = left / right;

    const auto div_is_ok = (-result == 1);

    result_is_ok = (div_is_ok && result_is_ok);
  }

  return result_is_ok;
}

auto pi_left = // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)
  #if defined(WIDE_DECIMAL_NAMESPACE)
  WIDE_DECIMAL_NAMESPACE::math::wide_decimal::pi<local_wide_decimal_type::decwide_t_digits10, local_limb_type, std::allocator<void>>();
  #else
  ::math::wide_decimal::pi<local_wide_decimal_type::decwide_t_digits10, local_limb_type, std::allocator<void>>();
  #endif

auto pi_right = pi_left; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)

auto test_various_zero_operations() -> bool // NOLINT(readability-function-cognitive-complexity)
{
  auto result_is_ok = true;

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(16));
           ++i)
  {
    const auto left_zero = generate_wide_decimal_value<local_wide_decimal_type>() * local_zero();

    const auto left_zero_as_float        = static_cast<float>(left_zero);
    const auto left_zero_as_wide_decimal = static_cast<local_wide_decimal_type>(left_zero_as_float);

    const auto result_zero_is_ok = (left_zero_as_wide_decimal.iszero() && (left_zero.iszero() == left_zero_as_wide_decimal.iszero()));

    result_is_ok = (result_zero_is_ok && result_is_ok);

    using std::frexp;

    int expptr { };
    const auto result_constexpr_frexp_unsafe = frexp(left_zero_as_float, &expptr);

    const auto result_constexpr_frexp_unsafe_is_ok =
    (
         (!(result_constexpr_frexp_unsafe > static_cast<float>(0.0L)))
      && (!(result_constexpr_frexp_unsafe < static_cast<float>(0.0L)))
      && (expptr == static_cast<int>(INT8_C(0)))
    );

    result_is_ok = (result_constexpr_frexp_unsafe_is_ok && result_is_ok);
  }

  {
    const auto result_of_sub_same_is_ok =
    (
          (pi_left  > 3.1F)
      &&  (pi_right > 3.1F)
      && ((pi_left - pi_right) == static_cast<int>(INT8_C(0)))
      && ((pi_left - pi_right) == local_zero())
    );

    result_is_ok = (result_of_sub_same_is_ok && result_is_ok);
  }

  {
    std::stringstream strm;

    strm << std::setprecision(3) << std::showpoint << local_wide_decimal_type(0);

    const auto str_zero_ctrl = std::string("0.000");

    const auto str_zero = strm.str();

    const auto str_zero_is_ok = (str_zero == str_zero_ctrl);

    result_is_ok = (str_zero_is_ok && result_is_ok);
  }

  {
    std::stringstream strm;

    strm << std::setprecision(3) << std::showpoint << std::showpos << local_wide_decimal_type(0);

    const auto str_zero_ctrl = std::string("+0.000");

    const auto str_zero = strm.str();

    const auto str_zero_is_ok = (str_zero == str_zero_ctrl);

    result_is_ok = (str_zero_is_ok && result_is_ok);
  }

  {
    std::stringstream strm;

    strm << std::setprecision(static_cast<int>(INT8_C(0))) << pi_left;

    const auto str_pi_left = strm.str();

    const auto str_pi_ctrl = std::string("3.14159265358979323846264338");

    const auto str_pi_left_is_ok = (str_pi_ctrl.find(str_pi_left) != std::string::npos);

    result_is_ok = (str_pi_left_is_ok && result_is_ok);
  }

  {
    std::stringstream strm;

    strm << std::setprecision(static_cast<int>(INT8_C(2))) << std::fixed << pi_right;

    const auto str_pi_right = strm.str();

    const auto str_pi_right_is_ok = (str_pi_right == "3.14");

    result_is_ok = (str_pi_right_is_ok && result_is_ok);
  }

  {
    local_wide_decimal_type small =
      local_wide_decimal_type::from_lst
      (
        {
          static_cast<typename local_wide_decimal_type::limb_type>(UINT8_C(1))
        },
        static_cast<typename local_wide_decimal_type::exponent_type>
        (
          local_wide_decimal_type::decwide_t_min_exp10 + static_cast<std::int32_t>(INT8_C(32))
        )
      );

    auto result_underflow_is_ok = false;

    for(auto   i = static_cast<unsigned>(UINT8_C(0));
               i < static_cast<unsigned>(UINT8_C(200));
             ++i)
    {
      small /= static_cast<unsigned>(UINT8_C(7));

      if(   (small < (std::numeric_limits<local_wide_decimal_type>::min)())
         && (small == static_cast<int>(INT8_C(0))))
      {
        result_underflow_is_ok = true;

        break;
      }
    }

    result_is_ok = (result_underflow_is_ok && result_is_ok);
  }

  {
    using local_rep_type   = typename local_wide_decimal_type::representation_type;
    using local_value_type = typename local_rep_type::value_type;

    local_rep_type rep(local_rep_type::static_size(), (std::numeric_limits<local_value_type>::max)());

    rep.fill(static_cast<local_value_type>(UINT8_C(0)));

    const auto result_zero_rep_is_ok = std::equal(rep.cbegin(), rep.cend(), local_zero().crepresentation().cbegin());

    result_is_ok = (result_zero_rep_is_ok && result_is_ok);
  }

  {
    using local_value_type = typename local_wide_decimal_type::representation_type::value_type;

    #if defined(WIDE_DECIMAL_NAMESPACE)
    using static_array_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::detail::fixed_static_array<local_value_type, local_wide_decimal_type::decwide_t_elem_number>;
    #else
    using static_array_type = ::math::wide_decimal::detail::fixed_static_array<local_value_type, local_wide_decimal_type::decwide_t_elem_number>;
    #endif

    static_array_type
      other_rep
      (
        static_cast<typename static_array_type::size_type>(UINT8_C(2)),
        static_cast<typename static_array_type::value_type>(UINT8_C(42))
      );

    const auto result_other_rep_is_ok =
      (
           (other_rep.at(static_cast<typename static_array_type::size_type>(UINT8_C(0))) == static_cast<typename static_array_type::value_type>(UINT8_C(42)))
        && (other_rep.at(static_cast<typename static_array_type::size_type>(UINT8_C(1))) == static_cast<typename static_array_type::value_type>(UINT8_C(42)))
        && std::equal
           (
             other_rep.cbegin() + static_cast<typename static_array_type::size_type>(UINT8_C(2)),
             other_rep.cend(),
             local_zero().crepresentation().cbegin() + static_cast<typename static_array_type::size_type>(UINT8_C(2))
           )
      );

      result_is_ok = (result_other_rep_is_ok && result_is_ok);
  }

  {
    const std::string str_zeros(static_cast<std::size_t>(33U), '0');

    const local_wide_decimal_type z(str_zeros.c_str());

    result_is_ok = ((z == local_zero()) && (z == static_cast<int>(INT8_C(0))) && result_is_ok);
  }

  {
    using std::exp;

    const auto exp_zero = exp(local_zero());

    const auto result_exp_zero_is_ok = (exp_zero == static_cast<int>(INT8_C(1)));

    result_is_ok = (result_exp_zero_is_ok && result_is_ok);
  }

  using std::pow;

  {
    const auto zero_raised_to_the_zero = pow(local_zero(), local_zero());

    const auto result_zero_raised_to_the_zero_is_ok = (zero_raised_to_the_zero == static_cast<int>(INT8_C(1)));

    result_is_ok = (result_zero_raised_to_the_zero_is_ok && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto x          = generate_wide_decimal_value<local_wide_decimal_type>(true);
    const auto x_pow_zero = pow(x, local_zero());

    result_is_ok = ((x_pow_zero == static_cast<int>(INT8_C(1))) && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto x              = generate_wide_decimal_value<local_wide_decimal_type>();
    const auto x_div_zero     = x / local_zero();
    const auto x_div_zero_ull = x / static_cast<unsigned long long>(local_zero()); // NOLINT(google-runtime-int)

    result_is_ok = ((x_div_zero == static_cast<int>(INT8_C(0))) && (x_div_zero_ull == static_cast<int>(INT8_C(0))) && result_is_ok);
  }

  return result_is_ok;
}

auto test_various_one_operations() -> bool
{
  auto result_is_ok = true;

  {
    const auto local_ten = local_wide_decimal_type(static_cast<unsigned>(UINT8_C(10)));

    for(auto u = static_cast<unsigned>(UINT8_C(1)); u < static_cast<unsigned>(UINT8_C(10)); ++u)
    {
      using std::ceil;

      const auto ceil_frac = ceil(local_wide_decimal_type(u) / local_ten);

      const auto result_ceil_frac_is_ok = (ceil_frac == local_one());

      result_is_ok = (result_ceil_frac_is_ok && result_is_ok);
    }
  }

  {
    using std::log;

    const auto log_one = log(local_one());

    const auto result_log_one_is_ok = (log_one == static_cast<int>(INT8_C(0)));

    result_is_ok = (result_log_one_is_ok && result_is_ok);
  }

  {
    const auto tol = static_cast<local_wide_decimal_type>(std::numeric_limits<local_wide_decimal_type>::epsilon() * 1000.0F);

    for(auto   i = static_cast<unsigned>(UINT32_C( 11));
               i < static_cast<unsigned>(UINT32_C(100));
             ++i)
    {
      const auto x = local_wide_decimal_type(local_wide_decimal_type(i) / static_cast<unsigned>(UINT8_C(100)));

      using std::log;
      const auto lg_x      =  log(x);
      const auto lg_x_ctrl = -log(local_one() / x);

      using std::fabs;

      const auto delta_lg_x = fabs(1 - (lg_x / lg_x_ctrl));

      const auto result_lg_x_is_ok = (delta_lg_x < tol);

      result_is_ok = (result_lg_x_is_ok && result_is_ok);

      if(i == static_cast<unsigned>(UINT32_C(50)))
      {
        const auto& ln_two_ctrl = 
          #if defined(WIDE_DECIMAL_NAMESPACE)
          WIDE_DECIMAL_NAMESPACE::math::wide_decimal::ln_two<local_wide_decimal_type::decwide_t_digits10, local_limb_type, std::allocator<void>>();
          #else
          ::math::wide_decimal::ln_two<local_wide_decimal_type::decwide_t_digits10, local_limb_type, std::allocator<void>>();
          #endif

        const auto delta_ln_half = fabs(1 - (lg_x / -ln_two_ctrl));

        const auto result_ln_half_is_ok = (delta_ln_half < tol);

        result_is_ok = (result_ln_half_is_ok && result_is_ok);
      }
    }
  }

  {
    std::stringstream strm;

    strm << local_near_one();

    const auto local_near_one_as_float = static_cast<float>(local_near_one());

    using std::fabs;

    const auto delta = fabs(1.0F - (local_near_one_as_float / 1.0F));

    const auto cast_local_near_one_to_float_is_ok = (delta < (std::numeric_limits<float>::epsilon() * 10.0F));

    result_is_ok = ((strm.str() == "1") && cast_local_near_one_to_float_is_ok && result_is_ok);
  }

  {
    std::stringstream strm;

    using std::ceil;

    strm << std::setprecision(std::numeric_limits<local_wide_decimal_type>::digits10)
         << ceil(local_near_one());

    const std::string str_one_from_strm = strm.str();

    result_is_ok = ((str_one_from_strm == "1") && result_is_ok);
  }

  {
    std::stringstream strm;

    strm << local_not_one();

    const auto local_not_one_as_float = static_cast<float>(local_not_one());

    using std::fabs;

    const auto delta = fabs(1.0F - (local_not_one_as_float / 0.9F));

    const auto cast_local_not_one_to_float_is_ok = (delta < (std::numeric_limits<float>::epsilon() * 10.0F));

    result_is_ok = ((strm.str() != "1") && cast_local_not_one_to_float_is_ok && result_is_ok);
  }

  {
    const auto one_div_near_one = local_one() / local_near_one();

    std::stringstream strm;

    strm << one_div_near_one;

    result_is_ok = ((strm.str() == "1") && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    const auto x               = generate_wide_decimal_value<local_wide_decimal_type>();
    const auto x_div_one       = x /  local_one();
    const auto x_div_near_one  = x /  local_near_one();
    const auto x_div_one_minus = x / -local_one();

    result_is_ok = ((x_div_one == x) && (x_div_near_one == x) && (x_div_one_minus == -x) && result_is_ok);
  }

  return result_is_ok;
}

auto test_various_min_max_operations() -> bool
{
  auto result_is_ok = true;

  {
    const auto local_max_ull = local_wide_decimal_type((std::numeric_limits<unsigned long long>::max)()); // NOLINT(google-runtime-int)

    const auto local_max_ull_excess = local_max_ull * local_max_ull;

    auto result_local_max_ull_is_ok =
      (
           static_cast<unsigned long long>(local_max_ull)        == (std::numeric_limits<unsigned long long>::max)() // NOLINT(google-runtime-int)
        && static_cast<unsigned long long>(local_max_ull_excess) == (std::numeric_limits<unsigned long long>::max)() // NOLINT(google-runtime-int)
      );

    const auto local_max_sll = local_wide_decimal_type((std::numeric_limits<signed long long>::max)()); // NOLINT(google-runtime-int)

    const auto local_max_sll_excess = local_max_sll * local_max_sll;

    auto result_local_max_sll_is_ok =
      (
           static_cast<signed long long>(local_max_sll)        == (std::numeric_limits<signed long long>::max)() // NOLINT(google-runtime-int)
        && static_cast<signed long long>(local_max_sll_excess) == (std::numeric_limits<signed long long>::max)() // NOLINT(google-runtime-int)
      );

    const auto local_min_sll = local_wide_decimal_type((std::numeric_limits<signed long long>::min)()); // NOLINT(google-runtime-int)

    const auto local_min_sll_excess = -(local_min_sll * local_min_sll);

    auto result_local_min_sll_is_ok =
      (
           static_cast<signed long long>(local_min_sll)        == (std::numeric_limits<signed long long>::min)() // NOLINT(google-runtime-int)
        && static_cast<signed long long>(local_min_sll_excess) == (std::numeric_limits<signed long long>::min)() // NOLINT(google-runtime-int)
      );

    #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
    const auto local_max_ldbl = static_cast<local_wide_decimal_type>((std::numeric_limits<long double>::max)());

    const auto local_ldbl_inf =
      static_cast<long double>
      (
        static_cast<local_wide_decimal_type>
        (
          local_max_ldbl * (std::numeric_limits<long double>::max)()
        )
      );

    const volatile auto ratio =
      static_cast<long double>
      (
        static_cast<long double>(local_max_ldbl) / (std::numeric_limits<long double>::max)()
      );

    using std::fabs;

    const auto delta_ldbl = fabs(static_cast<long double>(1.0F) - ratio);

    constexpr auto tol_ldbl =
      static_cast<long double>
      (
        std::numeric_limits<long double>::epsilon() * static_cast<long double>(2.0F)
      );

    #if defined(__GNUC__)
    const auto my_ldbl_isinf =
    (
         ( local_ldbl_inf == std::numeric_limits<long double>::infinity())
      || (-local_ldbl_inf == std::numeric_limits<long double>::infinity())
    );
    #else
    using std::isinf;
    const auto my_ldbl_isinf = (isinf)(local_ldbl_inf);
    #endif

    const auto result_local_max_ldbl_is_ok = ((delta_ldbl < tol_ldbl) && my_ldbl_isinf);
    #else
    const auto result_local_max_ldbl_is_ok = true;
    #endif

    const auto result_min_max_is_ok =
      (
           result_local_max_ull_is_ok
        && result_local_max_sll_is_ok
        && result_local_min_sll_is_ok
        && result_local_max_ldbl_is_ok
      );

    result_is_ok = (result_min_max_is_ok && result_is_ok);
  }

  {
    const auto local_near_max = (std::numeric_limits<local_wide_decimal_type>::max)() / 7U;

    const auto value_having_expected_overflow = local_near_max * 1000U;

    const bool result_overflow_is_ok =
      (value_having_expected_overflow == (std::numeric_limits<local_wide_decimal_type>::max)());

    result_is_ok = (result_overflow_is_ok && result_is_ok);
  }

  {
    const auto local_near_max = (std::numeric_limits<local_wide_decimal_type>::max)() / 7U;

    const auto value_having_expected_overflow = local_near_max * local_wide_decimal_type(static_cast<unsigned>(UINT16_C(1000)));

    const bool
      result_overflow_is_ok
      {
           (local_near_max < (std::numeric_limits<local_wide_decimal_type>::max)())
        && (value_having_expected_overflow == (std::numeric_limits<local_wide_decimal_type>::max)())
      };

    result_is_ok = (result_overflow_is_ok && result_is_ok);
  }

  {
    const auto local_max    = (std::numeric_limits<local_wide_decimal_type>::max)();
    const auto local_lowest = std::numeric_limits<local_wide_decimal_type>::lowest();

    const bool
      result_local_max_is_ok
      {
        (local_max > local_wide_decimal_type { (std::numeric_limits<long double>::max)() })
      };

    result_is_ok = (result_local_max_is_ok && result_is_ok);

    {
      using local_representation_type = typename local_wide_decimal_type::representation_type;

      const local_representation_type negated_lowest_rep { (-local_lowest).crepresentation() };

      const bool
        result_local_lowest_is_ok
        {
             (local_lowest < local_wide_decimal_type { std::numeric_limits<long double>::lowest() })
          && std::equal
             (
               negated_lowest_rep.cbegin(),
               negated_lowest_rep.cend(),
               local_max.crepresentation().cbegin()
             )
        };

      result_is_ok = (result_local_lowest_is_ok && result_is_ok);
    }
  }

  return result_is_ok;
}

#if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
auto test_frexp_in_all_ranges() -> bool
{
  eng_sgn.seed(util::util_pseudorandom_time_point_seed::value<typename eng_sgn_type::result_type>());
  eng_dig.seed(util::util_pseudorandom_time_point_seed::value<typename eng_dig_type::result_type>());
  eng_exp.seed(util::util_pseudorandom_time_point_seed::value<typename eng_exp_type::result_type>());

  auto result_is_ok = true;

  {
    using std::frexp;

    int expon { };
    const local_wide_decimal_type frexp_value = frexp(local_zero(), &expon);

    const auto result_frexp_of_zero_is_ok = 
      (
           (frexp_value == 0)
        && (frexp_value == local_zero())
        && (expon == 0)
      );

    result_is_ok = (result_frexp_of_zero_is_ok && result_is_ok);
  }

  {
    local_wide_decimal_type special_neg_value =
      -local_wide_decimal_type::from_lst
       (
         { static_cast<local_limb_type>(UINT32_C(312)), static_cast<local_limb_type>(UINT32_C(5000)) },
         static_cast<typename local_wide_decimal_type::exponent_type>(INT8_C(-4))
       );

    {
      using std::frexp;

      int expon { };
      const local_wide_decimal_type frexp_value = frexp(special_neg_value, &expon);

      std::stringstream strm;

      strm << std::setprecision(static_cast<std::streamsize>(INT8_C(4)))
           << std::fixed
           << frexp_value;

      const auto result_frexp_is_ok = ((strm.str() == "-0.5000") && (expon == -4));

      result_is_ok = (result_frexp_is_ok && result_is_ok);
    }

    {
      using std::frexp;

      const auto special_neg_value_as_float = static_cast<float>(special_neg_value);

      int expon_for_float { };
      const auto frexp_value_as_float = frexp(special_neg_value_as_float, &expon_for_float);

      const auto result_frexp_as_float_is_ok = ((frexp_value_as_float == -0.5F) && (expon_for_float == -4));

      result_is_ok = (result_frexp_as_float_is_ok && result_is_ok);
    }
  }

  constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(2048));

  for(auto i = static_cast<unsigned>(UINT8_C(0)); i < i_max_test; ++i)
  {
    const auto x =
      generate_wide_decimal_value<local_wide_decimal_type>
      (
        false,
        static_cast<int>(static_cast<float>(std::numeric_limits<float>::max_exponent10) * 0.65F),
        std::numeric_limits<float>::digits10
      );

    using std::frexp;

    int expon { };
    const local_wide_decimal_type frexp_value = frexp(x, &expon);

    std::stringstream strm;

    strm << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::max_digits10))
         << std::fixed
         << frexp_value;

    float frexp_value_as_float_from_strm { };
    strm >> frexp_value_as_float_from_strm;

    int expon_from_cast { };
    const auto frexp_value_as_float_from_cast = frexp(static_cast<float>(x), &expon_from_cast);

    const auto delta_frexp_as_float =
      (1.0F - (frexp_value_as_float_from_strm / frexp_value_as_float_from_cast));

    const auto result_frexp_as_float_is_ok =
      (delta_frexp_as_float <= (std::numeric_limits<float>::epsilon() * 2.0F));

    result_is_ok = (result_frexp_as_float_is_ok && result_is_ok);
  }

  for(auto i = static_cast<unsigned>(UINT8_C(0)); i < i_max_test; ++i)
  {
    const auto x =
      generate_wide_decimal_value<local_wide_decimal_type>
      (
        false,
        0,
        std::numeric_limits<float>::digits10
      );

    using std::frexp;

    int expon { };
    const local_wide_decimal_type frexp_value = frexp(x, &expon);

    std::stringstream strm;

    strm << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<float>::max_digits10))
         << std::fixed
         << frexp_value;

    float frexp_value_as_float_from_strm { };
    strm >> frexp_value_as_float_from_strm;

    int expon_from_cast { };
    const auto frexp_value_as_float_from_cast = frexp(static_cast<float>(x), &expon_from_cast);

    const auto delta_frexp_as_float =
      (1.0F - (frexp_value_as_float_from_strm / frexp_value_as_float_from_cast));

    const auto result_frexp_as_float_is_ok =
      (delta_frexp_as_float <= (std::numeric_limits<float>::epsilon() * 2.0F));

    result_is_ok = (result_frexp_as_float_is_ok && result_is_ok);
  }

  return result_is_ok;
}
#endif

auto test_string_ops_and_round_trips() -> bool
{
  eng_sgn.seed(util::util_pseudorandom_time_point_seed::value<typename eng_sgn_type::result_type>());
  eng_dig.seed(util::util_pseudorandom_time_point_seed::value<typename eng_dig_type::result_type>());
  eng_exp.seed(util::util_pseudorandom_time_point_seed::value<typename eng_exp_type::result_type>());

  auto result_is_ok = true;

  {
    static constexpr auto zeros_to_insert =
      static_cast<std::size_t>
      (
          static_cast<std::size_t>(std::numeric_limits<local_wide_decimal_type>::digits10)
        * static_cast<std::size_t>(UINT8_C(3))
      );

    const auto ten_pow_30 = local_wide_decimal_type("1E30");

    #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
    constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(8192));
    #else
    constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(1024));
    #endif

    for(auto i = static_cast<unsigned>(UINT8_C(0)); i < i_max_test; ++i)
    {
      using uint_digits_array_type = std::array<char, static_cast<std::size_t>(UINT8_C(5))>;

      const auto x =
        generate_wide_decimal_value<local_wide_decimal_type>
        (
          true,
          0,
          static_cast<int>(std::tuple_size<uint_digits_array_type>::value)
        );

      const auto big_uint_str_rep =
        [&x]() // NOLINT(modernize-use-trailing-return-type) // LCOV_EXCL_LINE
        {
          uint_digits_array_type data_uint_buf { };

          static_cast<void>(util::baselexical_cast(static_cast<std::uint32_t>(x), data_uint_buf.data(), data_uint_buf.data() + data_uint_buf.size()));

          std::string
            str_rep
            (
              static_cast<std::size_t>
              (
                  std::tuple_size<uint_digits_array_type>::value
                + static_cast<std::size_t>(UINT8_C(30))
              ),
              '0'
            );

          std::copy(data_uint_buf.cbegin(), data_uint_buf.cend(), str_rep.begin());

          str_rep.insert(str_rep.end(), '.');
          str_rep.insert(str_rep.end(), zeros_to_insert, '0');

          return str_rep;
        }();

      const auto result_big_uint_is_ok =
        [&x, &big_uint_str_rep, &ten_pow_30]() // NOLINT(modernize-use-trailing-return-type) // LCOV_EXCL_LINE
        {
          std::stringstream strm;

          strm << std::fixed
               << std::setprecision(static_cast<std::streamsize>(zeros_to_insert))
               << static_cast<local_wide_decimal_type>(x * ten_pow_30);

          return (big_uint_str_rep == strm.str());
        }();

        result_is_ok = (result_big_uint_is_ok && result_is_ok);
    }
  }

  for(auto i = static_cast<unsigned>(UINT32_C(100));
           i < static_cast<unsigned>(UINT32_C(1000)); // NOLINT(altera-id-dependent-backward-branch)
           i = static_cast<unsigned>(i + static_cast<unsigned>(UINT32_C(100))))
  {
    std::stringstream strm1;
    std::stringstream strm2;

    const auto pi_frac1 = static_cast<local_wide_decimal_type>(pi_left / static_cast<unsigned>(UINT32_C(1000)));
          auto pi_frac2 = local_wide_decimal_type { };

    strm1 << std::fixed << std::setprecision(static_cast<int>(INT8_C(3))) << pi_frac1;

    strm1 >> pi_frac2;

    strm2 << std::fixed << std::setprecision(static_cast<int>(i)) << pi_frac2;

    const auto str_pi_padded = strm2.str();

    std::string str_pi_padded_ctrl(static_cast<std::size_t>(i - static_cast<unsigned>(UINT8_C(5))), '0');

    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '4');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '1');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '3');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '0');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '0');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '.');
    str_pi_padded_ctrl.insert(str_pi_padded_ctrl.begin(), '0');

    const auto result_str_pi_padded_is_ok = (str_pi_padded == str_pi_padded_ctrl);

    result_is_ok = (result_str_pi_padded_is_ok && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(1));
             i < static_cast<unsigned>(UINT8_C(10));
           ++i)
  {
    const auto x = local_wide_decimal_type(i);

    std::stringstream strm;

    strm << std::scientific
         << std::noshowpoint
         << std::uppercase
         << std::setprecision(static_cast<std::streamsize>(INT8_C(0)))
         << x;

    const auto str_of_digit = strm.str();

    auto str_of_digit_ctrl = std::string("xE+000");

    str_of_digit_ctrl.at(static_cast<std::size_t>(UINT8_C(0))) =
      static_cast<char>
      (
        i + static_cast<unsigned>(UINT8_C(0x30))
      );

    const auto result_str_of_digit_is_ok = (str_of_digit == str_of_digit_ctrl);

    result_is_ok = (result_str_of_digit_is_ok && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(1));
             i < static_cast<unsigned>(UINT8_C(10));
           ++i)
  {
    std::array<char, static_cast<std::size_t>(UINT8_C(4))> digits_to_use =
    {
      static_cast<char>(i + static_cast<unsigned>(UINT8_C(0x30))),
      '.',
      static_cast<char>(i + static_cast<unsigned>(UINT8_C(0x30))),
      '\0'
    };

    const auto x = local_wide_decimal_type(digits_to_use.data());

    std::stringstream strm;

    strm << std::fixed
         << std::setw(static_cast<std::streamsize>(INT8_C(20)))
         << std::setfill('#')
         << std::setprecision(static_cast<std::streamsize>(INT8_C(1)))
         << x;

    const auto str_of_digits = strm.str();

    auto str_of_digits_ctrl = std::string(static_cast<std::size_t>(UINT8_C(17)), '#');

    str_of_digits_ctrl.insert(str_of_digits_ctrl.end(), digits_to_use.at(static_cast<std::size_t>(UINT8_C(2))));
    str_of_digits_ctrl.insert(str_of_digits_ctrl.end(), digits_to_use.at(static_cast<std::size_t>(UINT8_C(1))));
    str_of_digits_ctrl.insert(str_of_digits_ctrl.end(), digits_to_use.at(static_cast<std::size_t>(UINT8_C(0))));

    const auto result_str_of_digit_is_ok = (str_of_digits == str_of_digits_ctrl);

    result_is_ok = (result_str_of_digit_is_ok && result_is_ok);
  }

  const auto tol =
    static_cast<local_wide_decimal_type>
    (
      (static_cast<local_wide_decimal_type>(1U) / 1000U)
    );

  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(8192));
  #else
  constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(1024));
  #endif

  for(auto i = static_cast<unsigned>(UINT8_C(0)); i < i_max_test; ++i)
  {
    const auto x =
      generate_wide_decimal_value<local_wide_decimal_type>
      (
        false,
        static_cast<int>(static_cast<float>(std::numeric_limits<double>::max_exponent10) * 0.65F)
      );

    {
      std::stringstream strm;

      // Use I/O streaming with no flags whatsoever.
      strm << x;

      using std::fabs;

      const auto delta = fabs(1 - fabs(x / local_wide_decimal_type(strm.str().c_str())));

      result_is_ok = ((delta < tol) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with fixed flag only.
      strm << std::fixed << x;

      using std::fabs;

      const auto x_strm = local_wide_decimal_type(strm.str().c_str());

      const auto delta = fabs(1 - fabs(x / x_strm));

      result_is_ok = ((delta < tol) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with scientific flag only.
      strm << std::scientific << x;

      using std::fabs;

      const auto delta = fabs(1 - fabs(x / local_wide_decimal_type(strm.str().c_str())));

      result_is_ok = ((delta < tol) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with precision flags only.
      strm << std::setprecision(std::numeric_limits<local_wide_decimal_type>::max_digits10)
           << x;

      result_is_ok = ((x == local_wide_decimal_type(strm.str().c_str())) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with precision and fixed flags combined.
      strm << std::setprecision(std::numeric_limits<local_wide_decimal_type>::max_digits10)
           << std::fixed
           << x;

      result_is_ok = ((x == local_wide_decimal_type(strm.str().c_str())) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with precision and scientific flags combined.
      strm << std::setprecision(std::numeric_limits<local_wide_decimal_type>::max_digits10)
           << std::scientific
           << x;

      result_is_ok = ((x == local_wide_decimal_type(strm.str().c_str())) && result_is_ok);
    }

    {
      std::stringstream strm;

      // Use I/O streaming with precision and showpoint flags combined.
      strm << std::setprecision(std::numeric_limits<local_wide_decimal_type>::max_digits10)
           << std::showpoint
           << x;

      result_is_ok = ((x == local_wide_decimal_type(strm.str().c_str())) && result_is_ok);
    }
  }

  return result_is_ok;
}

auto test_various_rootn() -> bool
{
  auto result_is_ok = true;

  {
    using std::sqrt;

    const auto sqrt_minus_one  = sqrt(-local_one());

    const auto result_sqrt_minus_one_is_ok = (sqrt_minus_one == 0);

    result_is_ok = (result_sqrt_minus_one_is_ok && result_is_ok);
  }

  {
    using std::sqrt;

    const auto sqrt_one  = sqrt(local_one());
    const auto sqrt_zero = sqrt(local_zero());

    const auto result_sqrt_one_zero_is_ok =
      (
           (sqrt_one  == 1)
        && (sqrt_one  == local_one())
        && (sqrt_zero == 0)
        && (sqrt_zero == local_zero())
      );

    result_is_ok = (result_sqrt_one_zero_is_ok && result_is_ok);
  }

  const auto tol = local_wide_decimal_type(local_wide_decimal_type(1U) / 1000U);

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(16));
           ++i)
  {
    const auto x = generate_wide_decimal_value<local_wide_decimal_type>(true);

    const auto r = rootn(x, static_cast<std::int32_t>(INT8_C(1)));

    const auto result_rootn_self_one_is_ok = (r == x);

    result_is_ok = (result_rootn_self_one_is_ok && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(256));
           ++i)
  {
    const auto x = generate_wide_decimal_value<local_wide_decimal_type>(true);

    const auto r = rootn(x, static_cast<std::int32_t>(INT8_C(2)));

    using std::fabs;
    using std::sqrt;

    const auto delta = fabs(1 - (r / sqrt(x)));

    const auto result_rootn_sqrt_is_ok = (delta < tol);

    result_is_ok = (result_rootn_sqrt_is_ok && result_is_ok);
  }

  {
    const auto x = local_wide_decimal_type(local_wide_decimal_type(static_cast<unsigned>(UINT32_C(1011))) / static_cast<unsigned>(UINT8_C(10)));

    const auto r3_p = rootn(+x, static_cast<std::int32_t>(INT8_C(3)));
    const auto r3_m = rootn(-x, static_cast<std::int32_t>(INT8_C(3)));

    // N[(1011/10)^(1/3), 72]
    const auto r3_ctrl = local_wide_decimal_type("4.65854596766714528445828561942593317263489860860972122382125818005540070");

    using std::fabs;

    const auto delta_p = fabs(1 - (r3_p / +r3_ctrl));
    const auto delta_m = fabs(1 - (r3_m / -r3_ctrl));

    const auto result_rootn_cbrt_p_is_ok = (delta_p < tol);
    const auto result_rootn_cbrt_m_is_ok = (delta_m < tol);

    result_is_ok = (result_rootn_cbrt_p_is_ok && result_is_ok);
    result_is_ok = (result_rootn_cbrt_m_is_ok && result_is_ok);
  }

  {
    const auto x = local_wide_decimal_type(local_wide_decimal_type(static_cast<unsigned>(UINT32_C(1011))) / static_cast<unsigned>(UINT8_C(10)));

    const auto root_p4 = rootn(x, static_cast<std::int32_t>(INT8_C(4)));
    const auto root_m4 = rootn(x, static_cast<std::int32_t>(INT8_C(-4)));

    // N[(1011/10)^(1/4), 72]
    const auto root_p4_ctrl = local_wide_decimal_type("3.17093828009936777476602468193014551551860637726903314273455186255021157");

    // N[(1011/10)^(-1/4), 72]
    const auto root_m4_ctrl = local_wide_decimal_type("0.315364069454124781696219443391001124974032476918702238114164357323042771");

    using std::fabs;

    const auto delta_root_p4 = fabs(1 - (root_p4 / root_p4_ctrl));
    const auto delta_root_m4 = fabs(1 - (root_m4 / root_m4_ctrl));

    const auto result_root_p4_is_ok = (delta_root_p4 < tol);
    const auto result_root_m4_is_ok = (delta_root_m4 < tol);

    result_is_ok = (result_root_p4_is_ok && result_is_ok);
    result_is_ok = (result_root_m4_is_ok && result_is_ok);
  }

  {
    const auto x = local_wide_decimal_type(local_wide_decimal_type(static_cast<unsigned>(UINT32_C(1011))) / static_cast<unsigned>(UINT8_C(10)));

    const auto root_m4 = rootn_inv(x, static_cast<std::int32_t>(INT8_C(4)));

    // N[1 / ((1011/10)^(1/4)), 72]
    const auto root_m4_ctrl = local_wide_decimal_type("0.315364069454124781696219443391001124974032476918702238114164357323042771");

    using std::fabs;

    const auto delta_root_m4 = fabs(1 - (root_m4 / root_m4_ctrl));

    const auto result_root_m4_is_ok = (delta_root_m4 < tol);

    result_is_ok = (result_root_m4_is_ok && result_is_ok);
  }

  {
    const auto x_neg = local_wide_decimal_type(local_wide_decimal_type(static_cast<unsigned>(UINT32_C(1011))) / static_cast<signed>(INT8_C(-10)));

    const auto root_x_neg = rootn(x_neg, static_cast<std::int32_t>(INT8_C(4)));

    const auto result_root_x_neg_is_ok = (x_neg < static_cast<int>(INT8_C(0))) && (root_x_neg == static_cast<int>(INT8_C(0)));

    result_is_ok = (result_root_x_neg_is_ok && result_is_ok);
  }

  return result_is_ok;
}

template<typename NativeFloatType>
auto test_to_native_float_and_back() -> bool
{
  using native_float_type = NativeFloatType;

  static_assert(std::is_floating_point<native_float_type>::value, "Error: Invalid template floating-point parameter");

  auto result_is_ok = true;

  constexpr auto tol_factor = static_cast<native_float_type>(2.0L);

  const     auto tol1 = static_cast<local_wide_decimal_type>(static_cast<native_float_type>(std::numeric_limits<native_float_type>::epsilon()) * tol_factor);
  constexpr auto tol2 = static_cast<native_float_type>      (static_cast<native_float_type>(std::numeric_limits<native_float_type>::epsilon()) * tol_factor);

  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(4096));
  #else
  constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(512));
  #endif

  for(auto i = static_cast<unsigned>(UINT32_C(0)); i < i_max_test; ++i)
  {
    const auto x =
      generate_wide_decimal_value<local_wide_decimal_type>
      (
        false,
        static_cast<int>(static_cast<float>(std::numeric_limits<native_float_type>::max_exponent10) * static_cast<float>(0.85L))
      );

    const auto x_as_native_float = static_cast<native_float_type>(static_cast<long double>(x));

    const auto x_as_wide_decimal_reloaded_from_native_float = local_wide_decimal_type(x_as_native_float);
    const auto x_as_native_float_reloaded_from_wide_decimal = static_cast<native_float_type>(x_as_wide_decimal_reloaded_from_native_float);

    using std::fabs;

    const auto delta1 = fabs(local_one() - fabs(x / x_as_wide_decimal_reloaded_from_native_float));
    const auto delta2 = fabs(static_cast<native_float_type>(1.0F) - fabs(x_as_native_float / x_as_native_float_reloaded_from_wide_decimal));

    result_is_ok = ((delta1 < tol1) && (delta2 < tol2) && result_is_ok);

    {
      std::stringstream strm;

      const auto prec_of_strm =
        (std::min)
        (
          std::numeric_limits<local_wide_decimal_type>::digits10,
          static_cast<int>(std::numeric_limits<native_float_type>::max_digits10 + static_cast<int>(INT8_C(4)))
        );

      strm << std::scientific << std::setprecision(prec_of_strm) << x;

      const auto ld_from_strm = strtold(strm.str().c_str(), nullptr);

      const auto x_as_native_float_from_strm = static_cast<native_float_type>(ld_from_strm);

      const auto delta3 = fabs(static_cast<native_float_type>(1.0F) - fabs(x_as_native_float / x_as_native_float_from_strm));

      const auto tol3 = tol2;

      result_is_ok = ((delta3 < tol3) && result_is_ok);
    }
  }

  return result_is_ok;
}

template<typename NativeUnsignedIntegralType>
auto test_various_int_operations() -> bool
{
  using local_unsigned_type = NativeUnsignedIntegralType;
  using local_signed_type   = typename std::make_signed<local_unsigned_type>::type;

  constexpr auto unsigned_integral_digits10_to_use =
    static_cast<unsigned>
    (
      std::numeric_limits<local_unsigned_type>::digits10 -1
    );

  const auto unsigned_integral_digits10_low_limit =
    static_cast<unsigned>
    (
      (std::max)
      (
        static_cast<unsigned>(UINT32_C(2)),
        static_cast<unsigned>(std::numeric_limits<std::uint8_t>::digits10)
      )
    );

  static_assert(   (std::numeric_limits<local_unsigned_type>::digits >= static_cast<int>(INT8_C(16)))
                && (unsigned_integral_digits10_to_use >= static_cast<int>(INT8_C(3))),
                "Error: Template integral type parameter is not wide enough");

  bool result_is_ok = true;

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    for(auto   nd  = unsigned_integral_digits10_low_limit; // NOLINT(altera-id-dependent-backward-branch)
               nd <= unsigned_integral_digits10_to_use;    // NOLINT(altera-id-dependent-backward-branch)
             ++nd)
    {
      const auto x =
        generate_wide_decimal_value<local_wide_decimal_type>
        (
          true,
          0,
          static_cast<int>(nd)
        );

      const auto u = static_cast<local_unsigned_type>(x);

      const auto result_native_convert_is_ok = (local_wide_decimal_type(u) == x);

      result_is_ok = (result_native_convert_is_ok && result_is_ok);
    }
  }

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(UINT8_C(128));
           ++i)
  {
    for(auto   nd  = unsigned_integral_digits10_low_limit; // NOLINT(altera-id-dependent-backward-branch)
               nd <= unsigned_integral_digits10_to_use;    // NOLINT(altera-id-dependent-backward-branch)
             ++nd)
    {
      const auto x =
        generate_wide_decimal_value<local_wide_decimal_type>
        (
          false,
          0,
          static_cast<int>(nd)
        );

      const auto n = static_cast<local_signed_type>(x);

      const auto result_native_convert_is_ok = (local_wide_decimal_type(n) == x);

      result_is_ok = (result_native_convert_is_ok && result_is_ok);
    }
  }

  auto u10 = static_cast<local_unsigned_type>(UINT8_C(1));

  for(auto   i = static_cast<unsigned>(UINT8_C(0));
             i < static_cast<unsigned>(std::numeric_limits<local_signed_type>::digits10);
           ++i)
  {
    const auto s10 = static_cast<local_signed_type>(-static_cast<local_signed_type>(u10));

    const auto x = local_wide_decimal_type(s10);

    const auto result_native_convert_is_ok =
      (static_cast<local_unsigned_type>(x) == static_cast<local_unsigned_type>(s10));

    result_is_ok = (result_native_convert_is_ok && result_is_ok);

    u10 = static_cast<local_unsigned_type>(u10 * static_cast<local_unsigned_type>(UINT8_C(10)));
  }

  return result_is_ok;
}

auto test_odds_and_ends() -> bool
{
  auto result_is_ok = true;

  const auto tol = std::numeric_limits<local_wide_decimal_type>::epsilon() * static_cast<unsigned>(UINT16_C(1000));

  {
    #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
    constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(2048));
    #else
    constexpr auto i_max_test = static_cast<unsigned>(UINT32_C(512));
    #endif

    for(auto i = static_cast<unsigned>(UINT8_C(0)); i < i_max_test; ++i)
    {
      const auto x = generate_wide_decimal_value<local_wide_decimal_type>(false);

      using std::frexp;
      using std::ldexp;

      int expon { };
      const auto frexp_value = frexp(x, &expon);
      const auto ldexp_value = ldexp(frexp_value, expon);

      using std::fabs;

      const auto delta = fabs(1 - (x / ldexp_value));

      const auto result_frexp_ldexp_is_ok = (delta < tol);

      result_is_ok = (result_frexp_ldexp_is_ok && result_is_ok);
    }
  }

  {
    local_wide_decimal_type special_neg_value =
      -local_wide_decimal_type::from_lst
       (
         { static_cast<local_limb_type>(UINT32_C(312)), static_cast<local_limb_type>(UINT32_C(5000)) },
         static_cast<typename local_wide_decimal_type::exponent_type>(INT8_C(-4))
       );

    using local_internal_float_type = typename local_wide_decimal_type::internal_float_type;
    using local_exponent_type       = typename local_wide_decimal_type::exponent_type;

    local_internal_float_type mantissa { };
    local_exponent_type       expon    { };

    special_neg_value.extract_parts(mantissa, expon);

    const auto result_extract_parts_is_ok =
      ((mantissa == static_cast<local_internal_float_type>(static_cast<float>(-0.3125L))) && (expon == static_cast<int>(INT8_C(-1))));

    result_is_ok = (result_extract_parts_is_ok && result_is_ok);
  }

  {
    std::uniform_int_distribution<int> dist_m1_0_p1(-1, 1); // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
    std::mt19937 eng_m1_0_p1(util::util_pseudorandom_time_point_seed::value<typename std::mt19937::result_type>());

    unsigned cnt_pow_0 { };

    for(auto i = static_cast<unsigned>(UINT8_C(0)); i < static_cast<unsigned>(UINT8_C(128)); ++i)
    {
      const local_wide_decimal_type pi_val = pi_right;

      using std::pow;

      const int pwn { dist_m1_0_p1(eng_m1_0_p1) };

      auto pi_pow_zero = pow(pi_val, pwn);

      if(pwn == 0)
      {
        ++cnt_pow_0;

        const auto result_pi_pow_zero_is_ok = (pi_pow_zero == static_cast<int>(INT8_C(1)));

        result_is_ok = (result_pi_pow_zero_is_ok && result_is_ok);
      }
    }

    result_is_ok = ((cnt_pow_0 > static_cast<unsigned>(UINT8_C(0))) && result_is_ok);
  }

  {
    local_wide_decimal_type near_pi = pi_right;

    auto sub_pi = --near_pi;

    volatile auto result_sub_near_pi_is_ok = ((sub_pi < static_cast<float>(UINT8_C(3))) && (sub_pi > static_cast<float>(UINT8_C(2))));

    result_is_ok = (result_sub_near_pi_is_ok && result_is_ok);

    sub_pi = --near_pi;

    result_sub_near_pi_is_ok = ((sub_pi <static_cast<float>(UINT8_C(2))) && (sub_pi > static_cast<float>(UINT8_C(1))));

    result_is_ok = (result_sub_near_pi_is_ok && result_is_ok);
  }

  {
    local_wide_decimal_type frac_one_over_pi = static_cast<local_wide_decimal_type>(1 / pi_right);

    for(auto i = static_cast<unsigned>(UINT8_C(0)); i < static_cast<unsigned>(UINT8_C(10)); ++i)
    {
      auto np = int { };
      const auto fr = frexp(frac_one_over_pi, &np);

      const auto result_frexp_is_ok = (np < static_cast<int>(INT8_C(0)));

      result_is_ok = (result_frexp_is_ok && result_is_ok);

      frac_one_over_pi /= static_cast<int>(INT8_C(10));
    }
  }

  {
    local_wide_decimal_type mul_ten_of_pi = static_cast<local_wide_decimal_type>(pi_right * static_cast<unsigned>(UINT8_C(10)));

    for(auto i = static_cast<unsigned>(UINT8_C(0)); i < static_cast<unsigned>(UINT8_C(10)); ++i)
    {
      auto np = int { };
      const auto fr = frexp(mul_ten_of_pi, &np);

      const auto result_frexp_is_ok = (np > static_cast<int>(INT8_C(0)));

      result_is_ok = (result_frexp_is_ok && result_is_ok);

      mul_ten_of_pi *= static_cast<int>(INT8_C(10));
    }
  }

  {
    auto nf = int { };
    const auto fr = frexp(local_zero(), &nf);

    const auto result_frexp_of_zero_is_ok = ((fr == static_cast<int>(INT8_C(0))) && (nf == static_cast<int>(INT8_C(0))));

    result_is_ok = (result_frexp_of_zero_is_ok && result_is_ok);
  }

  for(auto i = static_cast<unsigned>(UINT8_C(0)); i < static_cast<unsigned>(UINT16_C(128)); ++i)
  {
    const auto n_exp2 = static_cast<int>(static_cast<int>(i) - static_cast<int>(INT8_C(64)));

    using std::ldexp;

    const auto flt = ldexp(static_cast<float>(3.14L), n_exp2);
    const auto dec = ldexp(static_cast<local_wide_decimal_type>(static_cast<local_wide_decimal_type>(static_cast<unsigned>(UINT16_C(314))) / static_cast<unsigned>(UINT8_C(100))), n_exp2);

    const auto flt_as_dec = local_wide_decimal_type { flt };

    const auto ratio = dec / flt_as_dec;

    using std::fabs;

    const auto delta = fabs(1 - ratio);

    const auto result_flt_is_ok = (delta < local_wide_decimal_type { static_cast<float>(0.1L) } );

    result_is_ok = (result_flt_is_ok && result_is_ok);
  }

  return result_is_ok;
}

} // namespace test_decwide_t_algebra_edge

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::test_decwide_t_algebra_edge____() -> bool // NOLINT(readability-identifier-naming,bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
#else
auto test_decwide_t_algebra_edge____() -> bool // NOLINT(readability-identifier-naming,bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
#endif
{
  auto result_is_ok = true;

  result_is_ok = (test_decwide_t_algebra_edge::test_div_by_other_sign_same               () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_mul_by_one_or_one_minus              () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_div_by_one_or_one_minus              () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_zero_operations              () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_one_operations               () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_min_max_operations           () && result_is_ok);
  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  result_is_ok = (test_decwide_t_algebra_edge::test_frexp_in_all_ranges                  () && result_is_ok);
  #endif
  result_is_ok = (test_decwide_t_algebra_edge::test_string_ops_and_round_trips           () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_rootn                        () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_to_native_float_and_back<float>      () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_to_native_float_and_back<double>     () && result_is_ok);
  #if !defined(DECWIDE_T_REDUCE_TEST_DEPTH)
  result_is_ok = (test_decwide_t_algebra_edge::test_to_native_float_and_back<long double>() && result_is_ok);
  #endif
  result_is_ok = (test_decwide_t_algebra_edge::test_various_int_operations<std::uint16_t>() && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_int_operations<std::uint32_t>() && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_int_operations<std::uint64_t>() && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_odds_and_ends                        () && result_is_ok);

  return result_is_ok;
}

auto test_decwide_t_algebra_edge::local_zero    () -> const local_wide_decimal_type& { static const local_wide_decimal_type my_zero(0U); return my_zero; }
auto test_decwide_t_algebra_edge::local_one     () -> const local_wide_decimal_type& { static const local_wide_decimal_type my_one (1U); return my_one; }
auto test_decwide_t_algebra_edge::local_near_one() -> const local_wide_decimal_type& { static const local_wide_decimal_type my_near_one("0.999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"); return my_near_one; }
auto test_decwide_t_algebra_edge::local_not_one () -> const local_wide_decimal_type& { static const local_wide_decimal_type my_not_one ("0.899999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"); return my_not_one; }

#if (defined(__GNUC__) && !defined(__clang__) && (__GNUC__ >= 12))
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
