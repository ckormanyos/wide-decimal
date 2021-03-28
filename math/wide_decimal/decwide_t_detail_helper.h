#ifndef DECWIDE_T_DETAIL_HELPER_2020_10_26_H_
  #define DECWIDE_T_DETAIL_HELPER_2020_10_26_H_

  #include <cstdint>

  namespace math { namespace wide_decimal { namespace detail {

  constexpr std::uint32_t pow2_maker_of_upper_limit(std::uint32_t value, std::size_t p2 = 0U)
  {
    return (value >> p2) != 0U ? pow2_maker_of_upper_limit(value, p2 + 1) : std::uint32_t(1) << p2;
  }

  constexpr std::uint32_t a029747_maker_of_upper_limit(std::uint32_t value)
  {
    // Sloanes's A029747: Numbers of the form 2^k times 1, 3 or 5.
    // m = 140000; Select[Union@Flatten@Outer[Times, {1, 3, 5}, 2^Range[0, Floor[Log2[m]]]], # < m &]
    return value <=     1L ?     1L :
           value <=     2L ?     2L :
           value <=     3L ?     3L :
           value <=     4L ?     4L :
           value <=     5L ?     5L :
           value <=     6L ?     6L :
           value <=     8L ?     8L :
           value <=    10L ?    10L :
           value <=    12L ?    12L :
           value <=    16L ?    16L :
           value <=    20L ?    20L :
           value <=    24L ?    24L :
           value <=    32L ?    32L :
           value <=    40L ?    40L :
           value <=    48L ?    48L :
           value <=    64L ?    64L :
           value <=    80L ?    80L :
           value <=    96L ?    96L :
           value <=   128L ?   128L :
           value <=   160L ?   160L :
           value <=   192L ?   192L :
           value <=   256L ?   256L :
           value <=   320L ?   320L :
           value <=   384L ?   384L :
           value <=   512L ?   512L :
           value <=   640L ?   640L :
           value <=   768L ?   768L :
           value <=  1024L ?  1024L :
           value <=  1280L ?  1280L :
           value <=  1536L ?  1536L :
           value <=  2048L ?  2048L :
           value <=  2560L ?  2560L :
           value <=  3072L ?  3072L :
           value <=  4096L ?  4096L :
           value <=  5120L ?  5120L :
           value <=  6144L ?  6144L :
           value <=  8192L ?  8192L :
           value <= 10240L ? 10240L :
           value <= 12288L ? 12288L :
           value <= 16384L ? 16384L :
           value <= 20480L ? 20480L :
           value <= 24576L ? 24576L :
           value <= 32768L ? 32768L :
           value <= 40960L ? 40960L :
           value <= 49152L ? 49152L :
           value <= 65536L ? 65536L :
           value <= 81920L ? 81920L :
           value <= 98304L ? 98304L : 131072L;
  }

  constexpr std::uint32_t pow10_maker(std::uint32_t n)
  {
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

  } } } // namespace math::wide_decimal::detail

#endif // DECWIDE_T_DETAIL_HELPER_2020_10_26_H_
