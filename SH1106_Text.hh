/**
 * @file ?/SH1106_Text.hh
 * @version 0.1
 *
 * @section License
 * Copyright (C) 2014, jediunix
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

#ifndef COSA_X_SH1106_TEXT_HH
#define COSA_X_SH1106_TEXT_HH

#include "Cosa/LCD.hh"
#include "Cosa/Canvas/Font.hh"

#include "OLED_IO.hh"

/**
 * SH1106 Dot Matrix OLED/PLED Segment/Common Driver with Controller
 * for LCD/IOStream access.
 *
 * SH1106 is manufactured by Sino Wealth Electronic Ltd.
 */
class SH1106_Text : public LCD::Device {
public:
  /**
   * Construct SH1106_Text OLED connected to given io port handler. The
   * display is initialized when calling begin(). 
   * @param[in] io handler.
   * @param[in] font font to display.
   * @param[in] display reset pin (optional).
   */
  SH1106_Text(OLED_IO* io, Font* font, Board::DigitalPin reset_pin);

  SH1106_Text(OLED_IO* io, Font* font);

  /**
   * @override LCD::Device
   * Start display for text output. Returns true if successful
   * otherwise false.
   * @return boolean.
   */
  virtual bool begin();

  /**
   * @override LCD::Device
   * Stop display and power down. Returns true if successful 
   * otherwise false.
   */
  virtual bool end();

  /**
   * @override LCD::Device
   * Turn display on. 
   */
  virtual void display_on();

  /**
   * @override LCD::Device
   * Turn display off. 
   */
  virtual void display_off();

  /**
   * @override LCD::Device
   * Display normal mode.
   */
  virtual void display_normal();

  /**
   * @override LCD::Device
   * Display inverse mode. 
   */
  virtual void display_inverse();

  /**
   * @override LCD::Device
   * Clear display and move cursor to home(0, 0).
   */
  virtual void display_clear();

  /**
   * @override LCD::Device
   * Set cursor position to given position.
   * @param[in] x.
   * @param[in] y.
   */
  virtual void set_cursor(uint8_t x, uint8_t y);

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
   * @override IOStream::Device
   * Write data from buffer with given size to device.
   * @param[in] buf buffer to write.
   * @param[in] size number of bytes to write.
   * @return number of bytes written or EOF(-1).
   */
  virtual int write(void* buf, size_t size);

  /**
   * Clear to end of line.
   */
  void line_clear()
  {
    uint8_t x, y;
    get_cursor(x, y);
    while (m_x < m_width) putchar(' ');
    set_cursor(x, y);
  }

  /**
   * Get width in characters.
   */
  uint8_t width()
    __attribute__((always_inline))
  {
    return (m_width);
  }

  /**
   * Get height in characters.
   */
  uint8_t height()
    __attribute__((always_inline))
  {
    return (m_height);
  }

protected:
  /** Display IO. */
  OLED_IO* m_io;

  /** Optional reset pin */
  Board::DigitalPin m_reset_pin;
  bool m_have_reset_pin;

  /** Width in display. */
  uint8_t m_width_in_pixels;

  /** Column offset. */
  uint8_t m_column_offset;

  /** Height in display. */
  uint8_t m_height_in_pixels;

  /**
   * Font.
   */
  Font* m_font;

  /**
   * Initialization script (in program memory).
   */
  static const uint8_t script[] PROGMEM;
  uint8_t m_initialized;

  /** Display width (characters per line). */
  uint8_t m_width;

  /** Display height (lines). */
  uint8_t m_height;

  /**
   * Write character to display.
   * @param[in] c character.
   */

  virtual void print(char c);

  /**
   * Set communication in data stream mode.
   */
  virtual void set_data_mode() 
    __attribute__((always_inline))
  { 
    m_io->mode(OLED_IO::DATA);
  }

  /**
   * Set communication in instruction stream mode.
   */
  virtual void set_instruction_mode() 
    __attribute__((always_inline))
  { 
    m_io->mode(OLED_IO::INSTRUCTION);
  }

  /**
   * Resize (recalculates character width/height)
   */
  virtual void resize();

  /**
   * Commands
   */
  enum Command {
    SW_DELAY = 0xf0,   // pseudo
    SCRIPT_END = 0xf1,   // pseudo

    DISPLAY_OFF = 0xae,
    DISPLAY_ON = 0xaf,
    DISPLAY_ALL_ON_RESUME = 0xa4,
    DISPLAY_ALL_ON = 0xa5,

    SET_DISPLAY_CLOCK_DIV = 0xd5,
    SET_MULTIPLEX = 0xa8,

    SET_DISPLAY_OFFSET = 0xd3,
    SET_START_LINE = 0x40,

    SEG_REMAP = 0xa0,

    COM_SCAN_INC = 0xc0,
    COM_SCAN_DEC = 0xc8,

    SET_COMPINS = 0xda,

    SET_CONTRAST = 0x81,

    SET_PRECHARGE = 0xd9,
    SET_VCOM_DETECT = 0xdb,

    DEACTIVATE_SCROLL = 0x2e,
    ACTIVATE_SCROLL = 0x2F,

    NORMAL_DISPLAY = 0xa6,
    INVERT_DISPLAY = 0xa7,

    SET_ROW = 0xb0,
    SET_LOW_COLUMN = 0x00,
    SET_HIGH_COLUMN = 0x10,

    EXTERNAL_VCC = 0x01,
    SWITCH_CAP_VCC = 0x02,

    SET_VERTICAL_SCROLL_AREA = 0xa3,
    RIGHT_HORIZONTAL_SCROLL = 0x26,
    LEFT_HORIZONTAL_SCROLL = 0x27,
    VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
    VERTICAL_AND_LEFT_HORIZTONAL_SCROLL = 0x2a
  } __attribute__((packed));
};
#endif
