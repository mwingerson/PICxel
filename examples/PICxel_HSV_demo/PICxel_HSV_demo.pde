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
#define LED_pin 0	 
#define millisecond_delay 10

PICxel strip(number_of_LEDs, LED_pin, HSV);

int hue = 0;
int sat = 0;
int val = 0;
int prevTime = 0;

void setup(){
	strip.begin();
}

void loop(){
	if(millis()-prevTime > millisecond_delay){
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
		
		for(int i= 0; i<number_of_LEDs; i++){
		if(i < number_of_LEDs/3)
			strip.HSVsetLEDColor(i, hue, 255, 50);
		else if(i < (2*number_of_LEDs)/3)
			strip.HSVsetLEDColor(i, 0, 255, val);
		else
			strip.HSVsetLEDColor(i, 0, sat,  100);
		}
	}
strip.refreshLEDs();
delay(1);
}


