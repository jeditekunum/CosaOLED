/**
 * @file ?/OLED_IO_SPI.hh
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

#ifndef COSA_X_OLED_IO_SPI_HH
#define COSA_X_OLED_IO_SPI_HH

#include "Cosa/SPI.hh"
#include "Cosa/OutputPin.hh"
#include "OLED_IO.hh"

/**
 * IO handler for OLED Controller/Driver when using hardware SPI.
 */
class OLED_IO_SPI : public OLED_IO, public SPI::Driver {
public:
  /**
   * Construct SSD1306_Text 4-wire serial port connected to given
   * enable/chip select pin and data/command pin.  Uses the SPI::MOSI
   * and SPI::SCK pins.
   * @param[in] en enable (cs) pulse.
   * @param[in] dc data/command pin.
   */
  OLED_IO_SPI(Board::DigitalPin en, Board::DigitalPin dc) :
    SPI::Driver(en, SPI::PULSE_HIGH),
    m_dc(dc)
  {
    m_mode = INSTRUCTION;
  }

  /**
   * @override OLED_IO_SPI
   * Initiate port for 8-bit serial mode.
   * @return true(1).
   */
  virtual void setup();
    
  /**
   * @override OLED_IO_SPI
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data);

  /**
   * @override OLED_IO_SPI
   * Write buffer to display.
   * @param[in] buf pointer to buffer.
   * @param[in] size number of bytes in buffer.
   */
  virtual void write8n(void* buf, size_t size);

protected:
  /** Execution time delay (us). */
#if !defined(BOARD_ATTINY)
  static const uint16_t SHORT_EXEC_TIME = 24;
#else
  static const uint16_t SHORT_EXEC_TIME = 8;
#endif

  OutputPin m_dc;
};

#endif
