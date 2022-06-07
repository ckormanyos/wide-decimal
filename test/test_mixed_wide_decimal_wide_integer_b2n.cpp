///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <utility>

#define WIDE_INTEGER_NAMESPACE ckormanyos
#define WIDE_DECIMAL_NAMESPACE ckormanyos
#define WIDE_INTEGER_DISABLE_IMPLEMENT_UTIL_DYNAMIC_ARRAY

#include <math/wide_decimal/decwide_t.h>
#include <math/wide_integer/uintwide_t.h>

#include <examples/example_decwide_t.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/utility/util_dynamic_array.h>

namespace example_mixed_wide_decimal_wide_integer
{
  constexpr auto wide_decimal_digits10 = INT32_C(101);
  constexpr auto wide_decimal_digits_2 = static_cast<std::int32_t>((INT32_C(101) * INT32_C(1000)) / INT32_C(301));

  constexpr auto wide_integer_bits =
    static_cast<WIDE_INTEGER_NAMESPACE::math::wide_integer::size_t>
    (
      (
           static_cast<int>(wide_decimal_digits_2 / 32)
        + (static_cast<int>(wide_decimal_digits_2 % 32) != 0 ? 1 : 0)
      )
      * 32
    );

  using wide_decimal_type = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::decwide_t<wide_decimal_digits10, std::uint32_t, std::allocator<void>>;
  using wide_integer_type = WIDE_INTEGER_NAMESPACE::math::wide_integer::uintwide_t<wide_integer_bits, std::uint32_t, std::allocator<void>, true>;

