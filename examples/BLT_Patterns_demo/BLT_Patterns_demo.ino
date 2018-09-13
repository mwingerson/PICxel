/************************************************************************
 *  BLT_Patterns_demo.ino - BTL_Patterns demo using PICxel library
 *
 *  A simple to use library for addressable LEDs like the WS2812 for
 *  the PIC32 line of microcontrollers.
 *
 *  tested supported boards:
 *    - chipKIT UNO32
 *    - chipKIT UC32
 *
 *  This library is protected under the GNU GPL v3.0 license
 *  http://www.gnu.org/licenses/
 ************************************************************************/

#include <PICxel.h>
#define byte uint8_t

#define NUMBER_OF_LEDS          15
#define LED_PIN                  0

PICxel strip(NUMBER_OF_LEDS, LED_PIN, GRB);
int fxVars[50];
byte *imgPtr;

void setup(){
  strip.begin();
  strip.setBrightness(100);
}

void loop(){
  reConfetti();

  strip.refreshLEDs();

  delay(100);
}

void reConfetti(){
  byte *ptr;
  if(fxVars[0] == 0) { // Initialize effect?
    fxVars[1] = random(1536); // Random hue
    // Probability of adding a new sparkle.
    //fxVars[2] = 60 + random(50);
    fxVars[2] = random(50);
    // Rate to fade out pixel.
    //fxVars[3] = 2 + random(3);
    fxVars[3] = 2 + random(10);
    // Initialize the pattern to black LEDs.
    //ptr = &imgData[0];
    ptr = strip.getColorArray();
    for(long i=0; i<strip.getNumberOfLEDs(); i++) {
      *ptr++ = 0; 
      *ptr++ = 0; 
      *ptr++ = 0;
    }
    fxVars[0] = 1; // Effect initialized
  }

  //ptr = &imgData[0];
  ptr = strip.getColorArray();
  byte a,b,c;
  long color;
  for(long i=0; i<strip.getNumberOfLEDs(); i++) {
    a = *ptr++;
    b = *ptr++;
    c = *ptr++;
    if (a > byte(fxVars[3])) {
      a = a - byte(fxVars[3]);
    } 
    else {
      a = 0;
    }
    if (b > byte(fxVars[3])) {
      b = b - byte(fxVars[3]);
    } 
    else {
      b = 0;
    }
    if (c > byte(fxVars[3])) {
      c = c - byte(fxVars[3]);
    } 
    else {
      c = 0;
    }
    ptr--;
    ptr--;
    ptr--;
    
    if (random(fxVars[2]) == 0) {
      color = hsv2rgb(random(1536), 255, strip.getBrightness());  // Random hue
      //color = strip.GRBToScaledColor(random(255), random(255), random(255));  // Random hue
      *ptr++ = color >> 16; 
      *ptr++ = color >> 8; 
      *ptr++ = color;
    }
    else {
      *ptr++ = a;
      *ptr++ = b;
      *ptr++ = c;
    }
  }
}

void reUSAConfetti(){
  byte *ptr;
  if(fxVars[0] == 0) { // Initialize effect?
    fxVars[1] = random(1536); // Random hue
    // Probability of adding a new sparkle.
    fxVars[2] = 60 + random(50);
    // Rate of fade out.
    fxVars[3] = 2;
    ptr = strip.getColorArray();
    for(long i=0; i<strip.getNumberOfLEDs(); i++) {
      *ptr++ = 0; 
      *ptr++ = 0; 
      *ptr++ = 0;
    }
    fxVars[0] = 1; // Effect initialized
  }

  ptr = strip.getColorArray();
  byte a,b,c,usa;
  for(long i=0; i<strip.getNumberOfLEDs(); i++) {
    if (random(fxVars[2]) == 0) {
      usa = random(3);
      if (usa == 0) { // Red
        *ptr++ = 0;
        *ptr++ = 255;
        *ptr++ = 0;
      } else if (usa == 1) { // White
        *ptr++ = 200;
        *ptr++ = 200;
        *ptr++ = 200;
      } else { // Blue
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 255;
      }
    } 
    else {
      a = *ptr++;
      b = *ptr++;
      c = *ptr++;
      if (a > byte(fxVars[3])) {
        a = a - byte(fxVars[3]);
      } 
      else {
        a = 0;
      }
      if (b > byte(fxVars[3])) {
        b = b - byte(fxVars[3]);
      } 
      else {
        b = 0;
      }
      if (c > byte(fxVars[3])) {
        c = c - byte(fxVars[3]);
      } 
      else {
        c = 0;
      }
      ptr--;
      ptr--;
      ptr--;	  
      *ptr++ = a;
      *ptr++ = b;
      *ptr++ = c;
    }
  }
}

