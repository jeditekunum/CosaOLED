/**
 * @file CosaOLED_Text_test.ino
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

//#define ONE_CHAR '&'
#define CYCLE_CHARS 0 // ms; 0 to benchmark

#define FLIPPED false
//#define FLIPPED true

#define SH1106
//#define SSD1306_64_TWI
//#define SSD1306_64_SPI
//#define SSD1306_32_TWI
//#define SSD1306_32_SPI

#define SYSTEM_5x7
//#define FIXEDNUMS_8x16
//#define SEGMENT_32x50

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
//#define FONT_8x13
//#define FONT_8x13B
//#define FONT_8x16
//#define FONT_9x15
//#define FONT_9x15B
//#define FONT_10x20
//#define FONT_12x24


#include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/Trace.hh"
#include "Cosa/RTC.hh"

#include "OLED_IO_TWI.hh"
#include "OLED_IO_SPI.hh"


#ifdef SYSTEM_5x7
#include <System5x7.hh>
#define FONT system5x7
#endif

#ifdef FONT_5x8
#include <Font5x8.hh>
#define FONT font5x8
#endif

#ifdef FONT_6x9
#include <Font6x9.hh>
#define FONT font6x9
#endif

#ifdef FONT_6x10
#include <Font6x10.hh>
#define FONT font6x10
#endif

#ifdef FONT_6x12
#include <Font6x12.hh>
#define FONT font6x12
#endif

#ifdef FONT_6x13
#include <Font6x13.hh>
#define FONT font6x13
#endif

#ifdef FONT_6x13B
#include <Font6x13B.hh>
#define FONT font6x13B
#endif

#ifdef FONT_7x13
#include <Font7x13.hh>
#define FONT font7x13
#endif

#ifdef FONT_7x13B
#include <Font7x13B.hh>
#define FONT font7x13B
#endif

#ifdef FONT_7x14
#include <Font7x14.hh>
#define FONT font7x14
#endif

#ifdef FONT_7x14B
#include <Font7x14B.hh>
#define FONT font7x14B
#endif

#ifdef FONT_8x13
#include <Font8x13.hh>
#define FONT font8x13
#endif

#ifdef FONT_8x13B
#include <Font8x13B.hh>
#define FONT font8x13B
#endif

#ifdef FONT_8x16
#include <Font8x16.hh>
#define FONT font8x16
#endif

#ifdef FONT_9x15
#include <Font9x15.hh>
#define FONT font9x15
#endif

#ifdef FONT_9x15B
#include <Font9x15B.hh>
#define FONT font9x15B
#endif

#ifdef FONT_10x20
#include <Font10x20.hh>
#define FONT font10x20
#endif

#ifdef FONT_12x24
#include <Font12x24.hh>
#define FONT font12x24
#endif

#ifdef FIXEDNUMS_8x16
#include <FixedNums8x16.hh>
#define FONT fixednums8x16
#endif

#ifdef SEGMENT_32x50
#include <Segment32x50.hh>
#define FONT segment32x50
#endif

#ifdef SH1106
#define DEVICE "SH1106"
#include "SH1106_Text.hh"
static OLED_IO_TWI oled_port(0x3c);
static SH1106_Text oled(&oled_port, (Font*)&FONT, FLIPPED);
#endif

#ifdef SSD1306_64_TWI
#define DEVICE "SSD1306 64 TWI"
#include "SSD1306x64_Text.hh"
static OLED_IO_TWI oled_port(0x3d);
static SSD1306x64_Text oled(&oled_port, (Font*)&FONT, FLIPPED);
#endif

#ifdef SSD1306_32_TWI
#define DEVICE "SSD1306 32 TWI"
#include "SSD1306x32_Text.hh"
static OLED_IO_TWI oled_port(0x3c);
static SSD1306x32_Text oled(&oled_port, (Font*)&FONT, FLIPPED);
#endif

#ifdef SSD1306_64_SPI
#define DEVICE "SSD1306 64 SPI"
#include "SSD1306x64_Text.hh"
static OLED_IO_SPI oled_port(Board::D11);
static SSD1306x64_Text oled(&oled_port, (Font*)&FONT, FLIPPED);
#endif

#ifdef SSD1306_32_SPI
#define DEVICE "SSD1306 32 SPI"
#include "SSD1306x32_Text.hh"
static OLED_IO_SPI oled_port(Board::D11);
static SSD1306x32_Text oled(&oled_port, (Font*)&FONT, FLIPPED);
#endif

IOStream ios(&uart);
static IOStream oledout(&oled);

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#ifdef SYSTEM_5x7
#define FONT_FIRST 0x0
#define FONT_LAST 0x7f
#else
#define FONT_FIRST FONT.FIRST
#define FONT_LAST FONT.LAST
#endif

uint8_t col = 0;
uint8_t row = 0;

void setup()
{
  RTC::begin();
  uart.begin(9600);
  trace.begin(&uart);
  trace << PSTR("CosaCanvasFont: started ")
        << PSTR(DEVICE)
        << PSTR(" font ") << STRINGIFY(FONT)
        << endl;

  oled.begin();

  // Flash
  oled.display_inverse();
  delay(250);
  oled.display_normal();

  oled.set_cursor(col, row);

#ifdef ONE_CHAR
  oled.putchar(ONE_CHAR);
#endif

#if !defined(ONE_CHAR) && !defined(CYCLE_CHARS)
  oledout << PSTR("Hello World!");
#endif

#ifdef CYCLE_CHARS
  trace << PSTR("Font has ") << (FONT.LAST-FONT.FIRST+1)
        << PSTR(" characters")
        << endl;

  oled.set_text_mode(OLED_Text::RAW_TEXT_MODE);
#endif
}

void loop()
{
#ifdef CYCLE_CHARS
  if (FONT.FIRST < ' ')
    oled.set_text_mode(oled.get_text_mode() | OLED_Text::INVERTED_TEXT_MODE);

#if CYCLE_CHARS == 0
  MEASURE("full character set ", 1)
#endif
  {
    for (uint16_t c = FONT.FIRST; c <= FONT.LAST; c++)
      {
        if (c == ' ' || c == 128)
          oled.set_text_mode(oled.get_text_mode() ^ OLED_Text::INVERTED_TEXT_MODE);

        oled.putchar(c);

        if (CYCLE_CHARS != 0) delay(CYCLE_CHARS);
      }
  }

  // Save text mode, set to normal
  uint8_t savemode = oled.get_text_mode();
  oled.set_text_mode(OLED_Text::NORMAL_TEXT_MODE);

  // Clear rest of screen
  uint8_t x, y;
  oled.get_cursor(x, y);
  for (uint8_t row = y; row < oled.height(); row++)
    {
      if (row > y)
        oled.set_cursor(0, row);
      oled.line_clear();
    }

  // Restore text mode
  oled.set_text_mode(savemode);

  // Flip underline for next pass
  oled.set_text_mode(oled.get_text_mode() ^ OLED_Text::UNDERLINED_TEXT_MODE);

  sleep(2);
  oled.display_clear();
#else
  delay(100);
#endif
}
