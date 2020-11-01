#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

bool math::wide_decimal::example002_pi()
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 18U>;

  math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi = math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>();

  constexpr std::array<local_limb_type, 8U> control_head =
  {{
    local_limb_type(3ULL),
    local_limb_type(14159265ULL),
    local_limb_type(35897932ULL),
    local_limb_type(38462643ULL),
    local_limb_type(38327950ULL),
    local_limb_type(28841971ULL),
    local_limb_type(69399375ULL),
    local_limb_type(10582097ULL)
  }};

  constexpr std::array<local_limb_type, 8U> control_tail =
  {{
    local_limb_type(20875424ULL),
    local_limb_type(50598956ULL),
    local_limb_type(78796130ULL),
    local_limb_type(33116462ULL),
    local_limb_type(83996346ULL),
    local_limb_type(46042209ULL),
    local_limb_type( 1061057ULL),
    local_limb_type(79458151ULL)
  }};

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + control_head.size(),
                                     control_head.cbegin());

  const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + 125001UL - 8UL,
                                     my_pi.crepresentation().cbegin() + 125001UL,
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
