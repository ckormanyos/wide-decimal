///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H // NOLINT(llvm-header-guard)
  #define CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H

  #include <array>
  #include <cstddef>
  #include <cstdint>

  #include <math/wide_decimal/decwide_t_detail_namespace.h>

  WIDE_DECIMAL_NAMESPACE_BEGIN

  #if(__cplusplus >= 201703L)
  namespace math::constants {
  #else
  namespace math { namespace constants { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  constexpr std::array<std::uint32_t, 7U + 1U> const_pi_control_head_32
  {
    // head std::uint32_t
    static_cast<std::uint32_t>(UINT32_C(3)),
    static_cast<std::uint32_t>(UINT32_C(14159265)),
    static_cast<std::uint32_t>(UINT32_C(35897932)),
    static_cast<std::uint32_t>(UINT32_C(38462643)),
    static_cast<std::uint32_t>(UINT32_C(38327950)),
    static_cast<std::uint32_t>(UINT32_C(28841971)),
    static_cast<std::uint32_t>(UINT32_C(69399375)),
    static_cast<std::uint32_t>(UINT32_C(10582097))
  };

  constexpr std::array<std::uint16_t, 14U + 1U> const_pi_control_head_16
  {
    // head std::uint16_t
    static_cast<std::uint16_t>(UINT16_C(3)),
    static_cast<std::uint16_t>(UINT16_C(1415)), static_cast<std::uint16_t>(UINT16_C(9265)),
    static_cast<std::uint16_t>(UINT16_C(3589)), static_cast<std::uint16_t>(UINT16_C(7932)),
    static_cast<std::uint16_t>(UINT16_C(3846)), static_cast<std::uint16_t>(UINT16_C(2643)),
    static_cast<std::uint16_t>(UINT16_C(3832)), static_cast<std::uint16_t>(UINT16_C(7950)),
    static_cast<std::uint16_t>(UINT16_C(2884)), static_cast<std::uint16_t>(UINT16_C(1971)),
    static_cast<std::uint16_t>(UINT16_C(6939)), static_cast<std::uint16_t>(UINT16_C(9375)),
    static_cast<std::uint16_t>(UINT16_C(1058)), static_cast<std::uint16_t>(UINT16_C(2097))
  };

  constexpr std::array<std::uint8_t, 28U + 1U> const_pi_control_head_08
  {
    // head std::uint8_t
    static_cast<std::uint8_t>(UINT8_C(3)),
    static_cast<std::uint8_t>(UINT8_C(14)), static_cast<std::uint8_t>(UINT8_C(15)), static_cast<std::uint8_t>(UINT8_C(92)), static_cast<std::uint8_t>(UINT8_C(65)),
    static_cast<std::uint8_t>(UINT8_C(35)), static_cast<std::uint8_t>(UINT8_C(89)), static_cast<std::uint8_t>(UINT8_C(79)), static_cast<std::uint8_t>(UINT8_C(32)),
    static_cast<std::uint8_t>(UINT8_C(38)), static_cast<std::uint8_t>(UINT8_C(46)), static_cast<std::uint8_t>(UINT8_C(26)), static_cast<std::uint8_t>(UINT8_C(43)),
    static_cast<std::uint8_t>(UINT8_C(38)), static_cast<std::uint8_t>(UINT8_C(32)), static_cast<std::uint8_t>(UINT8_C(79)), static_cast<std::uint8_t>(UINT8_C(50)),
    static_cast<std::uint8_t>(UINT8_C(28)), static_cast<std::uint8_t>(UINT8_C(84)), static_cast<std::uint8_t>(UINT8_C(19)), static_cast<std::uint8_t>(UINT8_C(71)),
    static_cast<std::uint8_t>(UINT8_C(69)), static_cast<std::uint8_t>(UINT8_C(39)), static_cast<std::uint8_t>(UINT8_C(93)), static_cast<std::uint8_t>(UINT8_C(75)),
    static_cast<std::uint8_t>(UINT8_C(10)), static_cast<std::uint8_t>(UINT8_C(58)), static_cast<std::uint8_t>(UINT8_C(20)), static_cast<std::uint8_t>(UINT8_C(97))
  };

  constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_10001
  {
    // tail 1 + 10^4 std::uint32_t
    static_cast<std::uint32_t>(UINT32_C(29552498)),
    static_cast<std::uint32_t>(UINT32_C(87275846)),
    static_cast<std::uint32_t>(UINT32_C(10126483)),
    static_cast<std::uint32_t>(UINT32_C(69998922)),
    static_cast<std::uint32_t>(UINT32_C(56959688)),
    static_cast<std::uint32_t>(UINT32_C(15920560)),
    static_cast<std::uint32_t>(UINT32_C( 1016552)),
    static_cast<std::uint32_t>(UINT32_C(56375678))
  };

  constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_100001
  {
    // tail 1 + 10^5 std::uint32_t
    static_cast<std::uint32_t>(UINT32_C(38043299)),
    static_cast<std::uint32_t>(UINT32_C(70695770)),
    static_cast<std::uint32_t>(UINT32_C(15078933)),
    static_cast<std::uint32_t>(UINT32_C(77286580)),
    static_cast<std::uint32_t>(UINT32_C(35712790)),
    static_cast<std::uint32_t>(UINT32_C(91376742)),
    static_cast<std::uint32_t>(UINT32_C( 8056554)),
    static_cast<std::uint32_t>(UINT32_C(93624646))
  };

  constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_1000001
  {
    // tail 1 + 10^6 std::uint32_t
    static_cast<std::uint32_t>(UINT32_C(20875424)),
    static_cast<std::uint32_t>(UINT32_C(50598956)),
    static_cast<std::uint32_t>(UINT32_C(78796130)),
    static_cast<std::uint32_t>(UINT32_C(33116462)),
    static_cast<std::uint32_t>(UINT32_C(83996346)),
    static_cast<std::uint32_t>(UINT32_C(46042209)),
    static_cast<std::uint32_t>(UINT32_C( 1061057)),
    static_cast<std::uint32_t>(UINT32_C(79458151))
  };

  constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_10001
  {
    // tail 1 + 10^4 std::uint16_t
    static_cast<std::uint16_t>(UINT16_C(2955)), static_cast<std::uint16_t>(UINT16_C(2498)),
    static_cast<std::uint16_t>(UINT16_C(8727)), static_cast<std::uint16_t>(UINT16_C(5846)),
    static_cast<std::uint16_t>(UINT16_C(1012)), static_cast<std::uint16_t>(UINT16_C(6483)),
    static_cast<std::uint16_t>(UINT16_C(6999)), static_cast<std::uint16_t>(UINT16_C(8922)),
    static_cast<std::uint16_t>(UINT16_C(5695)), static_cast<std::uint16_t>(UINT16_C(9688)),
    static_cast<std::uint16_t>(UINT16_C(1592)), static_cast<std::uint16_t>(UINT16_C( 560)),
    static_cast<std::uint16_t>(UINT16_C( 101)), static_cast<std::uint16_t>(UINT16_C(6552)),
    static_cast<std::uint16_t>(UINT16_C(5637)), static_cast<std::uint16_t>(UINT16_C(5678))
  };

  constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_100001
  {
    // tail 1 + 10^5 std::uint16_t
    static_cast<std::uint16_t>(UINT16_C(3804)), static_cast<std::uint16_t>(UINT16_C(3299)),
    static_cast<std::uint16_t>(UINT16_C(7069)), static_cast<std::uint16_t>(UINT16_C(5770)),
    static_cast<std::uint16_t>(UINT16_C(1507)), static_cast<std::uint16_t>(UINT16_C(8933)),
    static_cast<std::uint16_t>(UINT16_C(7728)), static_cast<std::uint16_t>(UINT16_C(6580)),
    static_cast<std::uint16_t>(UINT16_C(3571)), static_cast<std::uint16_t>(UINT16_C(2790)),
    static_cast<std::uint16_t>(UINT16_C(9137)), static_cast<std::uint16_t>(UINT16_C(6742)),
    static_cast<std::uint16_t>(UINT16_C( 805)), static_cast<std::uint16_t>(UINT16_C(6554)),
    static_cast<std::uint16_t>(UINT16_C(9362)), static_cast<std::uint16_t>(UINT16_C(4646))
  };

  constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_1000001
  {
    // tail 1 + 10^6 std::uint16_t
    static_cast<std::uint16_t>(UINT16_C(2087)), static_cast<std::uint16_t>(UINT16_C(5424)),
    static_cast<std::uint16_t>(UINT16_C(5059)), static_cast<std::uint16_t>(UINT16_C(8956)),
    static_cast<std::uint16_t>(UINT16_C(7879)), static_cast<std::uint16_t>(UINT16_C(6130)),
    static_cast<std::uint16_t>(UINT16_C(3311)), static_cast<std::uint16_t>(UINT16_C(6462)),
    static_cast<std::uint16_t>(UINT16_C(8399)), static_cast<std::uint16_t>(UINT16_C(6346)),
    static_cast<std::uint16_t>(UINT16_C(4604)), static_cast<std::uint16_t>(UINT16_C(2209)),
    static_cast<std::uint16_t>(UINT16_C( 106)), static_cast<std::uint16_t>(UINT16_C(1057)),
    static_cast<std::uint16_t>(UINT16_C(7945)), static_cast<std::uint16_t>(UINT16_C(8151))
  };



  constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_10001
  {
    // tail 1 + 10^4 std::uint8_t
    static_cast<std::uint8_t>(UINT8_C(29)), static_cast<std::uint8_t>(UINT8_C(55)), static_cast<std::uint8_t>(UINT8_C(24)), static_cast<std::uint8_t>(UINT8_C(98)),
    static_cast<std::uint8_t>(UINT8_C(87)), static_cast<std::uint8_t>(UINT8_C(27)), static_cast<std::uint8_t>(UINT8_C(58)), static_cast<std::uint8_t>(UINT8_C(46)),
    static_cast<std::uint8_t>(UINT8_C(10)), static_cast<std::uint8_t>(UINT8_C(12)), static_cast<std::uint8_t>(UINT8_C(64)), static_cast<std::uint8_t>(UINT8_C(83)),
    static_cast<std::uint8_t>(UINT8_C(69)), static_cast<std::uint8_t>(UINT8_C(99)), static_cast<std::uint8_t>(UINT8_C(89)), static_cast<std::uint8_t>(UINT8_C(22)),
    static_cast<std::uint8_t>(UINT8_C(56)), static_cast<std::uint8_t>(UINT8_C(95)), static_cast<std::uint8_t>(UINT8_C(96)), static_cast<std::uint8_t>(UINT8_C(88)),
    static_cast<std::uint8_t>(UINT8_C(15)), static_cast<std::uint8_t>(UINT8_C(92)), static_cast<std::uint8_t>(UINT8_C( 5)), static_cast<std::uint8_t>(UINT8_C(60)),
    static_cast<std::uint8_t>(UINT8_C( 1)), static_cast<std::uint8_t>(UINT8_C( 1)), static_cast<std::uint8_t>(UINT8_C(65)), static_cast<std::uint8_t>(UINT8_C(52)),
    static_cast<std::uint8_t>(UINT8_C(56)), static_cast<std::uint8_t>(UINT8_C(37)), static_cast<std::uint8_t>(UINT8_C(56)), static_cast<std::uint8_t>(UINT8_C(78))
  };

  constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_100001
  {
    // tail 1 + 10^5 std::uint8_t
    static_cast<std::uint8_t>(UINT8_C(38)), static_cast<std::uint8_t>(UINT8_C( 4)), static_cast<std::uint8_t>(UINT8_C(32)), static_cast<std::uint8_t>(UINT8_C(99)),
    static_cast<std::uint8_t>(UINT8_C(70)), static_cast<std::uint8_t>(UINT8_C(69)), static_cast<std::uint8_t>(UINT8_C(57)), static_cast<std::uint8_t>(UINT8_C(70)),
    static_cast<std::uint8_t>(UINT8_C(15)), static_cast<std::uint8_t>(UINT8_C( 7)), static_cast<std::uint8_t>(UINT8_C(89)), static_cast<std::uint8_t>(UINT8_C(33)),
    static_cast<std::uint8_t>(UINT8_C(77)), static_cast<std::uint8_t>(UINT8_C(28)), static_cast<std::uint8_t>(UINT8_C(65)), static_cast<std::uint8_t>(UINT8_C(80)),
    static_cast<std::uint8_t>(UINT8_C(35)), static_cast<std::uint8_t>(UINT8_C(71)), static_cast<std::uint8_t>(UINT8_C(27)), static_cast<std::uint8_t>(UINT8_C(90)),
    static_cast<std::uint8_t>(UINT8_C(91)), static_cast<std::uint8_t>(UINT8_C(37)), static_cast<std::uint8_t>(UINT8_C(67)), static_cast<std::uint8_t>(UINT8_C(42)),
    static_cast<std::uint8_t>(UINT8_C( 8)), static_cast<std::uint8_t>(UINT8_C( 5)), static_cast<std::uint8_t>(UINT8_C(65)), static_cast<std::uint8_t>(UINT8_C(54)),
    static_cast<std::uint8_t>(UINT8_C(93)), static_cast<std::uint8_t>(UINT8_C(62)), static_cast<std::uint8_t>(UINT8_C(46)), static_cast<std::uint8_t>(UINT8_C(46))
  };

  constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_1000001
  {
    // tail 1 + 10^6 std::uint8_t
    static_cast<std::uint8_t>(UINT8_C(20)), static_cast<std::uint8_t>(UINT8_C(87)), static_cast<std::uint8_t>(UINT8_C(54)), static_cast<std::uint8_t>(UINT8_C(24)),
    static_cast<std::uint8_t>(UINT8_C(50)), static_cast<std::uint8_t>(UINT8_C(59)), static_cast<std::uint8_t>(UINT8_C(89)), static_cast<std::uint8_t>(UINT8_C(56)),
    static_cast<std::uint8_t>(UINT8_C(78)), static_cast<std::uint8_t>(UINT8_C(79)), static_cast<std::uint8_t>(UINT8_C(61)), static_cast<std::uint8_t>(UINT8_C(30)),
    static_cast<std::uint8_t>(UINT8_C(33)), static_cast<std::uint8_t>(UINT8_C(11)), static_cast<std::uint8_t>(UINT8_C(64)), static_cast<std::uint8_t>(UINT8_C(62)),
    static_cast<std::uint8_t>(UINT8_C(83)), static_cast<std::uint8_t>(UINT8_C(99)), static_cast<std::uint8_t>(UINT8_C(63)), static_cast<std::uint8_t>(UINT8_C(46)),
    static_cast<std::uint8_t>(UINT8_C(46)), static_cast<std::uint8_t>(UINT8_C( 4)), static_cast<std::uint8_t>(UINT8_C(22)), static_cast<std::uint8_t>(UINT8_C( 9)),
    static_cast<std::uint8_t>(UINT8_C( 1)), static_cast<std::uint8_t>(UINT8_C( 6)), static_cast<std::uint8_t>(UINT8_C(10)), static_cast<std::uint8_t>(UINT8_C(57)),
    static_cast<std::uint8_t>(UINT8_C(79)), static_cast<std::uint8_t>(UINT8_C(45)), static_cast<std::uint8_t>(UINT8_C(81)), static_cast<std::uint8_t>(UINT8_C(51))
  };

  #if(__cplusplus >= 201703L)
  } // namespace math::constants
  #else
  } // namespace constants
  } // namespace math
  #endif

  WIDE_DECIMAL_NAMESPACE_END

#endif // CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H
