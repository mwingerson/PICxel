/************************************************************************/
/*  PICxel.h  - PIC32 Neopixel Library                                  */
/*                                                                      */
/*  A simple to use library for addressable LEDs like the WS2812 for    */
/*  the PIC32 line of microcontrollers.                                 */
/*                                                                      */
/*  tested supported boards:                                            */
/*    - Digilent UNO32                                                  */
/*    - Digilent UC32                                                   */
/*                                                                      */
/*  This library is protected under the GNU GPL v3.0 license            */
/*  http://www.gnu.org/licenses/                                        */
/*                                                                      */
/* 06/05/2017 Brian Schmalz Added support for all chipKIT boards, as    */
/*                          well as simplifying the main GRBrefreshLEDs()*/
/*                          function.                                   */
/*                          Tested on 40, 48, 80 and 200 MHz boards     */
/************************************************************************/
#ifndef PICxel_H
#define PICxel_H

#include <WProgram.h>
#include <stdint.h>

#define BYTE uint8_t

enum color_mode_t {GRB, HSV};
enum memory_mode_t {alloc, noalloc};

class PICxel{
public:
//PICxel constructor and destructor
  PICxel(uint16_t num, uint8_t pin, color_mode_t color_mode);
  PICxel(uint16_t num, uint8_t pin, color_mode_t colorMode, memory_mode_t memory_mode);


  void setArrayPointer(uint8_t* colorPtr);

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
  bool setNumberOfLEDs(uint16_t);
  uint8_t* getColorArray(void);
  uint8_t getBrightness(void);


//pin control variables 
  uint8_t pin;
  volatile uint32_t *portSet;
  volatile uint32_t *portClr;
  uint32_t pinMask;


private:

//colorArray variables
  color_mode_t colorMode;
  uint16_t numberOfLEDs;
  uint16_t numberOfBytes;
  uint8_t brightness; 
  uint8_t *colorArray;
  memory_mode_t memoryMode;

  
};
#endif // PICxel

/* Hard coded delays
 * See the GRBrefreshLEDs() function in PICxel.cpp for how these are used.
 * Even though it is very bad form to hard code assembly nop delays like this, there just aren't enough 
 * cycles (even at 40MHz - slowest chipKIT board) to do anything else and still meet timing.
 * The key time for Neopixels is T0H, the time of a 0 bit high pulse. That has to be
 * between 200ns and 500ns, no more. We need it to be on the short side of that because
 * when doing hardware debugging in MPALB X, the debugger appears to steal some cycles
 * every now and then, and will extend this pulse a bit. 
 * Now, at 40MHz, the core timer runs at 20MHz, and so 200ns is only 4 core timer
 * ticks, or 8 instructions. That's not enough time to make the decision, check the
 * core timer, and decide when to exit the loop waiting for the core timer to be
 * equal to a value. So hard coded nops it is.
 * Of course, this means there are different numbers of nops that are needed for
 * each possible chipKIT core speed. At this point, I believe we have 40MHz, 48MHz,
 * 80MHz and 200Mhz chipKIT boards. So those values will be used, at compile time,
 * to decide which group of nop macros get used. Unfortunately this does not take
 * into account changes to the core speed at runtime. Maybe that can be a future
 * upgrade to the PICxel library.
 * 
 * It's counter intuitive that some of these delays have no instructions in them.
 * The reason it works out is because of the other overhead in the loop code that
 * takes up time, so the delays don't need to really be as long.
 * 
 * See https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
 * for a reference on this timing topic.
 * 
 * Since the two types of LEDs (WS2812/WS2812S and WS2812B) have slightly different
 * timing requirements, we try to split the difference here. Our goals are:
 * T0H =  220 ns (on the short side so we can debug)
 * T0L = 1000 ns
 * T1H =  800 ns
 * T1L =  350 ns
 * 
 * All of these require the optimization level to be at -O2 (default for Arduino IDE)
 *
 * A nice optimization would be to determine the minimum F_CPU frequency where the
 * core timer method could be used, and use it for any frequency above that, and use
 * the hard coded nops for all frequencies below.
 */
#if F_CPU == 40000000L
    //  220 ns
    #define GRB_delay_T0H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n ");}
    // 1000 ns
    #define GRB_delay_T0L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  800 ns
    #define GRB_delay_T1H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}
    //  350 ns
    #define GRB_delay_T1L(); {asm volatile("nop\n nop\n nop\n nop\n");}
#elif F_CPU == 48000000L
    //  220 ns
    #define GRB_delay_T0H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  980 ns
    #define GRB_delay_T0L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  810 ns
    #define GRB_delay_T1H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}
    //  360 ns
    #define GRB_delay_T1L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n");}
#elif F_CPU == 80000000L
    //  220 ns
    #define GRB_delay_T0H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    // 1000 ns
    #define GRB_delay_T0L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  800 ns
    #define GRB_delay_T1H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  350 ns
    #define GRB_delay_T1L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
#elif F_CPU == 200000000L
    //  220 ns
    #define GRB_delay_T0H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    // 1000 ns
    #define GRB_delay_T0L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  800 ns
    #define GRB_delay_T1H(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
    //  350 ns
    #define GRB_delay_T1L(); {asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n ");}
#else
    #warning F_CPU is not defined to a known value. PICxcel library not able to create delays properly.
    #define GRB_delay_T0H();
    #define GRB_delay_T0L();
    #define GRB_delay_T1H();
    #define GRB_delay_T1L();
#endif

/* Note, these need to be measured and converted so F_CPU selects the right set, 
 * as we do above. */
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
