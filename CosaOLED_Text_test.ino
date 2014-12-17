/**
 * @file CosaOLED_Text_test.ino
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

//#define ONE_CHAR '&'
#define CYCLE_CHARS 10 // ms

//#define POST_GLYPH

#define SH1106
//#define SSD1306_64_TWI
//#define SSD1306_64_SPI
//#define SSD1306_32_TWI
//#define SSD1306_32_SPI

#define SYSTEM_5x7
//#define FIXEDNUMS_8x16
//#define SEGMENT_32x50

#ifdef POST_GLYPH
//#define FONT_5x8
//#define FONT_6x9
//#define FONT_6x10
//#define FONT_6x12
//#define FONT_6x13
//#define FONT_6x13B
//#define FONT_7x13
//#define FONT_7x13B
//#define FONT_7x14
//#define FONT_7x14B
//#define FONT_8x16
//#define FONT_10x20
#endif


#include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/Trace.hh"
#include "Cosa/OutputPin.hh"

#include "OLED_IO_TWI.hh"
#include "OLED_IO_SPI.hh"


#ifdef SYSTEM_5x7
#include "Cosa/Canvas/Font/System5x7.hh"
#define FONT system5x7
#endif

#ifdef FONT_5x8
#include "Cosa/Canvas/Font/Font5x8.hh"
#define FONT font5x8
#endif

#ifdef FONT_6x9
#include "Cosa/Canvas/Font/Font6x9.hh"
#define FONT font6x9
#endif

#ifdef FONT_6x10
#include "Cosa/Canvas/Font/Font6x10.hh"
#define FONT font6x10
#endif

#ifdef FONT_6x12
#include "Cosa/Canvas/Font/Font6x12.hh"
#define FONT font6x12
#endif

#ifdef FONT_6x13
#include "Cosa/Canvas/Font/Font6x13.hh"
#define FONT font6x13
#endif

#ifdef FONT_6x13B
#include "Cosa/Canvas/Font/Font6x13B.hh"
#define FONT font6x13B
#endif

#ifdef FONT_7x13
#include "Cosa/Canvas/Font/Font7x13.hh"
#define FONT font7x13
#endif

#ifdef FONT_7x13B
#include "Cosa/Canvas/Font/Font7x13B.hh"
#define FONT font7x13B
#endif

#ifdef FONT_7x14
#include "Cosa/Canvas/Font/Font7x14.hh"
#define FONT font7x14
#endif

#ifdef FONT_7x14B
#include "Cosa/Canvas/Font/Font7x14B.hh"
#define FONT font7x14B
#endif

#ifdef FONT_8x16
#include "Cosa/Canvas/Font/Font8x16.hh"
#define FONT font8x16
#endif

#ifdef FONT_10x20
#include "Cosa/Canvas/Font/Font10x20.hh"
#define FONT font10x20
#endif

#ifdef FIXEDNUMS_8x16
#include "Cosa/Canvas/Font/FixedNums8x16.hh"
#define FONT fixednums8x16
#endif

#ifdef SEGMENT_32x50
#include "Cosa/Canvas/Font/Segment32x50.hh"
#define FONT segment32x50
#endif

#ifdef SH1106
#define DEVICE "SH1106"
#include "SH1106_Text.hh"
static OLED_IO_TWI oled_port(0x3c);
static SH1106_Text oled(&oled_port, (Font*)&FONT);
#endif

#ifdef SSD1306_64_TWI
#define DEVICE "SSD1306 64 TWI"
#include "SSD1306x64_Text.hh"
static OLED_IO_TWI oled_port(0x3d);
static SSD1306x64_Text oled(&oled_port, (Font*)&FONT, Board::D0); // reset
#endif

#ifdef SSD1306_32_TWI
#define DEVICE "SSD1306 32 TWI"
#include "SSD1306x32_Text.hh"
static OLED_IO_TWI oled_port(0x3c);
static SSD1306x32_Text oled(&oled_port, (Font*)&FONT, Board::D0); // reset
#endif

#ifdef SSD1306_64_SPI
#define DEVICE "SSD1306 64 SPI"
#include "SSD1306x64_Text.hh"
static OLED_IO_SPI oled_port(Board::D1, Board::D2);  // cs, dc
static SSD1306x64_Text oled(&oled_port, (Font*)&FONT, Board::D0); // reset
#endif

#ifdef SSD1306_32_SPI
#define DEVICE "SSD1306 32 SPI"
#include "SSD1306x32_Text.hh"
static OLED_IO_SPI oled_port(Board::D1, Board::D2);  // cs, dc
static SSD1306x32_Text oled(&oled_port, (Font*)&FONT, Board::D0); // reset
#endif

OutputPin led(Board::LED);

IOStream ios(&uart);
static IOStream oledout(&oled);


void setup()
{
  uart.begin(9600);
  trace.begin(&uart);

  sleep(1);

  led.on();

  ios << endl << PSTR("CosaOLED_Text_test: ") << PSTR(DEVICE) << PSTR(" started") << endl;

  oled.begin();
  oled.display_inverse(); // flash
  delay(250);
  oled.display_normal();

  oled.set_cursor(0, 0);
#ifdef ONE_CHAR
  oled.putchar(ONE_CHAR);
#endif
}

uint8_t row = 0;
uint8_t col = 0;

void loop()
{
#ifdef CYCLE_CHARS
#if defined(FIXEDNUMS_8x16) || defined(SEGMENT_32x50)
  for (unsigned char c = FONT.first(); c <= FONT.last(); c++)
#else
  for (unsigned char c = ' '; c <= '~'; c++)
#endif
    {
      oled.putchar(c);
      delay(CYCLE_CHARS);

      col++;
      if (col == oled.width())
        {
          row++;
          if (row == oled.height())
            {
              row = 0;
            }
          col = 0;
          oled.set_cursor(col, row);
        }
    }
#else
  delay(100);
#endif
}