void reBeadChase(){
  byte *ptr;
  if(fxVars[0] == 0) { // Initialize effect?
    fxVars[1] = 6 + random(4); // Number of beads
    fxVars[2] = 4 + random(8); // Fade out speed
    for(long i=0; i<long(fxVars[1]); i++) {
      fxVars[3+i*3] = random(1536); // Random hue
      fxVars[3+i*3+1] = 3 + random(8); // Speed
      // Positions are represented on a number scale 20*strip.getNumberOfLEDs()
      fxVars[3+i*3+2] = random(20*strip.getNumberOfLEDs());
      // Reverse direction half the time.
      if(random(2) == 0) fxVars[3+i*3+1] = -fxVars[3+i*3+1];
    }
    ptr = strip.getColorArray();
    for(long i=0; i<strip.getNumberOfLEDs(); i++) {
      *ptr++ = 0; 
      *ptr++ = 0; 
      *ptr++ = 0;
    }
    fxVars[0] = 1; // Effect initialized
  }

  ptr = strip.getColorArray();
  byte a,b,c;
  long color;
  boolean found = false;
  // Dim all the pixels on the belt before placing beads with full
  // brightness at their current locations.
  for(long i=0; i<strip.getNumberOfLEDs(); i++) {
    a = *ptr++;
    b = *ptr++;
    c = *ptr++;

    if (a > 150) {
      a = 150;
    }
    if (a > byte(fxVars[2])) {
      a = a - byte(fxVars[2]);
    } 
    else {
      a = 0;
    }

    if (b > 150) {
      b = 150;
    }
    if (b > byte(fxVars[2])) {
      b = b - byte(fxVars[2]);
    } 
    else {
      b = 0;
    }

    if (c > 150) {
      c = 150;
    }
    if (c > byte(fxVars[2])) {
      c = c - byte(fxVars[2]);
    } 
    else {
      c = 0;
    }
    ptr--;
    ptr--;
    ptr--;
    *ptr++ = a;
    *ptr++ = b;
    *ptr++ = c;
  }

    ptr = strip.getColorArray();
  for (long i=0; i<strip.getNumberOfLEDs(); i++)
  {
    // Using bool 'found' to ensure that the ptr is only
    // incremented once during each iteration of the
    // outer loop
    found = false;
    for (long j=0; j<long(fxVars[1]); j++) {
      if (!found) {
        if (i == fxVars[3+j*3+2]/20) {
          color = hsv2rgb(fxVars[3+j*3], 255, 255);
		  
          *ptr++ = ((color&0xFF0000) >> 16); 
          *ptr++ = ((color&0x00FF00) >> 8); 
          *ptr++ = (color&0x0000FF);
          found = true;
        }
      }
    }

    if (!found) {
      ptr++;
      ptr++;
      ptr++;
    }
  }

  // Update positions for all the beads
  for (long j=0; j<long(fxVars[1]); j++) {
    // Position + Speed mod by 20x length of the belt
    fxVars[3+j*3+2] = (fxVars[3+j*3+2] + fxVars[3+j*3+1]) % (20*strip.getNumberOfLEDs());
    if (fxVars[3+j*3+2] < 0) {
      fxVars[3+j*3+2] = (20*strip.getNumberOfLEDs() - 1) - fxVars[3+j*3+2];
    }
  }
}

