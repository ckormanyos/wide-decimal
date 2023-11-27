///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example001e_algebra_and_constexpr() -> bool
#else
auto ::math::wide_decimal::example001e_algebra_and_constexpr() -> bool
#endif
{
  using local_limb_type = std::uint32_t;

  constexpr auto wide_decimal_digits10 = static_cast<std::int32_t>(INT32_C(100));

  #if defined(WIDE_DECIMAL_NAMESPACE)
  using wide_decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, void>;
  #else
  using wide_decimal_type = ::math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, void>;
  #endif

  auto result_is_ok = true;

  WIDE_DECIMAL_CONSTEXPR wide_decimal_type local_quarter_a(wide_decimal_type(static_cast<unsigned>(UINT8_C(25))) / static_cast<unsigned>(UINT8_C(100)));
  WIDE_DECIMAL_CONSTEXPR wide_decimal_type local_quarter_b(static_cast<float>(0.25L)); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  WIDE_DECIMAL_CONSTEXPR auto result_construct_is_ok = (local_quarter_a == local_quarter_b);

  result_is_ok = (result_construct_is_ok && result_is_ok);

  #if (defined(WIDE_DECIMAL_CONSTEXPR_IS_COMPILE_TIME_CONST) && (WIDE_DECIMAL_CONSTEXPR_IS_COMPILE_TIME_CONST != 0))
  static_assert(result_construct_is_ok, "Error: Constexpr construction of built-in float is incorrect");
  #endif

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE001E_ALGEBRA_AND_CONSTEXPR)

#include <iomanip>
#include <iostream>

auto main() -> int
{
  const auto result_is_ok = ::math::wide_decimal::example001e_algebra_and_constexpr();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
