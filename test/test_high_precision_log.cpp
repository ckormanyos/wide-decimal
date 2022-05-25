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
// When using g++ and -std=c++11
// g++ -finline-functions -march=native -mtune=native -O3 -Wall -Wextra -std=c++11 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_79_0 test/test_high_precision_log.cpp -pthread -lpthread -lgmp -lmpfr -o test_high_precision_log.exe
// ./test_high_precision_log.exe

namespace test_high_precision_log
{
  constexpr auto my_digits10 = static_cast<std::int32_t>(INT32_C(1000011));

  using local_boost_high_precision_backend_type = boost::multiprecision::mpfr_float_backend<my_digits10, boost::multiprecision::allocate_dynamic>;
  using local_boost_high_precision_number_type  = boost::multiprecision::number<local_boost_high_precision_backend_type, boost::multiprecision::et_off>;

  using local_wide_decimal_high_precision_number_type = ckormanyos::math::wide_decimal::decwide_t<my_digits10, std::uint32_t, std::allocator<void>>;
} // namespace test_high_precision_log

namespace local
{
  constexpr auto adders_array_size = static_cast<std::size_t>(UINT8_C(8));

  using adders_array_type = std::array<std::uint64_t, adders_array_size>;

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

  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[0U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[1U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[2U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[3U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[4U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[5U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[6U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");
  static_assert(static_cast<unsigned>(static_cast<std::uint64_t>(adders[7U] + UINT64_C(3)) % 3U) == 0U, "Error adder must provie fractional part 1/3 in this test");

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

    using local_size_type = typename adders_array_type::size_type;

    my_concurrency::parallel_for
    (
      static_cast<local_size_type>(0U),
      static_cast<local_size_type>(local::adders_array_size),
      [&result_is_ok, &do_calcs_log_lock, &tol](typename adders_array_type::size_type index)
      {
        const auto one_plus_adder_at_index = static_cast<std::uint64_t>(UINT64_C(1) + adders[index]);

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

  const auto duration =
    static_cast<float>
    (
        static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count())
      / 1000.0F
    );

  std::cout << "High-precision calculations took: " << std::fixed << std::setprecision(1) << duration << "s" << std::endl;

  return (result_is_ok ? 0 : -1);
}
