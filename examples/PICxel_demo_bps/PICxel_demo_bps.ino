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

#define number_of_LEDs 15
#define LED_pin 18                // Use pin 18 for RGB Station board
#define millisecond_delay 50

//PICxel constructor(uint8_t # of LEDs, uint8_t pin #, color_mode GRB or HSV);
PICxel strip(number_of_LEDs, LED_pin);

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
		delay(millisecond_delay);
	}
	
	for (int i = strip.getNumberOfLEDs(); i >= 0; i--)
	{
		strip.GRBsetLEDColor(i, 0, 255, 0);
		strip.refreshLEDs();
		delay(millisecond_delay);
	}
}
