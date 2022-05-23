///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#if !defined(WIDE_DECIMAL_NAMESPACE)
#define WIDE_DECIMAL_NAMESPACE ckormanyos
#endif

#include <boost/multiprecision/mpfr.hpp>

#include <math/wide_decimal/decwide_t.h>
#include <test/parallel_for.h>

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++ and -std=c++11
// g++ -finline-functions -march=native -mtune=native -O3 -Wall -Wextra -std=c++11 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_79_0 test/test_high_precision_exp.cpp -pthread -lpthread -lgmp -lmpfr -o test_high_precision_exp.exe
// ./test_high_precision_exp.exe

namespace test_high_precision_exp
{
  constexpr auto my_digits10 = static_cast<std::int32_t>(INT32_C(100011));

  using local_boost_high_precision_backend_type = boost::multiprecision::mpfr_float_backend<my_digits10, boost::multiprecision::allocate_dynamic>;
  using local_boost_high_precision_number_type  = boost::multiprecision::number<local_boost_high_precision_backend_type, boost::multiprecision::et_off>;

  using local_wide_decimal_high_precision_number_type = ckormanyos::math::wide_decimal::decwide_t<my_digits10, std::uint32_t, std::allocator<void>>;
} // namespace test_high_precision_exp

namespace local
{
  constexpr auto str_arg_array_size = static_cast<std::size_t>(UINT8_C(6));

  using str_arg_array_type = std::array<std::string, str_arg_array_size>;

  const str_arg_array_type str_args =
  {
    std::string("-123.4"),
    std::string("-12.34"),
    std::string("-1.234"),
    std::string("1.234"),
    std::string("12.34"),
    std::string("123.4")
  };

  template<typename HighPrecisionFloatLeftType,
           typename HighPrecisionFloatRightType>
  static auto do_calcs_exp() -> bool
  {
    using local_hp_float_left_type  = HighPrecisionFloatLeftType;
    using local_hp_float_right_type = HighPrecisionFloatRightType;

    const auto tol =
      local_hp_float_left_type
      (
          std::numeric_limits<local_hp_float_left_type>::epsilon()
        * static_cast<std::uint32_t>(UINT32_C(100000000))
      );

    auto result_is_ok = true;

    std::atomic_flag do_calcs_exp_lock = ATOMIC_FLAG_INIT;

    using local_size_type = typename str_arg_array_type::size_type;

    my_concurrency::parallel_for
    (
      static_cast<local_size_type>(0U),
      static_cast<local_size_type>(local::str_arg_array_size),
      [&result_is_ok, &do_calcs_exp_lock, &tol](typename str_arg_array_type::size_type index)
      {
        const auto x_left  = local_hp_float_left_type (str_args[index].c_str());
        const auto x_right = local_hp_float_right_type(str_args[index].c_str());

        using std::exp;

        const auto y_left  = exp(x_left);
        const auto y_right = exp(x_right);

        using std::fabs;

        std::stringstream strm;

        strm << std::setprecision(test_high_precision_exp::my_digits10) << y_right;

//std::cout << std::setprecision(test_high_precision_exp::my_digits10) << y_left  << std::endl << std::endl << std::endl;
//std::cout << std::setprecision(test_high_precision_exp::my_digits10) << y_right << std::endl << std::endl << std::endl;

        const local_hp_float_left_type y_ctrl(strm.str().c_str());

        const auto delta = fabs(1 - (y_left / y_ctrl));

        while(do_calcs_exp_lock.test_and_set()) { ; }
        const auto is_close_fraction = (delta < tol);

        result_is_ok = (is_close_fraction && result_is_ok);

        std::cout << "index: "
                  << index
                  << ", is_close_fraction: "
                  << std::boolalpha
                  << is_close_fraction
                  << std::endl;
        do_calcs_exp_lock.clear();
      }
    );

    return result_is_ok;
  }
} // namespace local

auto main() -> int
{
  using local_hp_float_left_type  = test_high_precision_exp::local_wide_decimal_high_precision_number_type;
  using local_hp_float_right_type = test_high_precision_exp::local_boost_high_precision_number_type;

  const auto start = std::chrono::high_resolution_clock::now();

  const auto result_is_ok =
    local::do_calcs_exp<local_hp_float_left_type, local_hp_float_right_type>();

  const auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  const auto duration =
    static_cast<float>
    (
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())
      / 1000.0F
    );

  std::cout << "High-precision calculations took: " << std::fixed << std::setprecision(1) << duration << "s" << std::endl;

  return (result_is_ok ? 0 : -1);
}
