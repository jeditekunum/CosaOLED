/**
 * @file ?/OLED_IO.cpp
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

#include "Cosa/OutputPin.hh"

#include "OLED_IO.hh"

void
OLED_IO::play_script(const uint8_t* script)
{
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
          write8b(cmd);
          while (count--) write8b(pgm_read_byte(bp++));
          break;
        }
    }
}