// Rainbow effect (1 or more full loops of color wheel at 100% saturation).
// Not a big fan of this pattern (it's way overused with LED stuff), but it's
// practically part of the Geneva Convention by now.
void reRainbowWrap(){
  if(fxVars[0] == 0) { // Initialize effect?
    // Number of repetitions (complete loops around color wheel); any
    // more than 4 per meter just looks too chaotic and un-rainbow-like.
    // Store as hue 'distance' around complete belt:
    fxVars[1] = (1 + random(3 * ((strip.getNumberOfLEDs() + 31) / 32))) * 1536;
    // Frame-to-frame hue increment (speed) -- may be positive or negative,
    // but magnitude shouldn't be so small as to be boring.  It's generally
    // still less than a full pixel per frame, making motion very smooth.
    fxVars[2] = 10 + random(fxVars[1]) / strip.getNumberOfLEDs();
    // Reverse speed and hue shift direction half the time.
    if(random(2) == 0) fxVars[1] = -fxVars[1];
    if(random(2) == 0) fxVars[2] = -fxVars[2];
    fxVars[3] = 0; // Current position
    fxVars[0] = 1; // Effect initialized
  }

  byte *ptr = strip.getColorArray();
  long color, i;
  for(i=0; i<strip.getNumberOfLEDs(); i++) {
   color = hsv2rgb(fxVars[3] + fxVars[1] * i / strip.getNumberOfLEDs(), 255, 255);

    *ptr++ = color >> 16;
    *ptr++ = color >> 8;
    *ptr++ = color;
  }
  fxVars[3] += fxVars[2];
}

void reDigilentConfetti(){
  byte *ptr;
  if(fxVars[0] == 0) { // Initialize effect?
    fxVars[1] = random(1536); // Random hue
    // Probability of adding a new sparkle.
    fxVars[2] = 60 + random(50);
    fxVars[3] = 2 + random(3);
    ptr = strip.getColorArray();
    for(long i=0; i<strip.getNumberOfLEDs(); i++) {
      *ptr++ = 0; 
      *ptr++ = 0; 
      *ptr++ = 0;
    }
    fxVars[0] = 1; // Effect initialized
  }

  ptr = strip.getColorArray();
  byte a,b,c;
  for(long i=0; i<strip.getNumberOfLEDs(); i++) {
    if (random(fxVars[2]) == 0) {
      if (random(3) == 0) {
		uint8_t white = 200;
        *ptr++ = white;
        *ptr++ = white;
        *ptr++ = white;
      } 
      else {
        *ptr++ = 255;
        *ptr++ = 0;
        *ptr++ = 0;
      }
    } 
    else {
      a = *ptr++;
      b = *ptr++;
      c = *ptr++;
      if (a > byte(fxVars[3])) {
        a = a - byte(fxVars[3]);
      } 
      else {
        a = 0;
      }
      if (b > byte(fxVars[3])) {
        b = b - byte(fxVars[3]);
      } 
      else {
        b = 0;
      }
      if (c > byte(fxVars[3])) {
        c = c - byte(fxVars[3]);
      } 
      else {
        c = 0;
      }
      ptr--;
      ptr--;
      ptr--;
      *ptr++ = a;
      *ptr++ = b;
      *ptr++ = c;
    }
  }
}

