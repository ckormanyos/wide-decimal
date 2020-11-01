#include <cstdint>
#include <iomanip>
#include <iostream>
#include <initializer_list>

#include <math/wide_decimal/decwide_t.h>

bool math::wide_decimal::example001_sqrt()
{
  using dec101_t = math::wide_decimal::decwide_t<101U>;

  const dec101_t a(1234U);
  const dec101_t b(dec101_t(56U) / 100U);

  const dec101_t s = sqrt(a + b);

  constexpr std::array<typename dec101_t::limb_type, 13U> control =
  {
    35,
    13630600,
    95963986,
    63933384,
    64041805,
    57597515,
    18287169,
    31452816,
    59761647,
    17710895,
    45289092,
    86350312,
    19132220
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
