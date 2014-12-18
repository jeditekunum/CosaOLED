/**
 * @file ?/SH1106_Text.cpp
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

#include "Cosa/Trace.hh"

#include "SH1106_Text.hh"

#ifdef POST_GLYPH
#include "Cosa/Canvas/FontGlyph.hh"
#else
#include "Cosa/Canvas/Font/System5x7.hh"
#define FONT system5x7
#endif

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
  OLED_IO::SCRIPT_END
};

SH1106_Text::SH1106_Text(OLED_IO* io, Font* font) :
    LCD::Device(),
    m_io(io),
    m_have_reset_pin(false),
    m_width_in_pixels(128),
    m_column_offset(2),
    m_height_in_pixels(64),
    m_font(font),
    m_initialized(false)
{
  resize();
}

SH1106_Text::SH1106_Text(OLED_IO* io, Font* font, Board::DigitalPin reset_pin) :
  SH1106_Text(io, font)
{
  m_reset_pin = reset_pin;
  m_have_reset_pin = true;
}

bool
SH1106_Text::begin()
{
  return (shared_begin(script));
}

bool
SH1106_Text::end()
{
  display_clear();
  display_off();
  return (true);
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
SH1106_Text::set_cursor(uint8_t x, uint8_t y)
{
  if (x >= m_width) x = 0;
  if (y >= m_height) y = 0;
  m_x = x;
  m_y = y;
}

int
SH1106_Text::putchar(char c)
{
  // Check for special characters
  if (c < ' ') {
    
    // Carriage-return: move to start of line
    if (c == '\r') {
      set_cursor(0, m_y);
      return (c);
    }

    // New-line: clear line
    if (c == '\n') {
      uint8_t x, y;
      set_cursor(0, m_y + 1);
      get_cursor(x, y);
      for (uint8_t i = 0; i < m_width; i++) {
        print(' ');
        m_x++;
      }
      set_cursor(x, y);
      return (c);
    }

    // Horizontal tab
    if (c == '\t') {
      uint8_t x = m_x + m_tab - (m_x % m_tab);
      uint8_t y = m_y + (x >= m_width);
      set_cursor(x, y);
      return (c);
    }

    // Form-feed: clear the display
    if (c == '\f') {
      display_clear();
      return (c);
    }

    // Back-space: move cursor back one step (if possible)
    if (c == '\b') {
      set_cursor(m_x - 1, m_y);
      return (c);
    }

  }

  if (m_x == m_width) {
      set_cursor(0, m_y + 1);
  }

  // Write character
  print(c);
  m_x++;

  return (c & 0xff);
}

int
SH1106_Text::write(void *buf, size_t size)
{
  int count = size;
  uint8_t *dp = (uint8_t*)buf;

  while (count--)
    putchar(*dp++);

  return (size);
}

bool 
SH1106_Text::shared_begin(const uint8_t* s)
{
  if (m_initialized) return (false);

  m_io->setup();

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::shared_begin BEGIN") << endl;
#endif

  if (m_have_reset_pin)
    {
#ifdef OLED_IO_DEBUG
      if (m_io->data_trace())
        trace << PSTR("SH1106_Text::shared_begin pin reset") << endl;
#endif
      m_io->reset_device(m_reset_pin);
    }

  m_io->play_script(s);

  m_initialized = true;
  display_clear();

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::shared_begin END") << endl;
#endif

  return (true);
}

void
SH1106_Text::print(char c)
{
#ifdef POST_GLYPH
  FontGlyph glyph(m_font, c);
  uint8_t w = glyph.WIDTH;
  uint8_t h = glyph.HEIGHT;

  uint8_t stripes = (h + (CHARBITS-1)) / CHARBITS;
  uint8_t stripe = 0;

  uint8_t start = m_x * (w + glyph.spacing()) + m_column_offset;
#else
  uint8_t w = FONT.WIDTH;

  uint8_t start = m_x * (w + FONT.SPACING) + m_column_offset;
#endif



#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("SH1106_Text::print('") << c << PSTR("') BEGIN") << endl;
#endif

#ifdef POST_GLYPH
  glyph.begin();

  //  trace << glyph;

  do
    {
      // Set row (page)
      m_io->write8b(0xb0 | ((m_y * stripes) + stripe));

      // Set column start
      m_io->write8b(SET_LOW_COLUMN | (start & 0xf));
      m_io->write8b(SET_HIGH_COLUMN | (start>>4));

      set_data_mode();
      m_io->write8n((void*)glyph.next_stripe(), glyph.WIDTH);
      set_instruction_mode();

      stripe++;

    } while (!glyph.eog());

  glyph.end();
#else
  // Set row (page)
  m_io->write8b(0xb0 | m_y);

  // Set column start
  m_io->write8b(SET_LOW_COLUMN | (start & 0xf));
  m_io->write8b(SET_HIGH_COLUMN | (start>>4));

  const uint8_t* bp = FONT.get_bitmap(c);

  set_data_mode();
  for (uint8_t i = 0; i<FONT.WIDTH; i++)
    m_io->write8b(pgm_read_byte(bp++));
  set_instruction_mode();
#endif

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

  for (uint8_t row=0; row<(m_height_in_pixels/8); row++)
    {
      // Set row (page)
      m_io->write8b(SET_ROW | row);
      // Set column start
      m_io->write8b(SET_LOW_COLUMN);
      m_io->write8b(SET_HIGH_COLUMN);

      uint8_t remain = m_width_in_pixels + (m_column_offset*2);

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

void
SH1106_Text::resize()
{
#ifdef POST_GLYPH
  m_width = m_width_in_pixels / (m_font->width('z') + m_font->spacing());
  m_height = m_height_in_pixels / (m_font->height('z') + m_font->line_spacing());
#else
  m_width = m_width_in_pixels / (FONT.WIDTH + FONT.SPACING);
  m_height = m_height_in_pixels / FONT.HEIGHT;
#endif
}
