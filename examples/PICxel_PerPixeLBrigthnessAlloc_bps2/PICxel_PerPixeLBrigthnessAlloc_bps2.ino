/************************************************************************/
/*  PICxel_demo.pde - PIC32 Neopixel Library Demo                       */
/*																		*/
/*  A simple to use library for addressable LEDs like the WS2812 for    */
/*  the PIC32 line of microcontrollers.                                 */
/*                                                                      */
/*  tested supported boards:                                            */
/*    - Digilent UNO32                                                  */
/*    - Digilent UC32                                                   */
/*                                                                      */
/*  This library is protected under the GNU GPL v3.0 license            */
/*  http://www.gnu.org/licenses/                                        */
/************************************************************************/

#include <PICxel.h>

#define NUMBER_OF_LEDS 15
#define LED_PIN 18                // Use pin 18 for RGB Station board
#define MILLISECOND_DELAY 50

// PICxel constructor(uint8_t # of LEDs, uint8_t pin #, color_mode_t GRB or HSV, memory_mode_t alloc or noalloc, brightness_mode_t globalOnly or perPixel);
PICxel strip(NUMBER_OF_LEDS, LED_PIN, GRB, alloc, perPixel);

void setup()
{
	strip.begin();
	strip.clear();
  // Fill pixels with initial color
  for (int i = 0; i < strip.getNumberOfLEDs(); i++)
  {
    strip.GRBsetLEDColor(i, 255, 128, 64);
  }
  strip.refreshLEDs();
}

void loop()
{
  static uint8_t j = 0;
  
	for (int i = 0; i < strip.getNumberOfLEDs(); i++)
	{
		strip.setPixelBrightness(i, j);
	}
  strip.refreshLEDs();
  delay(MILLISECOND_DELAY);

  j++;
}
