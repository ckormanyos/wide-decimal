///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <math/wide_decimal/decwide_t.h>

bool math::wide_decimal::example006_logarithm()
{
  // Compute 1,000 values of Log[(123456789/1000000) * (3^n)],
  // the result of which is Log[(123456789/1000000)] + (n Log[3])

  using dec1001_t = math::wide_decimal::decwide_t<1001U>;

  const dec1001_t control_base
  {
    "4."
    "8158912082037439290859846198570189641070729092477641189851225392867818091922368962401331896527129054"
    "2666982451156682861982679524527470166196254928165458284131035443816736211730436451521894844085101906"
    "0636084466017164897727191862876944629168307811518342680833354831758201699290992192035440727861285590"
    "8000194558288545181435034121332042612342464441366245658708127241516875453381972256430977100477994492"
    "1474658096051794033615830363454630975474185840919830130502452555134885705490664737858885196366894765"
    "6951094462223789705853191026178076148292450697704283854638855304180978799038854077251637293739120576"
    "0521703221737573691529408083457723228782448834154143092019701535755380818666246605093528537268510024"
    "5327578738401674847141550739942174920396318884548859759526811168922714652063318571945848520663995099"
    "2043823208979562358069992851979820083605474605201139892476772979368945875254998083845420172215964725"
    "7405172125930111781652611052213816835641831608143730061196534241354604509454101590453678456875749885"
    "9"
  };

  dec1001_t x = dec1001_t(UINT32_C(123456789)) / UINT32_C(1000000);

  const dec1001_t ln3 = log(dec1001_t(3U));
  const dec1001_t tol = dec1001_t(std::numeric_limits<dec1001_t>::epsilon() * 10);

  bool result_is_ok = true;

  const std::clock_t start = std::clock();

  for(unsigned i = 0U; i < 1000U; ++i)
  {
    const dec1001_t lg = log(x);

    const dec1001_t control = control_base + (ln3 * i);

    const dec1001_t closeness = fabs(1 - (lg / control));

    result_is_ok &= (closeness < tol);

    x *= 3U;
  }

  const std::clock_t stop = std::clock();

  std::cout << "Time example006_logarithm(): "
            << (float) (stop - start) / (float) CLOCKS_PER_SEC
            << std::endl;

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example006_logarithm();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
