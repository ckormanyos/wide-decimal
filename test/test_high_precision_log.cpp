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

#if !defined(WIDE_DECIMAL_NAMESPACE)
#define WIDE_DECIMAL_NAMESPACE ckormanyos
#endif

#include <boost/multiprecision/mpfr.hpp>

#include <math/wide_decimal/decwide_t.h>
#include <test/parallel_for.h>

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++-10 and -std=c++20
// g++-10 -finline-functions -finline-limit=128 -march=native -mtune=native -O3 -Wall -Wextra -std=c++20 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_79_0 test/test_high_precision_log.cpp -pthread -lpthread -lgmp -lmpfr -o test_high_precision_log.exe

namespace test_high_precision_log
{
  constexpr auto my_digits10 = static_cast<std::int32_t>(INT32_C(1000011));

  using local_boost_high_precision_backend_type = boost::multiprecision::mpfr_float_backend<my_digits10, boost::multiprecision::allocate_dynamic>;
  using local_boost_high_precision_number_type  = boost::multiprecision::number<local_boost_high_precision_backend_type, boost::multiprecision::et_off>;

  using local_wide_decimal_high_precision_number_type = ckormanyos::math::wide_decimal::decwide_t<my_digits10, std::uint32_t, std::allocator<void>>;
} // namespace test_high_precision_log

namespace local
{
  using adders_array_type = std::array<std::uint64_t, 8U>;

  constexpr adders_array_type adders =
  {
    UINT64_C(0),
    UINT64_C(3),
    UINT64_C(6),
    UINT64_C(9),
    UINT64_C(12),
    UINT64_C(15),
    UINT64_C(9999),
    UINT64_C(999999999999999999)
  };

  template<typename HighPrecisionFloatLeftType,
           typename HighPrecisionFloatRightType>
  static auto do_calcs_log() -> bool
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

    std::atomic_flag do_calcs_log_lock = ATOMIC_FLAG_INIT;

    my_concurrency::parallel_for
    (
      static_cast<typename adders_array_type::size_type>(0U),
      adders.size(),
      [&result_is_ok, &do_calcs_log_lock, &tol](typename adders_array_type::size_type index)
      {
        const auto one_plus_adder_at_index = static_cast<std::uint64_t>(1U + adders[index]);

        const auto x_left  = local_hp_float_left_type (one_plus_adder_at_index) / 3U;
        const auto x_right = local_hp_float_right_type(one_plus_adder_at_index) / 3U;

        using std::log;

        const auto y_left  = log(x_left);
        const auto y_right = log(x_right);

        using std::fabs;

        std::stringstream strm;

        strm << std::setprecision(test_high_precision_log::my_digits10) << y_right;

        const local_hp_float_left_type y_ctrl(strm.str().c_str());

        const auto delta = fabs(1 - (y_left / y_ctrl));

        while(do_calcs_log_lock.test_and_set()) { ; }
        const auto is_close_fraction = (delta < tol);

        result_is_ok = (is_close_fraction && result_is_ok);

        std::cout << "index: "
                  << index
                  << ", is_close_fraction: "
                  << std::boolalpha
                  << is_close_fraction
                  << std::endl;
        do_calcs_log_lock.clear();
      }
    );

    return result_is_ok;
  }
} // namespace local

auto main() -> int
{
  using local_hp_float_left_type  = test_high_precision_log::local_wide_decimal_high_precision_number_type;
  using local_hp_float_right_type = test_high_precision_log::local_boost_high_precision_number_type;

  const auto start = std::chrono::high_resolution_clock::now();

  const auto result_is_ok =
    local::do_calcs_log<local_hp_float_left_type, local_hp_float_right_type>();

  const auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  using namespace std::literals;

  const auto duration =
    static_cast<double>
    (
        static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())
      / 1000.0
    );

  std::cout << "High-precision calculations took: " << std::fixed << std::setprecision(1) << duration << "s" << std::endl;
}
