/**
 * @file ?/SH1106_Text.cpp
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

#include "SH1106_Text.hh"

const uint8_t SH1106_Text::script[] __PROGMEM = {
  DISPLAY_OFF, 0,                               // 0xae
  SET_DISPLAY_CLOCK_DIV, 1, 0x80,               // 0xd5 0x80
  SET_MULTIPLEX, 1, 0x3f,                       // 0xa8 0x3f
  SET_DISPLAY_OFFSET, 1, 0,                     // 0xd3 0x00
  SET_START_LINE, 0,                            // 0x40
  SEG_REMAP | 0x1, 0,                           // 0xa0 | 0x1
  COM_SCAN_DEC, 0,                              // 0xc8
  SET_COMPINS, 1, 0x12,                         // 0xda 0x12
  SET_CONTRAST, 1, 0xcf,                        // 0x81 0xcf
  SET_PRECHARGE, 1, 0xf1,                       // 0xd9 0xf1
  SET_VCOM_DETECT, 1, 0x40,                     // 0xdb 0x40
  DEACTIVATE_SCROLL, 0,                         // 0x2e
  DISPLAY_ALL_ON_RESUME, 0,                     // 0xa4
  NORMAL_DISPLAY, 0,                            // 0xa6
  DISPLAY_ON, 0,                                // 0xaf
  SCRIPT_END
};

const uint8_t SH1106_Text::flipped_script[] __PROGMEM = {
  DISPLAY_OFF, 0,                               // 0xae
  SET_DISPLAY_CLOCK_DIV, 1, 0x80,               // 0xd5 0x80
  SET_MULTIPLEX, 1, 0x3f,                       // 0xa8 0x3f
  SET_DISPLAY_OFFSET, 1, 0,                     // 0xd3 0x00
  SET_START_LINE, 0,                            // 0x40
  SEG_REMAP | 0x0, 0,                           // 0xa0 | 0x0 *
  COM_SCAN_INC, 0,                              // 0xc0 *
  SET_COMPINS, 1, 0x12,                         // 0xda 0x12
  SET_CONTRAST, 1, 0xcf,                        // 0x81 0xcf
  SET_PRECHARGE, 1, 0xf1,                       // 0xd9 0xf1
  SET_VCOM_DETECT, 1, 0x40,                     // 0xdb 0x40
  DEACTIVATE_SCROLL, 0,                         // 0x2e
  DISPLAY_ALL_ON_RESUME, 0,                     // 0xa4
  NORMAL_DISPLAY, 0,                            // 0xa6
  DISPLAY_ON, 0,                                // 0xaf
  SCRIPT_END
};

bool
SH1106_Text::begin()
{
  if (m_flipped)
    return (oled_begin(flipped_script));
  else
    return (oled_begin(script));
}

void
SH1106_Text::display_on()
{
#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_on BEGIN") << endl;
#endif

  m_io->write8b(DISPLAY_ON);

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_on END") << endl;
#endif
}

void
SH1106_Text::display_off()
{
#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_off BEGIN") << endl;
#endif

  m_io->write8b(DISPLAY_OFF);

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_off END") << endl;
#endif
}

void
SH1106_Text::display_normal()
{
#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_normal BEGIN") << endl;
#endif

  m_io->write8b(NORMAL_DISPLAY);

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_normal END") << endl;
#endif
}

void
SH1106_Text::display_inverse()
{
#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_inverse BEGIN") << endl;
#endif

  m_io->write8b(INVERT_DISPLAY);

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_inverse END") << endl;
#endif
}

void
SH1106_Text::print(char c)
{
  uint8_t start = m_x * (m_font->WIDTH + m_font->SPACING) + m_column_offset;

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
  {
    if (c >= ' ' && c <= '~')
      trace << PSTR("SH1106_Text::print('") << c << PSTR("') BEGIN") << endl;
    else
      trace << PSTR("SH1106_Text::print(") << hex << (uint8_t)c << dec << PSTR(") BEGIN") << endl;
  }
#endif

  Font::Glyph glyph(m_font,c);

  uint8_t underline_overlay = 0;
  if (m_mode & UNDERLINED_TEXT_MODE)
    underline_overlay = 1 << ((m_font->HEIGHT - 1) & 0x7);

  uint8_t underline = 0;

  bool clear = (c == ' ' && !m_font->available(c));

  for (uint8_t band = 0; band < BYTES(m_font->HEIGHT); band++)
    {
      if (band == BYTES(m_font->HEIGHT)-1)
        underline = underline_overlay;

      // Set row (page)
      m_io->write8b(0xb0 | ((m_y * BYTES(m_font->HEIGHT)) + band));

      // Set column start
      m_io->write8b(SET_LOW_COLUMN | (start & 0xf));
      m_io->write8b(SET_HIGH_COLUMN | (start>>4));

      set_data_mode();
      for (uint8_t i = 0; i<m_font->WIDTH; i++)
        if (!clear)
          m_io->write8b(transform(glyph.next() | underline));
        else
          m_io->write8b(transform(0));
      for (uint8_t i = 0; i<m_font->SPACING; i++)
        m_io->write8b(transform(0 | underline));
      set_instruction_mode();
    }

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::print END") << endl;
#endif
}

void
SH1106_Text::display_clear()
{
  uint8_t setbytes[32];

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_clear BEGIN") << endl;
#endif

  // set values
  memset(setbytes, 0, sizeof(setbytes));

  for (uint8_t row=0; row<(m_screen_height/8); row++)
    {
      // Set row (page)
      m_io->write8b(SET_ROW | row);
      // Set column start
      m_io->write8b(SET_LOW_COLUMN);
      m_io->write8b(SET_HIGH_COLUMN);

      uint8_t remain = m_screen_width + (m_column_offset*2);

      set_data_mode();
      while (remain)
        {
          uint8_t t = remain > sizeof(setbytes) ? sizeof(setbytes) : remain;
          m_io->write8n(setbytes, t);
          remain -= t;
        }
      set_instruction_mode();
    }

  set_cursor(0, 0);

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::display_clear END") << endl;
#endif
}
