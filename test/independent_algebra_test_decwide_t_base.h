///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#ifndef INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BASE_2020_10_17_H
  #define INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BASE_2020_10_17_H

  #include <string>

  #include <math/wide_decimal/decwide_t.h>

  WIDE_DECIMAL_NAMESPACE_BEGIN

  namespace test { namespace independent_algebra {

  class independent_algebra_test_decwide_t_base
  {
  public:
    virtual ~independent_algebra_test_decwide_t_base() { }

    virtual void get_string(std::string&) const = 0;

  protected:
    independent_algebra_test_decwide_t_base() { }
  };

  } }

  WIDE_DECIMAL_NAMESPACE_END

#endif // INDEPENDENT_ALGEBRA_TEST_DECWIDE_T_BASE_2020_10_17_H
