/**
 * @file ?/OLED_IO.hh
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

#ifndef COSA_X_OLED_IO_HH
#define COSA_X_OLED_IO_HH

#include "Cosa/Types.h"

#define OLED_IO_DEBUG

/**
 * Abstract OLED IO adapter to isolate communication specific
 * functions and allow access over serial interfaces; I2C/TWI
 * and SPI.
 */
class OLED_IO {
public:
  enum mode_t
    {
      INSTRUCTION,
      DATA
    };

  /**
   * @override OLED_IO
   * Initiate IO port.
   * @return bool.
   */
  virtual void setup() = 0;

  /**
   * @override OLED_IO
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data) = 0;

  /**
   * @override OLED_IO
   * Write buffer to display.
   * @param[in] buf pointer to buffer.
   * @param[in] size number of bytes in buffer.
   */
  virtual void write8n(void* buf, size_t size) = 0;

  /**
   * @override OLED_IO
   * Set data/command mode.
   * @param[in] flag.
   * @return previous mode.
   */
  mode_t mode(mode_t mode)
    __attribute__((always_inline))
  {
    mode_t previous = m_mode;
    m_mode = mode;
    return (previous);
  }

  /**
   * @override OLED_IO
   * Get data/command mode.
   * @return mode.
   */
  virtual mode_t mode()
    __attribute__((always_inline))
  {
    return (m_mode);
  }

#ifdef OLED_IO_DEBUG
  /**
   * @override OLED_IO
   * Set data trace mode.
   * @param[in] on/off.
   * @return previous state.
   */
  bool data_trace(bool state)
    __attribute__((always_inline))
  {
    bool previous = m_data_trace;
    m_data_trace = state;
    return (previous);
  }

  /**
   * @override OLED_IO
   * Get data trace mode.
   * @return trace state.
   */
  bool data_trace()
    __attribute__((always_inline))
  {
    return (m_data_trace);
  }
#endif

protected:
  /** Mode */
  mode_t m_mode;

#ifdef OLED_IO_DEBUG
  /** Data trace */
  bool m_data_trace;
#endif
};

#endif
