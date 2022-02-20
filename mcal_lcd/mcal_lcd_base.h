#ifndef MCAL_LCD_BASE_2020_06_10_H
  #define MCAL_LCD_BASE_2020_06_10_H

  #include <cstdint>

  #include <util/utility/util_noncopyable.h>

  #if(__cplusplus >= 201703L)
  namespace mcal::lcd {
  #else
  namespace mcal { namespace lcd { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  class lcd_base : private util::noncopyable // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
  {
  public:
    virtual ~lcd_base() = default;

    virtual auto init() -> bool = 0;

    virtual auto write(const char*             pstr,
                             std::uint_fast8_t length,
                             std::uint_fast8_t line_index) -> bool = 0;

  protected:
    lcd_base() = default;

    virtual auto set_line_index(const std::uint8_t index) -> bool { static_cast<void>(index); return true; }
  };

  #if(__cplusplus >= 201703L)
  } // namespace mcal::lcd
  #else
  } // namespace lcd
  } // namespace mcal
  #endif

#endif // MCAL_LCD_BASE_2020_06_10_H
