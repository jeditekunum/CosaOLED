/**
 * @file ?/OLED_IO_TWI.cpp
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

#include "OLED_IO_TWI.hh"

void
OLED_IO_TWI::setup()
{
}

void
OLED_IO_TWI::write8b(uint8_t data)
{
  uint8_t mode = (m_mode == INSTRUCTION) ? 0x00 : 0x40;

#ifdef OLED_IO_DEBUG
  if (m_data_trace)
    trace << PSTR("OLED_IO_TWI::write8b ")
          << ((m_mode == INSTRUCTION) ? PSTR("INSTRUCTION ") : PSTR("DATA "))
          << hex << data << endl;
#endif

  twi.begin(this);
  twi.write(mode, &data, sizeof(data));
  twi.end();
}

void
OLED_IO_TWI::write8n(void *buf, size_t size)
{
  uint8_t mode = (m_mode == INSTRUCTION) ? 0x00 : 0x40;

#ifdef OLED_IO_DEBUG
  if (m_data_trace)
    {
      uint8_t* bp = (uint8_t*)buf;
      trace << PSTR("OLED_IO_TWI::write8n ")
            << ((m_mode == INSTRUCTION) ? PSTR("INSTRUCTION ") : PSTR("DATA "))
            << size << PSTR(" bytes") << endl;
      for (uint8_t i=0; i<size; i++)
        trace << ((i>0) ? PSTR(",") : PSTR("")) << hex << *(bp++);
      trace << endl;
    }
#endif

  twi.begin(this);
  twi.write(mode, buf, size);
  twi.end();
}