void reDigilentSolidTwinkle(){
  byte *ptr, a, b, c;
  if(fxVars[0] == 0) { // Initialize effect?
    fxVars[1] = random(1536); // Random hue
    // Probability of adding a new sparkle.
    fxVars[2] = 20 + random(50);
    // Rate of fade out to background color.
    fxVars[3] = 7 + random(10);
    fxVars[4] = 255;//10;
    fxVars[5] = 0;//240;
    fxVars[6] = 0;
    ptr = strip.getColorArray();
    for(long i=0; i<strip.getNumberOfLEDs(); i++) {
      *ptr++ = byte(fxVars[4]);
      *ptr++ = byte(fxVars[5]);
      *ptr++ = byte(fxVars[6]);
    }
    fxVars[0] = 1; // Effect initialized
  }

  ptr = strip.getColorArray();
  for(long i=0; i<strip.getNumberOfLEDs(); i++) {
    if (random(fxVars[2]) == 0) {
		uint8_t white= 255;
      *ptr++ = white;
      *ptr++ = white;
      *ptr++ = white;
    } 
    else {
      a = *ptr++;
      b = *ptr++;
      c = *ptr++;
      if (a < (byte(fxVars[4]) - byte(fxVars[3]))) {
        a = a + byte(fxVars[3]);
      } 
      else if (a > (byte(fxVars[4]) + byte(fxVars[3]))) {
        a = a - byte(fxVars[3]);
      } 
      else {
        a = byte(fxVars[4]);
      }
      if (b < (byte(fxVars[5]) - byte(fxVars[3]))) {
        b = b + byte(fxVars[3]);
      } 
      else if (b > (byte(fxVars[5]) + byte(fxVars[3]))) {
        b = b - byte(fxVars[3]);
      } 
      else {
        b = byte(fxVars[5]);
      }
      if (c < (byte(fxVars[6]) - byte(fxVars[3]))) {
        c = c + byte(fxVars[3]);
      } 
      else if (c > (byte(fxVars[6]) + byte(fxVars[3]))) {
        c = c - byte(fxVars[3]);
      } 
      else {
        c = byte(fxVars[6]);
      }
      ptr--;
      ptr--;
      ptr--;
      *ptr++ = a;
      *ptr++ = b;
      *ptr++ = c;
    }
  }
}

  // The fixed-point sine and cosine functions use marginally more
  // conventional units, equal to 1/2 degree (720 units around full circle),
  // chosen because this gives a reasonable resolution for the given output
  // range (-127 to +127).  Sine table intentionally contains 181 (not 180)
  // elements: 0 to 180 *inclusive*.  This is normal.
  byte sineTable[181]  = {
    0,  1,  2,  3,  5,  6,  7,  8,  9, 10, 11, 12, 13, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30, 31, 32, 33, 34,
    35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
    67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 77, 78, 79, 80, 81,
    82, 83, 83, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 92, 93, 94,
    95, 95, 96, 97, 97, 98, 99,100,100,101,102,102,103,104,104,105,
    105,106,107,107,108,108,109,110,110,111,111,112,112,113,113,114,
    114,115,115,116,116,117,117,117,118,118,119,119,120,120,120,121,
    121,121,122,122,122,123,123,123,123,124,124,124,124,125,125,125,
    125,125,126,126,126,126,126,126,126,127,127,127,127,127,127,127,
    127,127,127,127,127
  };

  // ---------------------------------------------------------------------------
  // Assorted fixed-point utilities below this line.  Not real interesting.
  // Gamma correction compensates for our eyes' nonlinear perception of
  // intensity.  It's the LAST step before a pixel value is stored, and
  // allows intermediate rendering/processing to occur in linear space.
  // The table contains 256 elements (8 bit input), though the outputs are
  // only 7 bits (0 to 127).  This is normal and intentional by design: it
  // allows all the rendering code to operate in the more familiar unsigned
  // 8-bit colorspace (used in a lot of existing graphics code), and better
  // preserves accuracy where repeated color blending operations occur.
  // Only the final end product is converted to 7 bits, the native format
  // for the LPD8806 LED driver.  Gamma correction and 7-bit decimation
  // thus occur in a single operation.
  byte gammaTable[]  = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,
    4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  7,  7,
    7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11,
    11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16,
    16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
    23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30,
    30, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39,
    40, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50,
    50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62,
    62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 74, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
    92, 93, 94, 95, 96, 97, 98, 99,100,101,102,104,105,106,107,108,
    109,110,111,113,114,115,116,117,118,120,121,122,123,125,126,127
  };

