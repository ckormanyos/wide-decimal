///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2025.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#if !defined(WIDE_DECIMAL_NAMESPACE)
#define WIDE_DECIMAL_NAMESPACE ckormanyos
#endif

#include <boost/multiprecision/mpfr.hpp>

#include <math/wide_decimal/decwide_t.h>
#include <test/parallel_for.h>
#include <util/utility/util_pseudorandom_time_point_seed.h>

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/wide_decimal
// When using g++ and -std=c++20
// g++ -finline-functions -march=native -mtune=native -O3 -Werror -Wall -Wextra -std=c++20 -DWIDE_DECIMAL_NAMESPACE=ckormanyos -I. -I/mnt/c/boost/boost_1_83_0 test/test_high_precision_exp.cpp -pthread -lpthread -lgmp -lmpfr -o test_high_precision_exp.exe
// ./test_high_precision_exp.exe

namespace test_high_precision_exp
{
  namespace detail
  {
    template<typename FloatingPointType>
    static auto calc_pi() -> FloatingPointType
    {
      using floating_point_type = FloatingPointType;

      // Use a quadratically convergent Gauss AGM to compute pi.

      floating_point_type val_pi;

      floating_point_type a(static_cast<unsigned>(UINT8_C(1)));

      // Initialize bB to 0.5.
      floating_point_type bB(0.5F); // NOLINT(readability-identifier-naming,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      // Initialize t to 0.375.
      floating_point_type
        t
        {
            static_cast<floating_point_type>(static_cast<unsigned>(UINT8_C(3)))
          / static_cast<unsigned>(UINT8_C(8))
        };

      floating_point_type s(bB);

      // This loop is designed for a maximum of several million
      // decimal digits of pi. The index k should reach no higher
      // than about 25 or 30. After about 20 iterations, the precision
      // is about one million decimal digits.

      const auto digits10_iteration_goal =
        static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::digits10 / 2)
          + static_cast<std::uint32_t>(UINT8_C(9))
        );

      using std::log;
      using std::lround;

      const auto digits10_scale =
        static_cast<std::uint32_t>
        (
          lround
          (
            static_cast<float>(1000.0F * log(static_cast<float>(std::numeric_limits<floating_point_type>::radix))) / log(10.0F)
          )
        );

      for(auto   k = static_cast<unsigned>(UINT8_C(0));
                 k < static_cast<unsigned>(UINT8_C(48));
               ++k)
      {
        using std::sqrt;

        a      += sqrt(bB);
        a      /= static_cast<unsigned>(UINT8_C(2));
        val_pi  = a;
        val_pi *= a;
        bB      = val_pi;
        bB     -= t;
        bB     *= static_cast<unsigned>(UINT8_C(2));

        floating_point_type iterate_term(bB);

        const auto left_shift_amount = static_cast<unsigned>(k + static_cast<unsigned>(UINT8_C(1)));

        iterate_term -= val_pi;
        iterate_term *= static_cast<unsigned long long>(static_cast<unsigned long long>(UINT8_C(1)) << left_shift_amount); // NOLINT(google-runtime-int)

        s += iterate_term;

        // Test the number of precise digits from this iteration.
        // If it is there are enough precise digits, then the calculation
        // is finished.
        const auto ib =
          (std::max)
          (
            static_cast<std::int32_t>(INT8_C(0)),
            static_cast<std::int32_t>(-ilogb(iterate_term))
          );

        const auto digits10_of_iteration =
          static_cast<std::uint32_t>
          (
              static_cast<std::uint64_t>(static_cast<std::uint64_t>(ib) * digits10_scale)
            / static_cast<std::uint16_t>(UINT16_C(1000))
          );

        // Estimate the approximate decimal digits of this iteration term.
        // If we have attained about half of the total desired digits
        // with this iteration term, then the calculation is finished
        // because the change from the next iteration will be
        // insignificantly small.

        if(digits10_of_iteration > digits10_iteration_goal)
        {
          break;
        }

        t  = val_pi;
        t += bB;
        t /= static_cast<unsigned>(UINT8_C(4));
      }

