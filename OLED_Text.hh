/**
 * @file ?/OLED_Text.hh
 * @version 0.1
 *
 * @section License
 * Copyright (C) 2014-2015, jeditekunum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef COSA_X_OLED_TEXT_HH
#define COSA_X_OLED_TEXT_HH

#include "Cosa/LCD.hh"
#include <Font.h>

#include "OLED_IO.hh"

/**
 * Base class for OLED Text.
 */
class OLED_Text : public LCD::Device {
public:
  /**
   * @override LCD::Device
   * Text display mode.
   */
  enum {
    NORMAL_TEXT_MODE = 0,
    INVERTED_TEXT_MODE = _BV(0),
    UNDERLINED_TEXT_MODE = _BV(1),
    RAW_TEXT_MODE = _BV(7)
  } __attribute__((packed));
  typedef uint8_t TextMode;

  /**
   * Construct OLED_Text OLED connected to given io port handler.
   * Note that these devices only accept data in bands of 8 rows.
   * Therefore each line must start at a %8 boundary.  Some fonts
   * will display less lines than expected.  (For example, a 20
   * high font will only display as 2 lines on a 64 high display.)
   * LINE_SPACING is ignored in these devices.
   * @param[in] io handler.
   * @param[in] font font to display.
   * @param[in] screen_width.
   * @param[in] screen_height.
   * @param[in] column_offset.
   */
  OLED_Text(OLED_IO* io, Font* font,
            uint8_t screen_width, uint8_t screen_height,
            uint8_t column_offset = 0) :
    LCD::Device(),
    m_initialized(false),
    m_io(io),
    m_font(font),
    m_screen_width(screen_width),
    m_column_offset(column_offset),
    m_screen_height(screen_height),
    m_columns(screen_width / (m_font->WIDTH + m_font->SPACING)),
    m_rows(screen_height / (BYTES(m_font->HEIGHT)*8))
  {}

  /**
   * @override LCD::Device
   * Start display for text output. Returns true if successful
   * otherwise false.
   * @return boolean.
   */
  virtual bool begin() = 0;

  /**
   * @override LCD::Device
   * Stop display and power down. Returns true if successful
   * otherwise false.
   */
  virtual bool end()
  {
    display_clear();
    display_off();
    return (true);
  }

  /**
   * @override LCD::Device
   * Set cursor position to given position.
   * @param[in] x.
   * @param[in] y.
   */
  virtual void set_cursor(uint8_t x, uint8_t y)
  {
    if (x >= m_columns) x = 0;
    if (y >= m_rows) y = 0;
    m_x = x;
    m_y = y;
  }

  /**
   * @override IOStream::Device
   * Write character to display. Handles carriage-return-line-feed, back-
   * space, alert, horizontal tab and form-feed. Returns character or EOF
   * on error.
   * @param[in] c character to write.
   * @return character written or EOF(-1).
   */
  virtual int putchar(char c);

  /**
   * Until Cosa LCD::Device has get_text_mode...
   * Get text mode.
   * @return mode.
   */
  TextMode get_text_mode()
    __attribute__((always_inline))
  {
    LCD::Device::TextMode mode = LCD::Device::set_text_mode(LCD::Device::NORMAL_TEXT_MODE);
    set_text_mode(mode);
    return (mode);
  }

  /**
   * @override LCD::Device until it changes
   * Set text mode. Return previous text mode.
   * @param[in] mode new text mode.
   * @return previous text mode.
   */
  TextMode set_text_mode(TextMode mode)
    __attribute__((always_inline))
  {
    return (LCD::Device::set_text_mode((LCD::Device::TextMode)mode));
  }

  /**
   * @override IOStream::Device
   * Write data from buffer with given size to device.
   * @param[in] buf buffer to write.
   * @param[in] size number of bytes to write.
   * @return number of bytes written or EOF(-1).
   */
  virtual int write(const void* buf, size_t size);

  /**
   * Clear to end of line.
   */
  virtual void line_clear()
  {
    uint8_t x = m_x;
    uint8_t y = m_x;
    while (m_x < m_columns) putchar(' ');
    set_cursor(x, y);
  }

  /**
   * Get width in characters.
   */
  uint8_t width()
    __attribute__((always_inline))
  {
    return (m_columns);
  }

  /**
   * Get height in characters.
   */
  uint8_t height()
    __attribute__((always_inline))
  {
    return (m_rows);
  }

protected:
  uint8_t m_initialized;

  /** Display IO. */
  OLED_IO* m_io;

  /** Font. */
  Font* m_font;

  /** Width of display. */
  uint8_t m_screen_width;

  /** Column offset. */
  uint8_t m_column_offset;

  /** Height of display. */
  uint8_t m_screen_height;

  /** Display columns (characters per line). */
  uint8_t m_columns;

  /** Display rows (lines). */
  uint8_t m_rows;

  /**
   * Scripts are a sequence of bytes of the form
   * {command, #args, args...}
   * Command may be a special script control.
   */

  /**
   * Script control options.
   * (Cannot be real commands.)
   */
  enum Command {
    SW_DELAY = 0xf0,
    SCRIPT_END = 0xf1
  } __attribute__((packed));

  /**
   * Begin with script execution.
   * @param[in] script.
   */
  bool oled_begin(const uint8_t* script);

  /**
   * Set communication in data stream mode.
   */
  void set_data_mode()
    __attribute__((always_inline))
  {
    m_io->mode(OLED_IO::DATA);
  }

  /**
   * Set communication in instruction stream mode.
   */
  void set_instruction_mode()
    __attribute__((always_inline))
  {
    m_io->mode(OLED_IO::INSTRUCTION);
  }

  /**
   * Write character to display.
   * @param[in] c character.
   */

  virtual void print(char c);

  /**
   * Transform pixels based on text mode.
   * @param[in] byte.
   * @return possibly inverted byte.
   */
  uint8_t transform(uint8_t p)
    __attribute__((always_inline))
  {
    if (m_mode & INVERTED_TEXT_MODE)
      return (~p);
    else
      return (p);
  }
};
#endif
