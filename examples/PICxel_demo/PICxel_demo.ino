/************************************************************************
 *  PICxel_demo - PIC32 Neopixel Library Demo
 *
 *  A simple to use library for addressable LEDs like the WS2812 for
 *  the PIC32 line of microcontrollers.
 *
 *  supported boards:
 *    - all chipKIT boards
 *
 *  This library is protected under the GNU GPL v3.0 license
 *  http://www.gnu.org/licenses/
 *
 *  This demo is just about the simplest sketch you can have with PICxel.
 *  This sketch uses GRB mode.
 *  It creates a PICexl object called strip with space for 15 pixels,
 *  then walks through the pixels from one end of the strip to the other,
 *  first setting them to 100% red, then walks from the other end back
 *  changing each pixel to 100% green.
 *  The delay sets the speed of the color changing. Note that the strip
 *  is refreshed (i.e. the new values are sent out to the physical LEDs)
 *  after each color change.
 *
 *  To use this sketch on your own chipKIT board, simply change the pin
 *  number defined by LED_PIN to whatever pin you have your LED string
 *  attached to, and change the NUMBER_OF_LEDS to however many you have
 *  in your strip.
 *
 *  Note that in this demo, we are using the simplest form of the 
 *  constructor. When you just use two parameters in the constructor,
 *  you get to set the number of LEDs in the strip and the pin on which
 *  the strip is connected. The library assumes that you are using GRB 
 *  color mode, that you want the library to do the memory allocation for
 *  you (alloc mode) and that you are not using per-pixel brightness.
 *  All of these values can be changed by using more parameters.
 ************************************************************************/

#include <PICxel.h>

#define NUMBER_OF_LEDS      15
#define LED_PIN              0
#define MILLISECOND_DELAY   50

// PICxel constructor(uint8_t # of LEDs, uint8_t pin #);
PICxel strip(NUMBER_OF_LEDS, LED_PIN);

void setup()
{
  strip.begin();
  strip.setBrightness(30);
  strip.clear();
}

void loop()
{
  for (int i = 0; i < strip.getNumberOfLEDs(); i++)
  {
    strip.GRBsetLEDColor(i, 255, 0, 0);
    strip.refreshLEDs();
    delay(MILLISECOND_DELAY);
  }

  for (int i = strip.getNumberOfLEDs(); i >= 0; i--)
  {
    strip.GRBsetLEDColor(i, 0, 255, 0);
    strip.refreshLEDs();
    delay(MILLISECOND_DELAY);
  }
}