      return (val_pi + bB) / s;
    }

    constexpr auto local_digits10() -> std::int32_t { return static_cast<std::int32_t>(INT32_C(100001)); }

    using wide_decimal_type        = ckormanyos::math::wide_decimal::decwide_t<detail::local_digits10()>;
    using boost_float_backend_type = boost::multiprecision::mpfr_float_backend<static_cast<unsigned>(std::numeric_limits<wide_decimal_type>::digits10), boost::multiprecision::allocate_dynamic>;
    using boost_float_type         = boost::multiprecision::number<boost_float_backend_type, boost::multiprecision::et_off>;

    static_assert(local_digits10() == wide_decimal_type::decwide_t_digits10,
                  "Error: Wrong assumption about decwide_t digits10");
  } // namespace detail

  using detail::wide_decimal_type;
  using detail::boost_float_type;

  auto fb_to_fw(const boost_float_type& eb) -> wide_decimal_type
  {
    std::stringstream strm_b;

    strm_b << std::setprecision(std::numeric_limits<boost_float_type>::digits10 + static_cast<int>(INT8_C(3)))
           << eb;

    return wide_decimal_type(strm_b.str().c_str());
  };

  std::uniform_int_distribution<std::uint32_t> dst_sgn(UINT32_C(        0), UINT32_C(        1));
  std::uniform_int_distribution<std::uint32_t> dst_top(UINT32_C( 30000000), UINT32_C(999999999));
  std::uniform_int_distribution<std::uint32_t> dst_bot(UINT32_C(100000000), UINT32_C(999999999));

  using eng_sgn_type = std::ranlux24;
  using eng_top_type = std::mt19937;
  using eng_bot_type = std::minstd_rand0;

  eng_top_type eng_sgn;
  eng_top_type eng_top;
  eng_bot_type eng_bot;

  auto do_test() -> bool
  {
    const auto my_pi_w = ckormanyos::math::wide_decimal::pi<wide_decimal_type::decwide_t_digits10>();
    const auto my_pi_b = detail::calc_pi<boost_float_type>();

    eng_sgn.seed(util::util_pseudorandom_time_point_seed::value<typename eng_top_type::result_type>());
    eng_top.seed(util::util_pseudorandom_time_point_seed::value<typename eng_top_type::result_type>());
    eng_bot.seed(util::util_pseudorandom_time_point_seed::value<typename eng_bot_type::result_type>());

    auto result_is_ok = true;

    using std::ilogb;

    const auto ilogb_tol = ilogb(std::numeric_limits<wide_decimal_type>::epsilon());

    const auto my_one  = wide_decimal_type(static_cast<unsigned>(UINT8_C(1)));

    std::atomic_flag do_calcs_exp_lock = ATOMIC_FLAG_INIT;

    my_concurrency::parallel_for
    (
      static_cast<unsigned>(UINT8_C(0)),
      #if defined(__clang__)
      static_cast<unsigned>(UINT8_C(8)),
      #else
      static_cast<unsigned>(UINT8_C(24)),
      #endif
      [&my_pi_w, &my_pi_b, &result_is_ok, &do_calcs_exp_lock, &ilogb_tol, &my_one](unsigned count)
      {
        while(do_calcs_exp_lock.test_and_set()) { ; }
        const auto sgn_top = (dst_sgn(eng_sgn) == static_cast<std::uint32_t>(UINT32_C(1)));
        const auto sgn_bot = (dst_sgn(eng_sgn) == static_cast<std::uint32_t>(UINT32_C(1)));

        const auto top = dst_top(eng_top);
        const auto bot = dst_bot(eng_bot);

        const auto numer =
          static_cast<std::int32_t>
          (
            (!sgn_top) ? static_cast<std::int32_t>(top) : static_cast<std::int32_t>(-static_cast<std::int32_t>(top))
          );

        const auto denom =
          static_cast<std::int32_t>
          (
            (!sgn_bot) ? static_cast<std::int32_t>(bot) : static_cast<std::int32_t>(-static_cast<std::int32_t>(bot))
          );

        do_calcs_exp_lock.clear();

        const auto yw = wide_decimal_type((wide_decimal_type(numer) * my_pi_w) / denom);
        const auto ew = exp(yw);

        const auto yb = boost_float_type((boost_float_type(numer) * my_pi_b) / denom);
        const auto eb = exp(yb);

        using std::fabs;
        using std::ilogb;

        const auto delta = fabs(my_one - fabs(ew / fb_to_fw(eb)));

        const auto result_exp_is_ok = ((ilogb(delta) <= ilogb_tol) || (delta.iszero()));

        std::stringstream strm;

        strm << "count: "
             << std::setw(static_cast<std::streamsize>(INT8_C(2)))
             << count
             << ", yw: "
             << std::showpos
             << std::scientific
             << std::setprecision(static_cast<std::streamsize>(INT8_C(5)))
             << yw
             << ", ew: "
             << std::showpos
             << std::scientific
             << std::setprecision(static_cast<std::streamsize>(INT8_C(5)))
             << ew
             << ", delta: "
             << std::noshowpos
             << std::scientific
             << std::setprecision(static_cast<std::streamsize>(INT8_C(2)))
             << delta
             << ", result_is_ok: "
             << std::boolalpha
             << result_is_ok;

        const auto str_from_strm = strm.str();

        while(do_calcs_exp_lock.test_and_set()) { ; }
        result_is_ok = (result_is_ok && result_exp_is_ok);

        std::cout << str_from_strm << std::endl;
        do_calcs_exp_lock.clear();
      }
    );

    return result_is_ok;
  }
} // namespace test_high_precision_exp

int main()
{
  const auto begin = std::clock();

  const bool result_is_ok = test_high_precision_exp::do_test();

  const auto end = std::clock();

  const auto elapsed = static_cast<float>(static_cast<float>(end - begin) / CLOCKS_PER_SEC);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  std::cout << "High-precision calculations took: " << std::fixed << std::setprecision(1) << elapsed << "s" << std::endl;

  return (result_is_ok ? 0 : -1);
}
