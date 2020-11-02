#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

bool math::wide_decimal::example002_pi()
{
  using local_limb_type = std::uint16_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>;

  math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi = math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>();

  constexpr std::array<local_limb_type, 15U> control_head =
  {{
    local_limb_type(3ULL),
    local_limb_type(1415ULL), local_limb_type(9265ULL),
    local_limb_type(3589ULL), local_limb_type(7932ULL),
    local_limb_type(3846ULL), local_limb_type(2643ULL),
    local_limb_type(3832ULL), local_limb_type(7950ULL),
    local_limb_type(2884ULL), local_limb_type(1971ULL),
    local_limb_type(6939ULL), local_limb_type(9375ULL),
    local_limb_type(1058ULL), local_limb_type(2097ULL)
  }};

  constexpr std::array<local_limb_type, 16U> control_tail =
  {{
    local_limb_type(2087ULL), local_limb_type(5424ULL),
    local_limb_type(5059ULL), local_limb_type(8956ULL),
    local_limb_type(7879ULL), local_limb_type(6130ULL),
    local_limb_type(3311ULL), local_limb_type(6462ULL),
    local_limb_type(8399ULL), local_limb_type(6346ULL),
    local_limb_type(4604ULL), local_limb_type(2209ULL),
    local_limb_type( 106ULL), local_limb_type(1057ULL),
    local_limb_type(7945ULL), local_limb_type(8151ULL)
  }};

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + control_head.size(),
                                     control_head.cbegin());

  const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + 250001UL - control_tail.size(),
                                     my_pi.crepresentation().cbegin() + 250001UL,
                                     control_tail.cbegin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example002_pi();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
