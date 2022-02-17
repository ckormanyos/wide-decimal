#ifndef MCAL_LCD_BASE_2020_06_10_H
  #define MCAL_LCD_BASE_2020_06_10_H

  #include <cstdint>

  #include <util/utility/util_noncopyable.h>

  #if(__cplusplus >= 201703L)
  namespace mcal::lcd {
  #else
  namespace mcal { namespace lcd { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  class lcd_base : private util::noncopyable
  {
  public:
    virtual ~lcd_base() = default;

    virtual bool init() = 0;

    virtual bool write(const char* pstr,
                       const std::uint_fast8_t length,
                       const std::uint_fast8_t line_index) = 0;

  protected:
    lcd_base() = default;

    virtual bool set_line_index(const std::uint8_t) { return true; }
  };

  #if(__cplusplus >= 201703L)
  } // namespace mcal::lcd
  #else
  } // namespace lcd
  } // namespace mcal
  #endif

#endif // MCAL_LCD_BASE_2020_06_10_H
