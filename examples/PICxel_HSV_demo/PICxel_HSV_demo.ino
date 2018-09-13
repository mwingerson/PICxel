/************************************************************************
 *  PICxel_HSV_demo.ino - PIC32 Neopixel Library Demo
 *
 *  A simple to use library for addressable LEDs like the WS2812 for
 *  the PIC32 line of microcontrollers.
 *
 *  tested supported boards:
 *    - chipKIT UNO32
 *    - chipKIT UC32
 *    - Fubarino SD
 *
 *  This library is protected under the GNU GPL v3.0 license
 *  http://www.gnu.org/licenses/
 ************************************************************************/
#include <PICxel.h>

#define NUMBER_OF_LEDS    15
#define LED_PIN            0
#define MILLISECOND_DELAY 10

PICxel strip(NUMBER_OF_LEDS, LED_PIN, HSV);

int hue = 0;
int sat = 0;
int val = 0;
int prevTime = 0;

void setup()
{
  strip.begin();
}

void loop()
{
  if(millis()-prevTime > MILLISECOND_DELAY)
  {
    prevTime = millis();
    hue++;
    val++;
    sat++;
    if(hue > 1535)
      hue = 0;
    if(val > 255)
      val = 0;
    if(sat > 255)
      sat = 0;
    
    for(int i = 0; i < NUMBER_OF_LEDS; i++)
    {
      if(i < NUMBER_OF_LEDS/3)
      {
        strip.HSVsetLEDColor(i, hue, 255, 50);
      }
      else if (i < (2*NUMBER_OF_LEDS)/3)
      {
        strip.HSVsetLEDColor(i, 0, 255, val);
      }
      else
      {
        strip.HSVsetLEDColor(i, 0, sat,  100);
      }
    }
  }
  strip.refreshLEDs();
  delay(1);
}