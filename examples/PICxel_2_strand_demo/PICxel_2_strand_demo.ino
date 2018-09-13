/************************************************************************
 *  PICxel_2_strand_demo.ino - PIC32 Neopixel Library Demo
 *
 *  A simple to use library for addressable LEDs like the WS2812 for
 *  the PIC32 line of microcontrollers.
 *
 *  tested supported boards:
 *    - chipKIT UNO32
 *    - chipKIT UC32
 *    - chipHIT Fubarino SD
 *
 *  This library is protected under the GNU GPL v3.0 license
 *  http://www.gnu.org/licenses/
 ************************************************************************/
#include <PICxel.h>

#define NUMBER_OF_LEDS_STRIP1   15
#define LED_PIN_STRIP1           0

#define NUMBER_OF_LEDS_STRIP2   15
#define LED_PIN_STRIP2           1

#define millisecond_delay_strip 50

PICxel strip1(NUMBER_OF_LEDS_STRIP1, LED_PIN_STRIP1, HSV);
PICxel strip2(NUMBER_OF_LEDS_STRIP2, LED_PIN_STRIP2, HSV);

int hue = 0;
int sat = 255;
int val = 100;
int prevTime = 0;
int runner_num = 0;
int count_up_flag = 0;
int runner_delay_count = 0;
int runner_hue = 0;

void setup()
{
  strip1.begin();
  strip2.begin();
}

void loop()
{
  if (millis() - prevTime > 10)
  {
    prevTime = millis();
    hue++;
    val++;
    sat++;
    if (hue > 1535)
    {
      hue = 0;
    }
    if (val > 255)
    {
      val = 0;
    }
    if (sat > 255)
    {
      sat = 0;
    }
    
    for (int i = 0; i < NUMBER_OF_LEDS_STRIP1; i++)
    {
      if (i < (NUMBER_OF_LEDS_STRIP1/3))
      {
        strip1.HSVsetLEDColor(i, hue, 255, 50);
      }
      else if (i < (2*(NUMBER_OF_LEDS_STRIP1/3)))
      {
        strip1.HSVsetLEDColor(i, 0, 255, val);
      }
      else
      {
        strip1.HSVsetLEDColor(i, 0, sat,  100);
      }
    }
    
    if (runner_delay_count == 5)
    {
      if (count_up_flag == 0)
      {
        strip2.HSVsetLEDColor(runner_num, runner_hue++, 255, 100);
        strip2.clear(runner_num-1);
        runner_num++;
        if (runner_num == NUMBER_OF_LEDS_STRIP2)
        {
          count_up_flag = 1;
        }
      }
      else
      {
        strip2.HSVsetLEDColor(runner_num, runner_hue++, 255, 100);
        strip2.clear(runner_num+1);
        runner_num--;
        if (runner_num == -1)
        {
          count_up_flag = 0;
        }
      }
      if (runner_hue == 1535)
      {
        runner_hue = 0;
      }
      runner_delay_count = 0;
    }
    runner_delay_count++;	
    strip1.refreshLEDs();
    strip2.refreshLEDs();
  }
  delay(1);
}
