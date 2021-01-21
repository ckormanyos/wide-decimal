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
  T hypergeometric_1f1(const T& AP, const T& CP, const T& ZM)
  {
    // Implement a rational approximation of hypergeometric_1f1.
    // This C++11 code uses a computational scheme similar to
    // one shown in Y.L. Luke, "Algorithms for the Comuptation
    // of Mathematical Functions", Academic Press, New York, 1977.
    // The original Fortran77 source can be found in the subroutine
    // R1F1() on page 188. Slight corrections are made here
    // in order to repair one or two type-setting errors in the
    // original publication.

    // Luke's original Fortran77 and algorithmic work are fascinating
    // since they are generic and scalable --- decades ahead of their time.
    // Here, in fact, the calculation is scaled to 1,001 decimal digits
    // of precision.

    // Retain some stylistic elements and comments from Luke's
    // original Fortran77 coding style.

    const T Z(-ZM);

    // C
    // C INITIALIZATION :
    // C

    const T Z2(Z / 2U);

    const T my_zero(0U);
    const T my_one (1U);
    const T my_two (2U);

    std::array<T, 4U> A;
    std::array<T, 4U> B;

    {
      const T CT1X((AP * Z) / CP);
      const T CT2X(Z2 / (1U + CP));

      A[0U] = my_one;
      B[0U] = my_one;
      B[1U] = my_one + (((1U + AP) * Z2) / CP);
      A[1U] = B[1U] - CT1X;
      B[2U] = my_one + (((((my_two + B[1U]) * (2U + AP))) * CT2X) / 3U);
      A[2U] = B[2U] - ((my_one + CT2X) * CT1X);
    }

    std::uint_fast16_t CT1 = UINT16_C(3);
    T CT2;

    std::array<std::uint_fast16_t, 4U> N =
    {{
      UINT16_C(3),
      UINT16_C(2),
      UINT16_C(1),
      UINT16_C(0)
    }};

    std::array<T, 3U> G;

    // C
    // C FOR I=3,...,N , THE VALUES A(I) AND B(I) ARE CALCULATED
    // C USING THE RECURRENCE RELATIONS BELOW.
    // C

    const T Z2Z2 = (Z2 * Z2);

    for( ; N[0U] < UINT16_C(10000); ++N[0U])
    {
      // C
      // C CALCULATION OF THE MULTIPLIERS FOR THE RECURSION
      // C

      // Use simplification with computer algebra and integer values
      // where possible to reduce the total work of the following
      // calculations from:
      // 8 multiplications and 7 divisions (8 + (7 * 3)) = 29 to
      // 7 multiplications and 4 divisions (7 + (4 * 3)) = 19,
      // where division is assumed to require 3 times more work
      // than multiplication does.

      CT2   = Z2 / (CT1 * (CP + N[1U]));
      G[0U] = my_one + (CT2 * (N[2U] - AP));
      CT2   =  (CT2 *  (AP + N[1U])) / (CP + N[2U]);
      G[1U] =   CT2 * ((CP - N[1U]) + (((AP + N[0U]) / (CT1 + 2U)) * Z2));
      G[2U] = ((CT2 *  (AP - N[2U])) * ((AP + N[2U]) * (Z2Z2))) / (std::uint32_t(std::uint32_t(CT1 - my_two) * CT1) * (CP + N[3U]));

      // C -----------------------------------------------------------------
      // C THE RECURRENCE RELATIONS FOR A(I) and B(I) ARE AS FOLLOWS
      // C -----------------------------------------------------------------

      A[3U] = std::inner_product(G.crbegin(), G.crend(), A.cbegin(), my_zero);
      B[3U] = std::inner_product(G.crbegin(), G.crend(), B.cbegin(), my_zero);

      std::copy(N.crbegin() + 1U, N.crend(), N.rbegin());

      if(N[3U] > UINT16_C(2))
      {
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

        if(delta < std::numeric_limits<T>::epsilon())
        {
          break;
        }
      }

      // Shift the arrays for the next recursion.
      std::copy(A.cbegin() + 1U, A.cend(), A.begin());
      std::copy(B.cbegin() + 1U, B.cend(), B.begin());

      CT1 += UINT16_C(2);
    }

    // Return the rational approximation. This is given
    // by the ratio of the final recursions of A and B.
    return ((N[0U] < UINT16_C(10000)) ? (A.back() / B.back()) : T(0U));
  }
}

bool math::wide_decimal::example010a_hypergeometric_1f1()
{
  using dec1001_t = math::wide_decimal::decwide_t<1001U>;

  const dec1001_t a( dec1001_t(2U) / 3U);
  const dec1001_t b( dec1001_t(4U) / 3U);
  const dec1001_t z(-dec1001_t(3U) / 4U);

  const dec1001_t h1f1 = local::hypergeometric_1f1(a, b, z);

  // N[Hypergeometric1F1[2/3, 4/3, -3/4], 1003]
  const dec1001_t control
  {
    "0."
    "7081686536671296176070489154441892975263864254727871399151279335156755130218980066854878718659644876"
    "1305121691418288067229656427737120644431158087451536612475482465245551262861209703714089698028175454"
    "0070644713429820469109420541161232888933036256421986450730383363242819718385845578123146346562416912"
    "8459987657984935712740682227830255529693892427453550857117109315771584082876374760460718792291350348"
    "5190554952923571564136649540077735757762129258093268390205343996981829581788815350317949553374856332"
    "4600791123444332864835615846878224843114597006279577350905460171544671226390900903288844703332800883"
    "7461634557128794965583053125950999187778364988189708317696297046872660631251555306433290041457209364"
    "7846449827307910707408292805642488619055129649252986312848264653807329387537374580180299791208841957"
    "4522132120358559031634677139462281867313156223643446521436399244700681185183813207181756167736113034"
    "8578271847519210395098212613256528348855147753328519068248226379214308876937283270363298187389819025"
    "524"
  };

  using std::fabs;

  const dec1001_t closeness = fabs(1 - (h1f1 / control));

  const bool result_is_ok = closeness < (std::numeric_limits<dec1001_t>::epsilon() * 10);

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example010a_hypergeometric_1f1();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