// This function (which actually gets 'inlined' anywhere it's called)
// exists so that gammaTable can reside out of the way down here in the
// utility code...didn't want that huge table distracting or intimidating
// folks before even getting into the real substance of the program, and
// the compiler permits forward references to functions but not data.
inline byte gamma(byte x) {
  return gammaTable[x];
}

// Fixed-point colorspace conversion: HSV (hue-saturation-value) to RGB.
// This is a bit like the 'Wheel' function from the original strandtest
// code on steroids.  The angular units for the hue parameter may seem a
// bit odd: there are 1536 increments around the full color wheel here --
// not degrees, radians, gradians or any other conventional unit I'm
// aware of.  These units make the conversion code simpler/faster, because
// the wheel can be divided into six sections of 256 values each, very
// easy to handle on an 8-bit microcontroller.  Math is math, and the
// rendering code elsehwere in this file was written to be aware of these
// units.  Saturation and value (brightness) range from 0 to 255.
long hsv2rgb(long h, byte s, byte v) {
  byte r, g, b, lo;
  int  s1;
  long v1;

  // Hue
  h %= 1536;           // -1535 to +1535
  if(h < 0) h += 1536; //     0 to +1535
  lo = h & 255;        // Low byte  = primary/secondary color mix
  switch(h >> 8) {     // High byte = sextant of colorwheel
  case 0 : 
    r = 255     ; 
    g =  lo     ; 
    b =   0     ; 
    break; // R to Y
  case 1 : 
    r = 255 - lo; 
    g = 255     ; 
    b =   0     ; 
    break; // Y to G
  case 2 : 
    r =   0     ; 
    g = 255     ; 
    b =  lo     ; 
    break; // G to C
  case 3 : 
    r =   0     ; 
    g = 255 - lo; 
    b = 255     ; 
    break; // C to B
  case 4 : 
    r =  lo     ; 
    g =   0     ; 
    b = 255     ; 
    break; // B to M
  default: 
    r = 255     ; 
    g =   0     ; 
    b = 255 - lo; 
    break; // M to R  
  }

  // Saturation: add 1 so range is 1 to 256, allowing a quick shift operation
  // on the result rather than a costly divide, while the type upgrade to int
  // avoids repeated type conversions in both directions.
  s1 = s + 1;
  r = 255 - (((255 - r) * s1) >> 8);
  g = 255 - (((255 - g) * s1) >> 8);
  b = 255 - (((255 - b) * s1) >> 8);

  // Value (brightness) and 24-bit color concat merged: similar to above, add
  // 1 to allow shifts, and upgrade to long makes other conversions implicit.
  v1 = v + 1;
  //return (((r * v1) & 0xff00) << 8)|((g * v1) & 0xff00)|((b * v1)>> 8);
  return (((((g * v1) & 0xff00) << 8)|((r * v1) & 0xff00)))|((b * v1)>> 8);
}

char fixSin(int angle) {
  angle %= 720;               // -719 to +719
  if(angle < 0) angle += 720; //    0 to +719
  return (angle <= 360) ?
    sineTable[(angle <= 180) ?
    angle          : // Quadrant 1
    (360 - angle)] : // Quadrant 2
  -sineTable[(angle <= 540) ?
  (angle - 360)   : // Quadrant 3
  (720 - angle)] ; // Quadrant 4
}

char fixCos(int angle) {
  angle %= 720;               // -719 to +719
  if(angle < 0) angle += 720; //    0 to +719
  return (angle <= 360) ?
  ((angle <= 180) ?  sineTable[180 - angle]  : // Quad 1
  -sineTable[angle - 180]) : // Quad 2
  ((angle <= 540) ? -sineTable[540 - angle]  : // Quad 3
  sineTable[angle - 540]) ; // Quad 4
}
