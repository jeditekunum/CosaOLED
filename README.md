CosaOLED
========

OLED Drivers for Cosa

Support for I2C/TWI/SPI SSD1306/SH1106 OLED in Cosa

At this time only slim drivers are provided.  Slim means that it does not support
graphics beyond displaying fonts.  The SSD1306 family requires a full frame buffer
in memory since it does not support writing individual pixels nor can the devices
memory be read by applications.  Since Cosa is aimed at small AVR devices it would
require substantial resources and larger chips to support a full frame buffer.
