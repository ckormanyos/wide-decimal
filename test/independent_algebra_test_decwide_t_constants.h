///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_CONSTANTS_2022_02_11_H // NOLINT(llvm-header-guard)
  #define INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_CONSTANTS_2022_02_11_H

  #include <algorithm>
  #include <cmath>
  #include <cstdint>
  #include <limits>

  WIDE_DECIMAL_NAMESPACE_BEGIN

  #if(__cplusplus >= 201703L)
  namespace test::independent_algebra {
  #else
  namespace test { namespace independent_algebra { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  template<typename FloatingPointType>
  struct constants;

  template<typename FloatingPointType>
  struct constants
  {
  private:
    using floating_point_type = FloatingPointType;

    struct initializer
    {
      // Static data initializer
      initializer()
      {
        constants::my_value_pi    ();
        constants::my_value_ln_two();
      }

      initializer(const initializer&) = delete;
      initializer(initializer&&) = delete;

      auto operator=(const initializer&) -> initializer& = delete;
      auto operator=(initializer&&) noexcept -> initializer& = delete;

      ~initializer() = default; // LCOV_EXCL_LINE

      // LCOV_EXCL_START
      auto do_nothing() const -> void
      {
        // Do nothing on purpose.
      }
      // LCOV_EXCL_STOP
    };

    static initializer init; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  public:
    static auto my_value_pi() -> const floating_point_type&
    {
      init.do_nothing();
      static const floating_point_type val(constants<floating_point_type>::calc_pi());
      return val;
    }

    static auto my_value_ln_two() -> const floating_point_type&
    {
      init.do_nothing();
      static const floating_point_type val(constants<floating_point_type>::calc_ln_two());
      return val;
    }

  private:
    static auto calc_pi() -> floating_point_type
    {
      // Use a quadratically convergent Gauss AGM to compute pi.

      floating_point_type val_pi;

      floating_point_type a(1U);

      // Initialize bB to 0.5.
      floating_point_type bB(0.5F); // NOLINT(readability-identifier-naming,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      // Initialize t to 0.375.
      floating_point_type t(static_cast<floating_point_type>(3U) / 8U); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      floating_point_type s(bB);

      // This loop is designed for a maximum of several million
      // decimal digits of pi. The index k should reach no higher
      // than about 25 or 30. After about 20 iterations, the precision
      // is about one million decimal digits.

      const auto digits10_iteration_goal =
        static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::digits10 / 2)
          + static_cast<std::uint32_t>(9U)
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
        a      /= 2U;
        val_pi  = a;
        val_pi *= a;
        bB      = val_pi;
        bB     -= t;
        bB     *= 2U;

        floating_point_type iterate_term(bB);

        iterate_term -= val_pi;
        iterate_term *= static_cast<unsigned long long>(1ULL << (k + 1U)); // NOLINT(google-runtime-int)

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
            static_cast<std::uint64_t>(static_cast<std::uint64_t>(ib) * digits10_scale) / UINT32_C(1000)
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
        t /= 4U;
      }

      return (val_pi + bB) / s;
    }

    static auto calc_ln_two() -> floating_point_type
    {
      // Use an AGM method to compute log(2).

      // Set a0 = 1
      // Set b0 = 1 / (2^(m-2))

      floating_point_type ak(1U);

      constexpr auto n_times_factor =
        static_cast<float>
        (
          static_cast<float>(std::numeric_limits<floating_point_type>::digits10) * 1.67F
        );

      // Ensure that the resulting power is non-negative.
      // Also enforce that m >= 3.
      const auto m =
        (std::max)
        (
          static_cast<std::int32_t>(n_times_factor),
          static_cast<std::int32_t>(3)
        );

      floating_point_type bk =
        ldexp(floating_point_type(1U), static_cast<std::int32_t>(2 - m));

      const auto digits10_iteration_goal =
        static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(std::numeric_limits<floating_point_type>::digits10 / 2)
          + static_cast<std::uint32_t>(9U)
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

      for(auto   k = static_cast<std::int32_t>(INT32_C(0));
                 k < static_cast<std::int32_t>(INT32_C(64));
               ++k)
      {
        using std::ilogb;

        // Check for the number of significant digits to be
        // at least half of the requested digits. If at least
        // half of the requested digits have been achieved,
        // then break after the upcoming iteration.

        const auto ib = (std::max)(static_cast<std::int32_t>(0),
                                   static_cast<std::int32_t>(-ilogb(ak - bk)));

        const auto digits10_of_iteration =
          static_cast<std::uint32_t>
          (
            static_cast<std::uint64_t>(static_cast<std::uint64_t>(ib) * digits10_scale) / UINT32_C(1000)
          );

        const floating_point_type ak_tmp(ak);

        ak += bk;

        if(digits10_of_iteration > digits10_iteration_goal)
        {
          break;
        }

        using std::sqrt;

        ak /= 2;
        bk *= ak_tmp;
        bk  = sqrt(bk);
      }

      // We are now finished with the AGM iteration for log(2).
      // Note at this time that (ak = bk) = AGM(...)
      // Retrieve the value of pi and divide by (a * (2 * m)).

      return my_value_pi() / (ak * m);
    }
  };

  template<typename FloatingPointType>
  typename constants<FloatingPointType>::initializer constants<FloatingPointType>::init; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cert-err58-cpp)

  #if(__cplusplus >= 201703L)
  } // namespace test::independent_algebra
  #else
  } // namespace independent_algebra
  } // namespace test
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_CONSTANTS_2022_02_11_H
