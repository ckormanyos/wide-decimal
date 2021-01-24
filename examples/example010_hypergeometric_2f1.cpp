///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2018 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

#include <math/wide_decimal/decwide_t.h>

namespace local
{
  template<typename T>
  T hypergeometric_2f1(const T& AP, const T& BP, const T& CP, const T& ZM)
  {
    // Implement a rational approximation of hypergeometric_2f1.
    // This C++11 code uses a computational scheme similar to
    // one shown in Y.L. Luke, "Algorithms for the Comuptation
    // of Mathematical Functions", Academic Press, New York, 1977.
    // The original Fortran77 source can be found in the subroutine
    // R2F1() on pages 167-168. Slight corrections are made here
    // in order to repair one or two type-setting errors in the
    // original publication.

    // Luke's original Fortran77 and algorithmic work are fascinating
    // since they are generic and scalable --- decades ahead of their time.
    // Here, in fact, the calculation is scaled to 1,001 decimal digits
    // of precision.

    // Retain some stylistic elements and comments from Luke's
    // original Fortran77 coding style.

    const T Z(-ZM);

    const T my_zero(0U);
    const T my_one (1U);

    // C
    // C INITIALIZATION :
    // C

          T SABZ((AP + BP) * Z);
    const T AB   (AP * BP);
    const T ABZ  (AB * Z);
    const T ABZ1((Z + ABZ) + SABZ);
    const T ABZ2((ABZ1 + SABZ) + (3U * Z));

    std::array<T, 4U> A;
    std::array<T, 4U> B;

    B[0U] = my_one;
    A[0U] = my_one;

    const T CP1(CP + my_one);

    B[1U] = my_one + (ABZ1 / (CP * 2U));
    A[1U] = B[1U]  - (ABZ  / CP);

    const T CT1(2U * CP1);

    B[2U] = my_one + ((ABZ2 / CT1) * (my_one + ABZ1 / ((-T(6U)) + (CT1 * 3U))));
    A[2U] = B[2U]  - ((ABZ  / CP)  * (my_one + ((ABZ2 - ABZ1) / CT1)));

    SABZ /= 4U;

    const T Z2(Z / 2U);

    std::array<T, 9U> D;

    D[0U] = (((T(7U) / 2U) - AB) * Z2) - SABZ;
    D[1U] = ABZ1 / 4U;
    D[2U] = D[1U] - (SABZ * 2U);
    D[3U] = CP1 + my_one;
    D[4U] = CP1 * D[3U];
    D[5U] = CP  * D[4U];
    D[6U] = T(3U) / 2U;
    D[7U] = T(3U) / 4U;
    D[8U] = D[7U] * Z;

    std::array<T, 3U> G;

    // C
    // C FOR I=3,...,N , THE VALUES A(I) AND B(I) ARE CALCULATED
    // C USING THE RECURRENCE RELATIONS BELOW.
    // C

    for(std::uint_fast16_t XI = UINT16_C(3); XI < UINT16_C(10000); ++XI)
    {
      G[2U]  = (D[2U] * D[1U]) / (D[7U] * D[5U]);
      D[1U] += (D[8U] + SABZ);
      D[2U] += (D[8U] - SABZ);
      G[2U] *= (D[1U] / D[6U]);
      G[0U]  =  my_one + ((D[1U] + D[0U]) / (D[6U] * D[3U]));
      G[1U]  =  D[1U] / (D[4U] * D[6U]);
      D[7U] += (D[6U] * 2U);
      ++D[6U];
      G[1U] *= ((CP1 - XI) - ((D[2U] + D[0U]) / D[6U]));

      // C -----------------------------------------------------------------
      // C THE RECURRENCE RELATIONS FOR A(I) and B(I) ARE AS FOLLOWS
      // C -----------------------------------------------------------------

      A[3U] = std::inner_product(G.crbegin(), G.crend(), A.cbegin(), my_zero);
      B[3U] = std::inner_product(G.crbegin(), G.crend(), B.cbegin(), my_zero);

      // Check if the iteration difference (delta) is within
      // tolerance and break from the recursion if it is.
      // Here we analyze the difference between this iteration
      // result and the previous iteration result using:
      //
      //         |     |(A2/B2)]| |
      // delta = | 1 - |--------| |
      //         |     |(A3/B3) | |
      //
      //         |     |(A2*B3)| |
      //       = | 1 - |-------| |,
      //         |     |(A3*B2)| |
      //
      // where the absolute value of the ratio in the second term
      // is used and the absolute value of delta is used.

      using std::fabs;

      const T ratio = fabs((A[2U] * B[3U]) / (A[3U] * B[2U]));
      const T delta = fabs(my_one - ratio);

      if((XI > UINT16_C(7)) && (delta < std::numeric_limits<T>::epsilon()))
      {
        break;
      }

      // Shift the arrays for the next recursion.
      std::copy(A.cbegin() + 1U, A.cend(), A.begin());
      std::copy(B.cbegin() + 1U, B.cend(), B.begin());

      D[8U] +=  Z2;
      D[0U] += (D[8U] * 2U);
      D[5U] += (D[4U] * 3U);
      D[4U] += (D[3U] * 2U);

      ++D[3U];
    }

    // Return the rational approximation. This is given
    // by the ratio of the final recursions of A and B.
    return A.back() / B.back();
  }
}

bool math::wide_decimal::example010_hypergeometric_2f1()
{
  using dec1001_t = math::wide_decimal::decwide_t<1001U>;

  const dec1001_t a( dec1001_t(2U) / 3U);
  const dec1001_t b( dec1001_t(4U) / 3U);
  const dec1001_t c( dec1001_t(5U) / 7U);
  const dec1001_t z(-dec1001_t(3U) / 4U);

  const dec1001_t h2f1 = local::hypergeometric_2f1(a, b, c, z);

  // N[Hypergeometric2F1[2/3, 4/3, 5/7, -3/4], 1003]
  const dec1001_t control
  {
    "0."
    "5010047360876106403820298707781130663700964458315280453481414661084883532272790762746959490255023246"
    "0312226542605539888036608718209715889951504568390329053796255363287603231434476268734389439044422167"
    "5284089490545688340381318820728928965614986405528607006166515996158859902644742923187058504872539864"
    "5526393970602257621956446639141677360052376900435972099485022668952455616438857995487419477325979436"
    "0624373454823168201440969762998535751649093599185576558711187071798391688855988414416119107109432755"
    "7650459245296569373944014705113399144656127270454166592212540118941954333081250095816519510598056279"
    "9701330083670272177694982980992777101459836814865935861581023713422564394525179768543448930391133399"
    "2365965485619664248076371405092957089253100001866455395914231855187336610490908971056693326326421884"
    "7958097660733256147292859976308158801627280967329287950929961899242779950084734217898887067896870114"
    "6800382971696052755503110505432490729940028678881070548295642016866992748303933000057149684544171490"
    "241"
  };

  using std::fabs;

  const dec1001_t closeness = fabs(1 - (h2f1 / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example010_hypergeometric_2f1();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
