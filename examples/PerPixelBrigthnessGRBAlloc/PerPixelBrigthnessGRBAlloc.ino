/*
 * PerPixelBrightnessGRBAlloc.ino - PIC32 Neopixel Library Demo
 *
 * A simple to use library for addressable LEDs like the WS2812 for
 * the PIC32 line of microcontrollers.
 *
 * supported boards: All known chipKIT boards
 *
 * This library is protected under the GNU GPL v3.0 license
 * http://www.gnu.org/licenses/
 *
 * This demo uses the built-in memory allocation of library for the arrays
 * necessary for per-pixel brightness control. The library uses malloc to 
 * allocate memory from the heap. (The heap is only big enough for about 
 * 249 pixels.)
 * 
 * The general idea of this sketch is to set up the entire pixel array
 * with a single color, and then initialize the per-pixel brightness values
 * to a ramp, starting with 0 brightness for pixel 0, and 100% brightness
 * for pixel NUMBER_OF_LEDS.
 * 
 * Then in loop(), we just increment each pixel's brightness and update the
 * LEDs with the new values. This shows that you can control the brightness
 * of each pixel on it's own, without changing the color values at all.
 */

#include <PICxel.h>

#define NUMBER_OF_LEDS    15
#define LED_PIN            18
#define MILLISECOND_DELAY 50

// PICxel constructor(
//  uint8_t # of LEDs, 
//  uint8_t pin #, 
//  color_mode_t GRB or HSV,
//  memory_mode_t alloc or noalloc, 
//  brightness_mode_t globalOnly or perPixel);
PICxel strip(NUMBER_OF_LEDS, LED_PIN, GRB, alloc, perPixel);

void setup()
{
  strip.begin();
  strip.clear();
  // Fill pixels with initial color and brightness
  for (int i = 0; i < strip.getNumberOfLEDs(); i++)
  {
    strip.GRBsetLEDColor(i, 255, 128, 64);
    strip.setPixelBrightness(i, (i * 255) / NUMBER_OF_LEDS);
  }
  strip.refreshLEDs();
}

void loop()
{
  // For the light show, just walk each brightness up by a bit
  for (int i = 0; i < strip.getNumberOfLEDs(); i++)
  {
    strip.setPixelBrightness(i, strip.getPixelBrightness(i) + 8);
  }
  strip.refreshLEDs();
  delay(MILLISECOND_DELAY);
}
