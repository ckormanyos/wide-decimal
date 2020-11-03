///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <array>
#include <cstdint>
#include <memory>

#include <math/wide_decimal/decwide_t.h>

bool math::wide_decimal::example001_sqrt()
{
  using local_limb_type = std::uint16_t;

  using dec101_t = math::wide_decimal::decwide_t<101U, local_limb_type, std::allocator<void>, float>;

  const dec101_t a(1234U);
  const dec101_t b(dec101_t(56U) / 100U);

  const dec101_t s = sqrt(a + b);

  constexpr std::array<typename dec101_t::limb_type, 25U> control =
  {
    local_limb_type(35ULL),
    local_limb_type(1363ULL), local_limb_type( 600ULL),
    local_limb_type(9596ULL), local_limb_type(3986ULL),
    local_limb_type(6393ULL), local_limb_type(3384ULL),
    local_limb_type(6404ULL), local_limb_type(1805ULL),
    local_limb_type(5759ULL), local_limb_type(7515ULL),
    local_limb_type(1828ULL), local_limb_type(7169ULL),
    local_limb_type(3145ULL), local_limb_type(2816ULL),
    local_limb_type(5976ULL), local_limb_type(1647ULL),
    local_limb_type(1771ULL), local_limb_type( 895ULL),
    local_limb_type(4528ULL), local_limb_type(9092ULL),
    local_limb_type(8635ULL), local_limb_type( 312ULL),
    local_limb_type(1913ULL), local_limb_type(2220ULL)
  };

  const bool result_is_ok = std::equal(s.crepresentation().cbegin(),
                                       s.crepresentation().cbegin() + control.size(),
                                       control.cbegin());

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
