PIC32 Neopixel Library                                  

A simple to use library for addressable LEDs like the WS2812 for the 
PIC32 line of microcontrollers.  Originally this library was a basic 
port of the Adafruit NeoPixel library but has evolved to be its own 
unique library.  The PICxel library take advantage of the PIC32 
architecture and has two different operating modes for red-green-blue 
and hue-saturation-value.  

The most exciting feature to date, is the inline HSV to GRB conversion.  
This feature decreases memory usage and frees up most of the clock 
cycles used in the conversion.

Much of this library is inspired by the excellent Adafruit Neopixel 
library made by Phil Burgess and the open source community.  I 
decided to keep the library separate from the Adafruit Neopixel 
library because of the divergence between Arduino IDE and MPIDE.  
With that I owe a great thanks to Phil Burgess and the open source 
community did a great job on the Neopixel library.  A lot of this 
library is a derivative of their excellent work including some 
functions that are verbatim.  My Dad always told me "If it isn't 
broken then why are you trying to fix it?" He never actually said that 
but definitely should have.  

The library was originally written by Marshall Wingerson.    

Big thanks to Adafruit for being awesome!

tested supported boards:
  - Digilent UNO32                                                  
  - Digilent UC32                                                  

updates:
- 1/1/2015 - The library was reposted after I pulled it down.  I did 
not know enough about licensing and protection of IP.  I was concerned 
that my workplace's owning company could take ownership of my work that
I do in my own time.  Sorry for the inconvenience.    

