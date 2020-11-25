///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <array>
#include <cstdint>
#include <initializer_list>

#include <math/wide_decimal/decwide_t.h>

namespace
{
  template<typename UnsignedIntegralType>
  constexpr std::initializer_list<UnsignedIntegralType> const_sqrt_control();

  template<>
  constexpr std::initializer_list<std::uint16_t> const_sqrt_control()
  {
    return
    {
      std::uint16_t(35ULL),
      std::uint16_t(1363ULL), std::uint16_t( 600ULL),
      std::uint16_t(9596ULL), std::uint16_t(3986ULL),
      std::uint16_t(6393ULL), std::uint16_t(3384ULL),
      std::uint16_t(6404ULL), std::uint16_t(1805ULL),
      std::uint16_t(5759ULL), std::uint16_t(7515ULL),
      std::uint16_t(1828ULL), std::uint16_t(7169ULL),
      std::uint16_t(3145ULL), std::uint16_t(2816ULL),
      std::uint16_t(5976ULL), std::uint16_t(1647ULL),
      std::uint16_t(1771ULL), std::uint16_t( 895ULL),
      std::uint16_t(4528ULL), std::uint16_t(9092ULL),
      std::uint16_t(8635ULL), std::uint16_t( 312ULL),
      std::uint16_t(1913ULL), std::uint16_t(2220ULL)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint32_t> const_sqrt_control()
  {
    return
    {
      std::uint32_t(35ULL),
      std::uint32_t(13630600ULL),
      std::uint32_t(95963986ULL),
      std::uint32_t(63933384ULL),
      std::uint32_t(64041805ULL),
      std::uint32_t(57597515ULL),
      std::uint32_t(18287169ULL),
      std::uint32_t(31452816ULL),
      std::uint32_t(59761647ULL),
      std::uint32_t(17710895ULL),
      std::uint32_t(45289092ULL),
      std::uint32_t(86350312ULL),
      std::uint32_t(19132220ULL)
    };
  }
}

bool math::wide_decimal::example001_sqrt()
{
  using local_limb_type = std::uint16_t;

  using dec101_t = math::wide_decimal::decwide_t<101U, local_limb_type>;

  const dec101_t a(1234U);
  const dec101_t b(dec101_t(56U) / 100U);

  const dec101_t s = sqrt(a + b);

  const bool result_is_ok = std::equal(s.crepresentation().cbegin(),
                                       s.crepresentation().cbegin() + const_sqrt_control<local_limb_type>().size(),
                                       const_sqrt_control<local_limb_type>().begin());

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example001_sqrt();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
