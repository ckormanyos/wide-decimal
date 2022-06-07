///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstdint>
#include <random>
#include <string>

#include <math/wide_decimal/decwide_t.h>
#include <test/test_decwide_t_algebra.h>
#include <util/utility/util_baselexical_cast.h>

namespace test_decwide_t_algebra_edge {

using local_limb_type = std::uint32_t;

constexpr std::int32_t local_wide_decimal_digits10 = INT32_C(51);

#if defined(WIDE_DECIMAL_NAMESPACE)
using local_wide_decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<local_wide_decimal_digits10, local_limb_type>;
#else
using local_wide_decimal_type = ::math::wide_decimal::decwide_t<local_wide_decimal_digits10, local_limb_type, void>;
#endif

std::uniform_int_distribution<std::uint32_t> dst_sgn(UINT32_C(0), UINT32_C(1));     // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
std::uniform_int_distribution<std::uint32_t> dst_dig(UINT32_C(1), UINT32_C(9));     // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
std::uniform_int_distribution<std::uint32_t> dst_exp(UINT32_C(0), UINT32_C(10000)); // NOLINT(cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)

using eng_sgn_type = std::ranlux24;
using eng_dig_type = std::minstd_rand0;
using eng_exp_type = std::mt19937;

eng_sgn_type eng_sgn; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
eng_dig_type eng_dig; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
eng_dig_type eng_exp; // NOLINT(cert-msc32-c,cert-msc51-cpp,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)

auto generate_wide_decimal_value(bool is_positive = false) -> local_wide_decimal_type
{
  std::string
    str_x
    (
      static_cast<std::size_t>(std::numeric_limits<local_wide_decimal_type>::digits10 - 2), '0'
    );

  std::generate(str_x.begin(),
                str_x.end(),
                []() // NOLINT(modernize-use-trailing-return-type,-warnings-as-errors)
                {
                  const auto dig = dst_dig(eng_dig);

                  const auto next_char =
                    static_cast<char>
                    (
                      dig + static_cast<std::uint32_t>(UINT32_C(0x30))
                    );

                  return next_char;
                });

  const auto val_exp = dst_exp(eng_exp);

  const auto sgn_exp = (dst_sgn(eng_sgn) == static_cast<std::uint32_t>(UINT32_C(1)));

  str_x.insert(str_x.length(), static_cast<std::size_t>(1U), 'E');

  char pstr_exp[static_cast<std::size_t>(UINT8_C(32))] = { '\0' }; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

  pstr_exp[0U] = static_cast<char>(sgn_exp ? '-' : '+');

  const char* p_end = util::baselexical_cast(val_exp, &pstr_exp[1U]); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

  {
    auto offs = str_x.length();

    str_x.insert(offs, static_cast<std::size_t>(1U), 'E');

    for(auto p = static_cast<const char*>(pstr_exp); p != p_end; ++p) // NOLINT(llvm-qualified-auto,readability-qualified-auto)
    {
      offs = str_x.length();

      str_x.insert(offs, static_cast<std::size_t>(1U), *p);
    }
  }

  auto generated_result = local_wide_decimal_type(local_wide_decimal_type(str_x.c_str()));

  if(!is_positive)
  {
    const auto sgn_left = (dst_sgn(eng_sgn) == static_cast<std::uint32_t>(UINT32_C(1)));

    if(sgn_left)
    {
      generated_result.negate();
    }
  }

  return generated_result;
}

auto test_div_by_other_sign_same() -> bool
{
  auto result_is_ok = true;

  eng_sgn.seed(static_cast<typename eng_sgn_type::result_type>(std::random_device{ }()));
  eng_dig.seed(static_cast<typename eng_dig_type::result_type>(std::random_device{ }()));

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(1024));
           ++i)
  {
    local_wide_decimal_type left = generate_wide_decimal_value();

    const local_wide_decimal_type right = -left;

    const local_wide_decimal_type result = left / right;

    const bool div_is_ok = (-result == 1);

    result_is_ok = (div_is_ok && result_is_ok);
  }

  return result_is_ok;
}

auto test_various_zero_operations() -> bool
{
  const local_wide_decimal_type local_zero(0U);
  const local_wide_decimal_type local_zero_as_pow_to_raise(0U);

  auto result_is_ok = true;

  const local_wide_decimal_type zero_raised_to_the_zero = pow(local_zero, local_zero_as_pow_to_raise);

  const auto result_zero_raised_to_the_zero = (zero_raised_to_the_zero == 1);

  result_is_ok = (result_zero_raised_to_the_zero && result_is_ok);

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(128));
           ++i)
  {
    const local_wide_decimal_type x          = generate_wide_decimal_value(true);
    const local_wide_decimal_type x_pow_zero = pow(x, local_zero);

    result_is_ok = ((x_pow_zero == 1) && result_is_ok);
  }

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(128));
           ++i)
  {
    const local_wide_decimal_type x          = generate_wide_decimal_value();
    const local_wide_decimal_type x_div_zero = x / local_zero;

    result_is_ok = ((x_div_zero == 0) && result_is_ok);
  }

  return result_is_ok;
}

auto test_various_one_operations() -> bool
{
  const local_wide_decimal_type local_one(1U);

  auto result_is_ok = true;

  for(auto   i = static_cast<unsigned>(UINT32_C(0));
             i < static_cast<unsigned>(UINT32_C(128));
           ++i)
  {
    const local_wide_decimal_type x         = generate_wide_decimal_value();
    const local_wide_decimal_type x_div_one = x / local_one;

    result_is_ok = ((x_div_one == x) && result_is_ok);
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

  result_is_ok = (test_decwide_t_algebra_edge::test_div_by_other_sign_same () && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_zero_operations() && result_is_ok);
  result_is_ok = (test_decwide_t_algebra_edge::test_various_one_operations () && result_is_ok);

  return result_is_ok;
}