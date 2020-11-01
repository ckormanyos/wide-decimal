///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2009 - 2020.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

#ifndef UTIL_LEXICAL_CAST_2009_11_24_H_
  #define UTIL_LEXICAL_CAST_2009_11_24_H_

  #include <sstream>
  #include <string>

  namespace Util
  {
    template<typename T> inline std::string lexical_cast(const T& t)
    {
      std::stringstream ss;

      ss << t;

      return ss.str();
    }
  }

#endif // UTIL_LEXICAL_CAST_2009_11_24_H_