  template<typename FloatingPointType>
  auto pi() -> FloatingPointType
  {
    return static_cast<FloatingPointType>(3.1415926535897932384626433832795029L); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  template<>
  auto pi() -> wide_decimal_type
  {
    return WIDE_DECIMAL_NAMESPACE::math::wide_decimal::pi<wide_decimal_digits10>();
  }

  constexpr auto b2n_max = static_cast<std::size_t>(UINT8_C(102));

  using wide_integer_b2n_array_type = std::array<wide_integer_type, b2n_max>;

  // Numerator[BernoulliB[2*Range[0, 50]]]
  const wide_integer_b2n_array_type b2n_numerators =
  {
    typename wide_integer_b2n_array_type::value_type("1"),
    typename wide_integer_b2n_array_type::value_type("1"),
    typename wide_integer_b2n_array_type::value_type("-1"),
    typename wide_integer_b2n_array_type::value_type("1"),
    typename wide_integer_b2n_array_type::value_type("-1"),
    typename wide_integer_b2n_array_type::value_type("5"),
    typename wide_integer_b2n_array_type::value_type("-691"),
    typename wide_integer_b2n_array_type::value_type("7"),
    typename wide_integer_b2n_array_type::value_type("-3617"),
    typename wide_integer_b2n_array_type::value_type("43867"),
    typename wide_integer_b2n_array_type::value_type("-174611"),
    typename wide_integer_b2n_array_type::value_type("854513"),
    typename wide_integer_b2n_array_type::value_type("-236364091"),
    typename wide_integer_b2n_array_type::value_type("8553103"),
    typename wide_integer_b2n_array_type::value_type("-23749461029"),
    typename wide_integer_b2n_array_type::value_type("8615841276005"),
    typename wide_integer_b2n_array_type::value_type("-7709321041217"),
    typename wide_integer_b2n_array_type::value_type("2577687858367"),
    typename wide_integer_b2n_array_type::value_type("-26315271553053477373"),
    typename wide_integer_b2n_array_type::value_type("2929993913841559"),
    typename wide_integer_b2n_array_type::value_type("-261082718496449122051"),
    typename wide_integer_b2n_array_type::value_type("1520097643918070802691"),
    typename wide_integer_b2n_array_type::value_type("-27833269579301024235023"),
    typename wide_integer_b2n_array_type::value_type("596451111593912163277961"),
    typename wide_integer_b2n_array_type::value_type("-5609403368997817686249127547"),
    typename wide_integer_b2n_array_type::value_type("495057205241079648212477525"),
    typename wide_integer_b2n_array_type::value_type("-801165718135489957347924991853"),
    typename wide_integer_b2n_array_type::value_type("29149963634884862421418123812691"),
    typename wide_integer_b2n_array_type::value_type("-2479392929313226753685415739663229"),
    typename wide_integer_b2n_array_type::value_type("84483613348880041862046775994036021"),
    typename wide_integer_b2n_array_type::value_type("-1215233140483755572040304994079820246041491"),
    typename wide_integer_b2n_array_type::value_type("12300585434086858541953039857403386151"),
    typename wide_integer_b2n_array_type::value_type("-106783830147866529886385444979142647942017"),
    typename wide_integer_b2n_array_type::value_type("1472600022126335654051619428551932342241899101"),
    typename wide_integer_b2n_array_type::value_type("-78773130858718728141909149208474606244347001"),
    typename wide_integer_b2n_array_type::value_type("1505381347333367003803076567377857208511438160235"),
    typename wide_integer_b2n_array_type::value_type("-5827954961669944110438277244641067365282488301844260429"),
    typename wide_integer_b2n_array_type::value_type("34152417289221168014330073731472635186688307783087"),
    typename wide_integer_b2n_array_type::value_type("-24655088825935372707687196040585199904365267828865801"),
    typename wide_integer_b2n_array_type::value_type("414846365575400828295179035549542073492199375372400483487"),
    typename wide_integer_b2n_array_type::value_type("-4603784299479457646935574969019046849794257872751288919656867"),
    typename wide_integer_b2n_array_type::value_type("1677014149185145836823154509786269900207736027570253414881613"),
    typename wide_integer_b2n_array_type::value_type("-2024576195935290360231131160111731009989917391198090877281083932477"),
    typename wide_integer_b2n_array_type::value_type("660714619417678653573847847426261496277830686653388931761996983"),
    typename wide_integer_b2n_array_type::value_type("-1311426488674017507995511424019311843345750275572028644296919890574047"),
    typename wide_integer_b2n_array_type::value_type("1179057279021082799884123351249215083775254949669647116231545215727922535"),
    typename wide_integer_b2n_array_type::value_type("-1295585948207537527989427828538576749659341483719435143023316326829946247"),
    typename wide_integer_b2n_array_type::value_type("1220813806579744469607301679413201203958508415202696621436215105284649447"),
    typename wide_integer_b2n_array_type::value_type("-211600449597266513097597728109824233673043954389060234150638733420050668349987259"),
    typename wide_integer_b2n_array_type::value_type("67908260672905495624051117546403605607342195728504487509073961249992947058239"),
    typename wide_integer_b2n_array_type::value_type("-94598037819122125295227433069493721872702841533066936133385696204311395415197247711")
  };

  // Denominator[BernoulliB[2*Range[0, 50]]]
  const wide_integer_b2n_array_type b2n_denominators =
  {
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        1))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       30))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       42))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       30))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       66))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     2730))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      510))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      798))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      330))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      138))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     2730))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      870))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    14322))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      510))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(  1919190))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    13530))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     1806))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      690))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      282))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    46410))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       66))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     1590))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      798))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      870))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      354))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C( 56786730))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      510))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    64722))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       30))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     4686))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(140100870))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(       30))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     3318))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(   230010))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(      498))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(  3404310))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    61410))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(   272118))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(     1410))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(  4501770))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(        6))),
    typename wide_integer_b2n_array_type::value_type(static_cast<std::int32_t>(INT32_C(    33330))),
  };

  wide_decimal_type to_string(const wide_integer_type& x)
  {
    std::stringstream strm;

    strm << x;

    return wide_decimal_type(strm.str().c_str());
  }

  auto bernoulli_table() -> util::dynamic_array<wide_decimal_type>&
  {
    static util::dynamic_array<wide_decimal_type> bernoulli_table(std::tuple_size<wide_integer_b2n_array_type>::value);

    return bernoulli_table;
  }

  template<typename FloatingPointType>
  auto bernoulli_b(FloatingPointType* bn, std::uint32_t n) -> void
  {
    using floating_point_type = FloatingPointType;

    // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
    // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
    // Cambridge University Press, 2010, p. 237.

    const auto m = static_cast<std::uint32_t>(n / 2U);

    util::dynamic_array<floating_point_type> tangent_numbers(m + 1U);

    tangent_numbers[0U] = 0U;
    tangent_numbers[1U] = 1U;

    for(std::uint32_t k = 1U; k < m; ++k)
    {
      tangent_numbers[k + 1U] = tangent_numbers[k] * k;
    }

    for(auto k = static_cast<std::uint32_t>(2U); k <= m; ++k)
    {
      for(auto j = k; j <= m; ++j)
      {
        const std::uint32_t j_minus_k = j - k;

        tangent_numbers[j] =   (tangent_numbers[j - 1] *  j_minus_k)
                             + (tangent_numbers[j]     * (j_minus_k + 2U));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(std::uint32_t i = 1U; i < static_cast<std::uint32_t>(n / 2U); ++i)
    {
      const auto two_i = static_cast<std::uint32_t>(2U * i);

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const auto b_neg = ((two_i % 4U) == 0U);

      bn[two_i] = ((!b_neg) ? b : -b); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

      two_pow_two_m *= 4U;
    }

    bn[0U] =  1U;                          // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    bn[1U] = floating_point_type(-1) / 2U; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
} // namespace example_mixed_wide_decimal_wide_integer

auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example008_bernoulli_tgamma() -> bool
{
  using namespace example_mixed_wide_decimal_wide_integer;

  // Initialize the table of Bernoulli numbers from a tangent number calculation
  // These can be compared with: Table[N[BernoulliB[n], 101], {n, 0, 100, 2}]

  bernoulli_b
  (
    bernoulli_table().data(),
    static_cast<std::uint32_t>
    (
      bernoulli_table().size()
    )
  );

  bool result_is_ok = true;

  const wide_decimal_type tol = std::numeric_limits<wide_decimal_type>::epsilon() * 100U;

  for(auto i = static_cast<std::size_t>(0U);
           i < bernoulli_table().size();
           i = static_cast<std::size_t>(i + static_cast<std::size_t>(UINT8_C(2))))
  {
    const auto i2n = static_cast<std::size_t>(i / 2U);

    // Convert the Bernoulli numerators and denominators
    // (which are tabulated as wide-integers) to wide-decimals.

    wide_decimal_type b2n =
      wide_decimal_type
      (
          wide_decimal_type(to_string(b2n_numerators[i2n]))
        / static_cast<std::int32_t>(b2n_denominators[i2n])
      );

    using std::fabs;

    // Compare the tabulated and converted Bernoulli numbers
    // with those computed from tangent numbers.

    const auto delta = fabs(1 - fabs(b2n / bernoulli_table().at(i)));

    result_is_ok = ((delta < tol) && result_is_ok);
  }

  return result_is_ok;
}

auto main() -> int
{
  const auto result_is_ok = WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example008_bernoulli_tgamma();

  const auto flg = std::cout.flags();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  std::cout.flags(flg);

  return (result_is_ok ? 0 : -1);
}
