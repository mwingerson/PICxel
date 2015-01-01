/************************************************************************/
/*  PICxel.h  - PIC32 Neopixel Library                                  */
/*																		*/
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
/*  it?" He never actually said that but definitely should have.  		*/
/*                                                                      */
/*  This library was written by Marshall Wingerson.  Please enjoy the   */
/*  library.  Big thanks to Adafruit for being awesome.                 */
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

#ifndef PICxel_H
#define PICxel_H

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#include <WProgram.h>
#include <stdint.h>

#define BYTE uint8_t

enum color_mode_t {GRB, HSV};

class PICxel{
public:
//PICxel constructor and destructor
	PICxel(uint16_t num, uint8_t pin, color_mode_t color_mode);
	~PICxel(void);

//PICxel control functions
	void begin(void);
	void refreshLEDs(void);
	void GRBrefreshLEDs(void);
	void HSVrefreshLEDs(void);
	
	void GRBsetLEDColor(uint16_t number, uint8_t green, uint8_t red, uint8_t blue);
	void GRBsetLEDColor(uint16_t number, uint32_t color);
	
	void HSVsetLEDColor(uint16_t number, uint16_t hue, uint8_t sat, uint8_t val);
	void HSVsetLEDColor(uint16_t number, uint32_t color);

	void clear();
	void clear(uint8_t num);

	uint32_t colorToScaledColor(uint32_t color);
	uint32_t HSVToColor(unsigned int HSV);

//set class variable functions 
	void setBrightness(uint8_t b);	

//get class variable functions
	uint16_t getNumberOfLEDs(void);
	uint8_t* getColorArray(void);
	uint8_t getBrightness(void);

private:
//pin control variables	
	uint8_t pin;
	volatile uint32_t *portSet;
	volatile uint32_t *portClr;
	uint32_t pinMask;

//colorArray variables
	color_mode_t colorMode;
	uint16_t numberOfLEDs;
	uint16_t numberOfBytes;
	uint8_t brightness;	
	uint8_t *colorArray;

	
};
#endif // PICxel

//preprocessor defined delays for readability
//GRB delay one
#define GRB_delay_one(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}

//GRB delay two
#define GRB_delay_two(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}

//GRB delay three
#define GRB_delay_three(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}

#define HSV_bit_0_delay_0 "nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_0_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_0_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_0_delay_3 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_bit_1_delay_0 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_1_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_1_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_bit_2_delay_0 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_2_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_2_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_bit_3_delay_0 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_3_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_3_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_bit_4_delay_0 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_4_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_4_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_bit_5_delay_0 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_5_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_bit_5_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"

#define HSV_universal_delay_1 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_universal_delay_2 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
#define HSV_universal_delay_3 "nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"
