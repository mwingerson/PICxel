/************************************************************************/
/*  PICxel_2_strand_demo.pde - PIC32 Neopixel Library Demo              */
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

#define number_of_LEDs_strip1 15
#define LED_pin_strip1 0	 

#define number_of_LEDs_strip2 15
#define LED_pin_strip2 1	 

#define millisecond_delay_strip 50

PICxel strip1(number_of_LEDs_strip1, LED_pin_strip1, HSV);
PICxel strip2(number_of_LEDs_strip2, LED_pin_strip2, HSV);

int hue = 0;
int sat = 255;
int val = 100;
int prevTime = 0;
int runner_num = 0;
int count_up_flag = 0;
int runner_delay_count = 0;
int runner_hue = 0;

void setup(){
	strip1.begin();
	//strip.setBrightness(100);	//brightness is not needed in HSV
	
	strip2.begin();
	strip2.setBrightness(100);
}

void loop(){
	if(millis()-prevTime > 10){
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
		
		for(int i= 0; i<number_of_LEDs_strip1; i++){
		if(i < number_of_LEDs_strip1/3)
			strip1.HSVsetLEDColor(i, hue, 255, 50);
		else if(i < 2*(number_of_LEDs_strip1/3))
			strip1.HSVsetLEDColor(i, 0, 255, val);
		else
			strip1.HSVsetLEDColor(i, 0, sat,  100);
		}
		
		if(runner_delay_count == 5){
			if(count_up_flag == 0){
				strip2.HSVsetLEDColor(runner_num, runner_hue++, 255, 100);
				strip2.clear(runner_num-1);
				runner_num++;
				if(runner_num == number_of_LEDs_strip2)
					count_up_flag = 1;
			}
			else{
				strip2.HSVsetLEDColor(runner_num, runner_hue++, 255, 100);
				strip2.clear(runner_num+1);
				runner_num--;
				if(runner_num == -1)
					count_up_flag = 0;
			}
			if(runner_hue == 1535)
				runner_hue = 0;
			runner_delay_count = 0;
		}
		runner_delay_count++;	
	}
strip1.refreshLEDs();
strip2.refreshLEDs();
delay(1);
}
