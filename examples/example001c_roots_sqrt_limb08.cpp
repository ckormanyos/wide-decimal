///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

bool math::wide_decimal::example001c_roots_sqrt_limb08()
{
  using local_limb_type = std::uint8_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 16U>;

  using dec101_t = math::wide_decimal::decwide_t<wide_decimal_digits10,
                                                 local_limb_type,
                                                 local_allocator_type,
                                                 float,
                                                 std::int16_t>;

  const dec101_t s = sqrt(dec101_t(123456U) / 100);

  // N[Sqrt[123456/100], 101]
  const dec101_t control
  {
    "35."
    "1363060095963986639333846404180557597515182871693145281659761647177108954528909286350312191322209780"
    "5376509458612063848933362936036902894775479901536869612529882911098089704769067860255038125371130309"
    "7376476849395654679954112865931807823733220588199157932745047491503972076995050120516343736839242915"
    "4369007794430771608198640304436146239417896958061796560916877079311161817597168274846189811523239304"
    "2642889928069497444785243273980837009588523459749479302460885283605547111733236159315706029981137688"
    "8991852788360379505972332110711893204646089148658294227900893783477868274739342763981945636625577086"
    "9672247622850251850193579637713379900799611913793836060987736238490134981026465200443718457075956217"
    "6553633317360948823009715346637345353199394836821376443690590142905598515413670933316255259528441000"
    "3594505770419599844025682991191472179397790550351313826295588062939873150278838895995158537860944019"
    "1539544776173919125406452516595735890842387785268673310183794490210307597283027587373720292917113922"
    "51"
  };

  const dec101_t closeness = fabs(1 - fabs(s / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec101_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example001c_roots_sqrt_limb08();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
