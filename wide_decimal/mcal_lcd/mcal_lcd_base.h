#ifndef MCAL_LCD_BASE_2020_06_10_H_
  #define MCAL_LCD_BASE_2020_06_10_H_

  #include <cstdint>

  #include <util/utility/util_noncopyable.h>

  namespace mcal { namespace lcd {

  class lcd_base : private util::noncopyable
  {
  public:
    virtual ~lcd_base() = default;

    virtual bool init() = 0;

    virtual bool write_n(const char* pstr,
                         const std::uint_fast8_t length,
                         const std::uint_fast8_t line_index) = 0;

  protected:
    lcd_base() = default;
  };

  } } // namespace mcal::lcd

#endif // MCAL_LCD_BASE_2020_06_10_H_
