///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

// This work is also based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469

#ifndef DECWIDE_T_DETAIL_2020_10_26_H_
  #define DECWIDE_T_DETAIL_2020_10_26_H_

  #include <array>
  #include <cstdint>
  #include <initializer_list>
  #include <memory>

  #include <util/utility/util_dynamic_array.h>

  namespace math { namespace wide_decimal { namespace detail {

  constexpr std::uint32_t pow2_maker_of_upper_limit(std::uint32_t value, std::size_t p2 = 0U)
  {
    // Calculate the nearest power of 2 larger than or equal to the input value.
    return (value >> p2) != 0U ? pow2_maker_of_upper_limit(value, p2 + 1) : std::uint32_t(1) << p2;
  }

  constexpr std::uint32_t a029747_maker_of_upper_limit(std::uint32_t value)
  {
    // Sloanes's A029747: Numbers of the form 2^k times 1, 3 or 5.
    // m = 140000; Select[Union@Flatten@Outer[Times, {1, 3, 5}, 2^Range[0, Floor[Log2[m]]]], # < m &]
    // In addition, crop the list to begin with 32 or higher.
    return value <=    32UL ?    32UL :
           value <=    40UL ?    40UL :
           value <=    48UL ?    48UL :
           value <=    64UL ?    64UL :
           value <=    80UL ?    80UL :
           value <=    96UL ?    96UL :
           value <=   128UL ?   128UL :
           value <=   160UL ?   160UL :
           value <=   192UL ?   192UL :
           value <=   256UL ?   256UL :
           value <=   320UL ?   320UL :
           value <=   384UL ?   384UL :
           value <=   512UL ?   512UL :
           value <=   640UL ?   640UL :
           value <=   768UL ?   768UL :
           value <=  1024UL ?  1024UL :
           value <=  1280UL ?  1280UL :
           value <=  1536UL ?  1536UL :
           value <=  2048UL ?  2048UL :
           value <=  2560UL ?  2560UL :
           value <=  3072UL ?  3072UL :
           value <=  4096UL ?  4096UL :
           value <=  5120UL ?  5120UL :
           value <=  6144UL ?  6144UL :
           value <=  8192UL ?  8192UL :
           value <= 10240UL ? 10240UL :
           value <= 12288UL ? 12288UL :
           value <= 16384UL ? 16384UL :
           value <= 20480UL ? 20480UL :
           value <= 24576UL ? 24576UL :
           value <= 32768UL ? 32768UL :
           value <= 40960UL ? 40960UL :
           value <= 49152UL ? 49152UL :
           value <= 65536UL ? 65536UL :
           value <= 81920UL ? 81920UL :
           value <= 98304UL ? 98304UL : 131072UL;
  }

  constexpr std::uint32_t pow10_maker(std::uint32_t n)
  {
    // Make the constant power of 10^n.
    return ((n == UINT32_C(0)) ? UINT32_C(1) : pow10_maker(n - UINT32_C(1)) * UINT32_C(10));
  }

  template<const std::int32_t MyDigits10,
           typename LimbType>
  struct decwide_t_helper
  {
    static constexpr std::int32_t digits10          = MyDigits10;
    static constexpr std::int32_t digits            = digits10;
    static constexpr std::int32_t max_digits10      = static_cast<std::int32_t>(digits10 + 1);
    static constexpr std::int32_t radix             = static_cast<std::int32_t>(10);

    static constexpr std::int32_t elem_digits10     =
      ((std::is_same<LimbType, std::uint32_t>::value == true)
        ? static_cast<std::int32_t>(8)
        : ((std::is_same<LimbType, std::uint16_t>::value == true) ? static_cast<std::int32_t>(4)
                                                                  : static_cast<std::int32_t>(2)));

    static constexpr std::int32_t elem_number_extra = 3;
    static constexpr std::int32_t elem_number       = static_cast<std::int32_t>(((digits10 / elem_digits10) + (((digits10 % elem_digits10) != 0) ? 1 : 0)) + elem_number_extra);
    static constexpr std::int32_t elem_mask         = static_cast<std::int32_t>(pow10_maker((std::uint32_t)  elem_digits10));
    static constexpr std::int32_t elem_mask_half    = static_cast<std::int32_t>(pow10_maker((std::uint32_t) (elem_digits10 / 2)));
  };

  template <typename MyType,
            const std::uint_fast32_t MySize,
            typename MyAlloc>
  class fixed_dynamic_array final : public util::dynamic_array<MyType, MyAlloc>
  {
  private:
    using base_class_type = util::dynamic_array<MyType, MyAlloc>;

  public:
    constexpr fixed_dynamic_array()
      : base_class_type(MySize) { }

    fixed_dynamic_array(const typename base_class_type::size_type       s,
                        const typename base_class_type::value_type&     v = typename base_class_type::value_type(),
                        const typename base_class_type::allocator_type& a = typename base_class_type::allocator_type())
      : base_class_type(MySize, typename base_class_type::value_type(), a)
    {
      std::fill(base_class_type::begin(),
                base_class_type::begin() + (std::min)(MySize, (std::uint_fast32_t) s),
                v);
    }

    constexpr fixed_dynamic_array(const fixed_dynamic_array& other_array)
      : base_class_type((const base_class_type&) other_array) { }

    explicit fixed_dynamic_array(std::initializer_list<typename base_class_type::value_type> lst)
      : base_class_type(MySize)
    {
      std::copy(lst.begin(),
                lst.begin() + (std::min)((std::uint_fast32_t) lst.size(), MySize),
                base_class_type::begin());
    }

    constexpr fixed_dynamic_array(fixed_dynamic_array&& other_array)
      : base_class_type((base_class_type&&) other_array) { }

    fixed_dynamic_array& operator=(const fixed_dynamic_array& other_array)
    {
      base_class_type::operator=((const base_class_type&) other_array);

      return *this;
    }

    fixed_dynamic_array& operator=(fixed_dynamic_array&& other_array)
    {
      base_class_type::operator=((base_class_type&&) other_array);

      return *this;
    }

    virtual ~fixed_dynamic_array() { }

    static constexpr typename base_class_type::size_type static_size()
    {
      return MySize;
    }
  };

  template<typename MyType,
           const std::uint_fast32_t MySize>
  class fixed_static_array final : public std::array<MyType, MySize>
  {
  private:
    using base_class_type = std::array<MyType, MySize>;

  public:
    fixed_static_array() { }

    fixed_static_array(const typename base_class_type::size_type   s,
                       const typename base_class_type::value_type& v = typename base_class_type::value_type())
    {
      std::fill(base_class_type::begin(),
                base_class_type::begin() + (std::min)(MySize, (std::uint_fast32_t) s),
                v);

      std::fill(base_class_type::begin() + (std::min)(MySize, (std::uint_fast32_t) s),
                base_class_type::end(),
                typename base_class_type::value_type());
    }

    constexpr fixed_static_array(const fixed_static_array& other_array)
      : base_class_type(static_cast<const base_class_type&>(other_array)) { }

    template<const std::uint_fast32_t OtherSize>
    fixed_static_array(const fixed_static_array<std::uint_fast32_t, OtherSize>& other_array)
    {
      std::copy(other_array.cbegin(),
                other_array.cbegin() + (std::min)(OtherSize, MySize),
                base_class_type::begin());

      std::fill(base_class_type::begin() + (std::min)(OtherSize, MySize),
                base_class_type::end(),
                typename base_class_type::value_type());
    }

    explicit fixed_static_array(std::initializer_list<typename base_class_type::value_type> lst)
    {
      std::copy(lst.begin(),
                lst.begin() + (std::min)((std::uint_fast32_t) lst.size(), MySize),
                base_class_type::begin());

      std::fill(base_class_type::begin() + (std::min)((std::uint_fast32_t) lst.size(), MySize),
                base_class_type::end(),
                typename base_class_type::value_type());
    }

    constexpr fixed_static_array(fixed_static_array&& other_array)
      : base_class_type(static_cast<base_class_type&&>(other_array)) { }

    fixed_static_array& operator=(const fixed_static_array& other_array)
    {
      base_class_type::operator=((const base_class_type&) other_array);

      return *this;
    }

    fixed_static_array& operator=(fixed_static_array&& other_array)
    {
      base_class_type::operator=((base_class_type&&) other_array);

      return *this;
    }

    ~fixed_static_array() { }

    static constexpr typename base_class_type::size_type static_size()
    {
      return MySize;
    }
  };

  typedef enum enum_os_float_field_type
  {
    os_float_field_scientific,
    os_float_field_fixed,
    os_float_field_none
  }
  os_float_field_type;

  } } } // namespace math::wide_decimal::detail

#endif // DECWIDE_T_DETAIL_2020_10_26_H_
