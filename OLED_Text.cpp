/**
 * @file ?/OLED_Text.cpp
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

#include "OLED_Text.hh"

bool
OLED_Text::oled_begin(const uint8_t* script)
{
  if (m_initialized) return (false);

  m_io->setup();

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("OLED_Text::begin BEGIN") << endl;
#endif

  const uint8_t* bp = script;
  uint8_t count;
  uint8_t cmd;

  while ((cmd = pgm_read_byte(bp++)) != SCRIPT_END)
    {
      count = pgm_read_byte(bp++);

      switch (cmd)
        {
        case SW_DELAY:
          DELAY(count);
          break;

        default:
          m_io->write8b(cmd);
          while (count--) m_io->write8b(pgm_read_byte(bp++));
          break;
        }
    }

  m_initialized = true;
  display_clear();

#ifdef OLED_IO_DEBUG
  if (m_io->data_trace())
    trace << PSTR("OLED_Text::begin END") << endl;
#endif

  return (true);
}

int
OLED_Text::putchar(char c)
{
  // Check for special characters
  if (!(m_mode & RAW_TEXT_MODE) && c < ' ') {
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
      for (uint8_t i = 0; i < m_columns; i++) {
        print(' ');
        m_x++;
      }
      set_cursor(x, y);
      return (c);
    }

    // Horizontal tab
    if (c == '\t') {
      uint8_t x = m_x + m_tab - (m_x % m_tab);
      uint8_t y = m_y + (x >= m_columns);
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

  if (m_x == m_columns) {
      set_cursor(0, m_y + 1);
  }

  // Write character
  print(c);
  m_x++;

  return (c & 0xff);
}

int
OLED_Text::write(const void *buf, size_t size)
{
  int count = size;
  uint8_t *dp = (uint8_t*)buf;

  while (count--)
    putchar(*dp++);

  return (size);
}

void
OLED_Text::print(char c)
{
  UNUSED(c);
}
