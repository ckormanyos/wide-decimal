#include <math/wide_decimal/decwide_t.h>
#include <test/independent_algebra_test.h>

namespace
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(10001);
}

bool test_decwide_t_algebra_add_()
{
  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_add_<wide_decimal_digits10, local_limb_type, std::allocator<void>, double, 128U, 4U, test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10, local_limb_type, std::allocator<void>, double>>();

  return result_is_ok;
}

bool test_decwide_t_algebra_sub_()
{
  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_sub_<wide_decimal_digits10, local_limb_type, std::allocator<void>, double, 128U, 4U, test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10, local_limb_type, std::allocator<void>, double>>();

  return result_is_ok;
}

bool test_decwide_t_algebra_mul_()
{
  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_mul_<wide_decimal_digits10, local_limb_type, std::allocator<void>, double, 128U, 4U, test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10, local_limb_type, std::allocator<void>, double>>();

  return result_is_ok;
}

bool test_decwide_t_algebra_div_()
{
  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_div_<wide_decimal_digits10, local_limb_type, std::allocator<void>, double, 128U, 4U, test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10, local_limb_type, std::allocator<void>, double>>();

  return result_is_ok;
}

bool test_decwide_t_algebra_sqrt()
{
  const bool result_is_ok =
    test::independent_algebra::independent_algebra_test_sqrt<wide_decimal_digits10, local_limb_type, std::allocator<void>, double, 128U, 4U, test::independent_algebra::independent_algebra_test_boost_cpp<wide_decimal_digits10, local_limb_type, std::allocator<void>, double>>();

  return result_is_ok;
}
