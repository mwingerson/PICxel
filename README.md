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
to preserve the standard functionality of the original Neopixel
Library. My Dad always told me "If it isn't broken then why are 
you trying to fix it?" He never actually said that but definitely 
should have.

The library was originally written by Marshall Wingerson.

Big thanks to Adafruit for being awesome!

tested supported boards:
  - chipKIT UNO32
  - chipKIT UC32
  - chipKIT Fubarino SD
  - chipKIT Fubarino Mini
  - chipKIT WiFire
  - RGB Station

The PICxel library should work with any Arduino-compatible PIC32 
based board running at 40MHz, 48MHz, 80MHz or 200MHz (which is 
currently all of them).

(09/05/2018)
Version 2 changes by Brian Schmalz : brian@schmalzhaus

Version 2 brings per-pixel brightness ability to RGB mode as well as a simpler
constructor with default parameters. All previous sketches will still work with
no modifications. More examples added as well.

NOTE: HSV mode does not need per-pixel brightness, since you get that with the
'V' parameter (value) in HSV. 

NOTE2: HSV mode does NOT work properly with chipKIT boards that are not
running at 80MHz. (So, like, Fubarino Mini, DP32, WiFire, etc.) This will
be addressed in a future version of the library. HSV mode does work just 
fine for all 80MHz chipKIT boards (I.e. Fubarino SD, uC32, UNO32, MAX32,
etc.)
