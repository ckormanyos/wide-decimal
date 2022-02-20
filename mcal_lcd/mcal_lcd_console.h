#ifndef MCAL_LCD_CONSOLE_2020_06_10_H
  #define MCAL_LCD_CONSOLE_2020_06_10_H

  #include <iostream>
  #include <string>

  #include <mcal_lcd/mcal_lcd_base.h>

  #if(__cplusplus >= 201703L)
  namespace mcal::lcd {
  #else
  namespace mcal { namespace lcd { // NOLINT(modernize-concat-nested-namespaces)
  #endif

  class lcd_console : public mcal::lcd::lcd_base
  {
  public:
    lcd_console() = default;

    virtual ~lcd_console() = default;

    virtual auto write(const char*             pstr,
                             std::uint_fast8_t length,
                             std::uint_fast8_t line_index) -> bool
    {
      static_cast<void>(line_index);

      bool write_is_ok;

      if((pstr != nullptr) && (length > 0U))
      {
        const std::string str(pstr, pstr + length);

        std::cout << str << std::endl;

        write_is_ok = true;
      }
      else
      {
        write_is_ok = false;
      }

      return write_is_ok;
    }

    virtual auto init() -> bool { return true; }
  };

  #if(__cplusplus >= 201703L)
  } // namespace mcal::lcd
  #else
  } // namespace lcd
  } // namespace mcal
  #endif

#endif // MCAL_LCD_CONSOLE_2020_06_10_H
