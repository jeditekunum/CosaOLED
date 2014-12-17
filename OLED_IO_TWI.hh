/**
 * @file ?/OLED_IO_TWI.hh
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

#ifndef COSA_X_OLED_IO_TWI_HH
#define COSA_X_OLED_IO_TWI_HH

#include "Cosa/TWI.hh"
#include "OLED_IO.hh"

/**
 * IO handler for OLED Controller/Driver when using I2C/TWI.
 */
class OLED_IO_TWI : public OLED_IO, public TWI::Driver {
public:
  /**
   * Construct IO port handler using I2C/TWI with given address.
   * @param[in] addr address.
   */
  OLED_IO_TWI(uint8_t addr) :
    TWI::Driver(addr)
  {
    m_mode = INSTRUCTION;
  }

  /**
   * @override OLED_IO_TWI
   * Initiate port for 8-bit serial mode.
   * @return true(1).
   */
  virtual void setup();
    
  /**
   * @override OLED_IO_TWI
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data);

  /**
   * @override OLED_IO_TWI
   * Write buffer to display.
   * @param[in] buf pointer to buffer.
   * @param[in] size number of bytes in buffer.
   */
  virtual void write8n(void* buf, size_t size);
};

#endif
