/************************************************************************/
/*  PICxel_demo.pde - PIC32 Neopixel Library Demo                       */
/*                                                                      */
/*  A simple to use library for addressable LEDs like the WS2812 for    */
/*  the PIC32 line of microcontrollers.                                 */
/*                                                                      */
/*  Here is a port of the Adafruit neopixel library specifically for    */
/*  the chipkit line of microcontrollers from Digilent. I decided to    */
/*  keep the library separate from the Adafruit Neopixel library        */
/*  because of the divergence between Arduino IDE and MPIDE.  With that */ 
/*  Phil Burgess and the open source community did a great job on the   */
/*  Neopixel library and a lot of this library is a derivative of their */
/*  excellent work including some functions that are verbatim.  My Dad  */
/*  always told me "If it isn't broken then why are you trying to fix   */
/*  it?" He never actually said that but definitely should have.        */
/*                                                                      */
/*  This library was written by Marshall Wingerson.  Please enjoy the   */
/*  library.  Big thanks to Adafruit for being awesome.                 */
/*                                                                      */
/*  Demo Description: In this demo the LED strip fill from the first    */
/*  LED to the final LED with green then fills from the final LED to    */
/*  the first LED with red.  Then the pattern repeats.                  */
/*                                                                      */
/*  tested supported boards:                                            */
/*    - Digilent UNO32                                                  */
/*    - Digilent UC32                                                   */
/*                                                                      */
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or modify 
  it under the terms of the GNU Lesser General Public License as 
  published by the Free Software Foundation; either version 2.1 of the 
  License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public 
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA
*/

#include <PICxel.h>

#define number_of_LEDs 15
#define LED_pin 0	 
#define millisecond_delay 50

//PICxel constructor(uint8_t # of LEDs, uint8_t pin #, color_mode GRB or HSV);
PICxel strip(number_of_LEDs, LED_pin, GRB);

void setup(){
	strip.begin();
	strip.setBrightness(30);
	strip.clear();
}

void loop(){
	for(int i=0; i<strip.getNumberOfLEDs(); i++){
		strip.GRBsetLEDColor(i, 255, 0, 0);
		strip.refreshLEDs();
		delay(millisecond_delay);
	}
	
	for(int i=strip.getNumberOfLEDs(); i>=0; i--){
		strip.GRBsetLEDColor(i, 0, 255, 0);
		strip.refreshLEDs();
		delay(millisecond_delay);
	}
}



















