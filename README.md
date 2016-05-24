PIC32 Neopixel Library - PICxel                            

A simple to use library for addressable LEDs like the WS2812 for the 
PIC32 line of microcontrollers.  Originally this library was a basic 
port of the Adafruit NeoPixel library but has evolved to be its own 
unique library. The PICxel library takes advantage of the PIC32 
architecture and has two different operating modes for red-green-blue 
and hue-saturation-value.  

The most exciting feature to date, is the inline HSV to GRB conversion.  
This feature decreases memory usage and frees up most of the clock 
cycles used in the conversion.

A new feature allows for the user to manage their own memory, this is 
useful when using a lot of LEDs, 500+.

Much of this library is inspired by the excellent Adafruit Neopixel 
library made by Phil Burgess and the open source community. I owe 
them a great thanks. A lot of this library is a derivative of their 
excellent work including some functions that are verbatim. I tried 
to preserve the standard funcitonality of the original Neopixel
Library. My Dad always told me "If it isn't broken then why are 
you trying to fix it?" He never actually said that but definitely 
should have.  

The library was originally written by Marshall Wingerson.    

Big thanks to Adafruit for being awesome!

tested supported boards:
  - COMING SOON! Red Lemur
  - Digilent UNO32                                                  
  - Digilent UC32
  - chipKIT Fubarino SD

The PICxel library should work with any Arduino-compatible PIC32 based board operating at 80MHz.
