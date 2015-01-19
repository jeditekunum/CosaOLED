/**
 * @file ?/OLED_IO_SPI.cpp
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

#include "OLED_IO_SPI.hh"

void
OLED_IO_SPI::setup()
{
}

void
OLED_IO_SPI::write8b(uint8_t data)
{
#ifdef OLED_IO_DEBUG
  if (m_data_trace)
    trace << PSTR("OLED_IO_SPI::write8b ")
          << ((m_mode == INSTRUCTION) ? PSTR("INSTRUCTION ") : PSTR("DATA "))
          << hex << data << endl;
#endif

  spi.acquire(this);
    spi.begin();
      if (m_mode == DATA)
        m_dc.high();
      else
        m_dc.low();
      spi.transfer(data);
    spi.end();
  spi.release();
  DELAY(SHORT_EXEC_TIME);
}

void
OLED_IO_SPI::write8n(void *buf, size_t size)
{
  uint8_t *bp = (uint8_t*)buf;

#ifdef OLED_IO_DEBUG
  if (m_data_trace)
    {
      uint8_t* bp2 = (uint8_t*)buf;
      trace << PSTR("OLED_IO_SPI::write8n ")
            << ((m_mode == INSTRUCTION) ? PSTR("INSTRUCTION ") : PSTR("DATA "))
            << size << PSTR(" bytes") << endl;
      for (uint8_t i=0; i<size; i++)
        trace << ((i>0) ? PSTR(",") : PSTR("")) << hex << *(bp2++);
      trace << endl;
    }
#endif

  spi.acquire(this);
    spi.begin();
      if (m_mode == DATA)
        m_dc.high();
      else
        m_dc.low();
      while (size--)
        spi.transfer(*(bp++));
    spi.end();
  spi.release();
  DELAY(SHORT_EXEC_TIME);
}
