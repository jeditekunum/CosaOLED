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

#include "OLED_Text.hh"

/**
 * SH1106 Dot Matrix OLED/PLED Segment/Common Driver with Controller
 * for LCD/IOStream access.
 *
 * SH1106 is manufactured by Sino Wealth Electronic Ltd.
 */
class SH1106_Text : public OLED_Text {
public:
  /**
   * Construct SH1106_Text OLED connected to given io port handler. The
   * display is initialized when calling begin(). 
   * @param[in] io handler.
   * @param[in] font font to display.
   * @param[in] flipped screen.
   * @param[in] screen_width.
   * @param[in] screen_height.
   * @param[in] column_offset.
   */
  SH1106_Text(OLED_IO* io, Font* font,
              bool flipped = false,
              uint8_t screen_width = 128, uint8_t screen_height = 64,
              uint8_t column_offset = 2) :
    OLED_Text(io, font, screen_width, screen_height, column_offset),
    m_flipped(flipped)
  {}

  /**
   * @override LCD::Device
   * Start display for text output. Returns true if successful
   * otherwise false.
   * @return boolean.
   */
  virtual bool begin();

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

protected:
  /**
   * Configure for flipped?
   */
  const bool m_flipped;

  /**
   * Initialization script (in program memory).
   */
  static const uint8_t script[] PROGMEM;
  static const uint8_t flipped_script[] PROGMEM;

  /**
   * Write character to display.
   * @param[in] c character.
   */

  virtual void print(char c);

  /**
   * Commands
   */
  enum Command {
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
