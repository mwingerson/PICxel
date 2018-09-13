/************************************************************************/
/*  PICxel.cpp  - PIC32 Neopixel Library                                */
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
/*                          well as simplifying the main                */
/*                          GRBrefreshLEDs() function.                  */
/*                          Tested on 40, 48, 80 and 200 MHz boards     */
/* 07/28/2017 Brian Schmalz Reformatted to ease readability             */
/*                          Added more comments, function header blocks */
/*                          Added support for per-pixel brightness      */
/*                            values by adding per-pixel brightness     */
/*                            array, new functions to support setting   */
/*                            it, and new array for storing original    */
/*                            color values                              */
/*                                                                      */
/* TODO: Update HSV delays for F_CPU other than 80MHz                   */
/************************************************************************/

#include "PICxel.h"

/************************************************************************/
/*  Constructor for the PICxel class                                    */
/*  Note the optional parameters in the PICxel.h file for this function */
/*  If you only include the minimum (pixelCount and pin) then you'll    */
/*  get a colorMode of GRB, a memoryMode of alloc and a brightnessMode  */
/*  of globalOnly. You can override any of those parameters if you want */
/************************************************************************/
PICxel::PICxel(
  uint16_t pixelCount,
  uint8_t pixelPin,
  color_mode_t newColorMode,
  memory_mode_t newMemoryMode,
  brightness_mode_t newBrightnessMode,
  uint8_t* colorArrayPtr,
  uint8_t* originalColorArrayPtr,
  uint8_t* pixelBrightnessArrayPtr
) :
  brightnessMode(newBrightnessMode),
  colorMode(newColorMode),
  memoryMode(newMemoryMode),
  colorArraySizeBytes(0),
  globalBrightness(0xFF),
  colorArray(NULL),
  originalColorArray(NULL),
  pixelBrightnessArray(NULL),
  pin(pixelPin),
  portSet(portOutputRegister(digitalPinToPort(pixelPin)) + 2),
  portClr(portOutputRegister(digitalPinToPort(pixelPin)) + 1),
  pinMask(digitalPinToBitMask(pixelPin))
{
  if (memoryMode == alloc)
  {
    // User is asking us to allocate the pixel color arrays using calloc
    if (colorMode == GRB)
    {
      colorArraySizeBytes =  3 * pixelCount;
      colorArray = (uint8_t *)calloc(colorArraySizeBytes, sizeof(uint8_t));

      // Only allocate extra two arrays if user wants per-pixel brightness
      if (brightnessMode == perPixel)
      {
        originalColorArray = (uint8_t *)calloc(colorArraySizeBytes, sizeof(uint8_t));
        pixelBrightnessArray = (uint8_t *)calloc(pixelCount, sizeof(uint8_t));
      }
    }
    else  // colorMode == HSV
    {
      colorArraySizeBytes = 4 * pixelCount;
      colorArray = (uint8_t *)calloc(colorArraySizeBytes, sizeof(uint8_t));
      // Only allocate extra two arrays if user wants per-pixel brightness
      if (brightnessMode == perPixel)
      {
        // TODO: Handle the HSV version of allocating originalColorArray
        // and pixelBrightnessArray
      }
    }
  }
  else // memoryMode == noalloc
  {
    // User is passing in pre-allocated arrays for pixel color arrays,
    // If non-NULL, copy over into our member array pointers.
    if (colorArrayPtr != NULL)
    {
      colorArray = colorArrayPtr;
    }
    if (originalColorArrayPtr != NULL)
    {
      originalColorArray = originalColorArrayPtr;
    }
    if (pixelBrightnessArrayPtr != NULL)
    {
      pixelBrightnessArray = pixelBrightnessArrayPtr;
    }

    if (colorMode == GRB)
    {
      colorArraySizeBytes =  3 * pixelCount;
    }
    else  // colorMode == HSV
    {
      colorArraySizeBytes = 4 * pixelCount;
    }
  }

  // Handle the case where we've run out of memory
  if (
    colorArray == NULL 
    || 
    (
      brightnessMode == perPixel
      && 
      (
        originalColorArray == NULL 
        || 
        pixelBrightnessArray == NULL
      )
    )
  )
  {
    // By setting the number of LEDs to zero, we prevent any of the rest of the code from
    // really doing much, and thus alerting the user to something wrong
    numberOfLEDs = 0;
    colorArraySizeBytes = 0;

    // Deallocate any memory that did get allocated (only by us - lave user allocation alone)
    if (memoryMode == alloc)
    {
      if (colorArray != NULL)
      {
        free(colorArray);
      }
      if (originalColorArray != NULL)
      {
        free(originalColorArray);
      }
      if (pixelBrightnessArray != NULL)
      {
        free(pixelBrightnessArray);
      }
    }
  }
  else
  {
    // Indicate successful memory allocation by setting the global number of LEDs properly
    numberOfLEDs = pixelCount;
    
    // No need to init colorArray or originalColorArray as calloc sets them to zero
    
    // Init individual pixel brightnesses to maximum
    if (brightnessMode == perPixel)
    {
      memset((void *)pixelBrightnessArray, 0xFF, pixelCount);
    }
  }
}

/************************************************************************/
/*  Destructor for the PICxel class                                     */
/************************************************************************/
PICxel::~PICxel()
{
  if (memoryMode == alloc)
  {
    // Deallocate any memory that did got allocated
    if (colorArray != NULL)
    {
      free(colorArray);
    }
    if (originalColorArray != NULL)
    {
      free(originalColorArray);
    }
    if (pixelBrightnessArray != NULL)
    {
      free(pixelBrightnessArray);
    }
  }
}

/************************************************************************/
/*  Sets the pinMode for selected pin                                   */
/************************************************************************/
void PICxel::begin()
{
  if (colorArraySizeBytes)
  {
    // let chipKIT library handle the tri-state buffer and 
    // assign analog inputs as needed
    pinMode(pin, OUTPUT);
    
    // clear the pin
    *portClr = pinMask;
  }
}

/************************************************************************/
/*  Clears the entire color array in either GRB or HSV mode             */
/************************************************************************/
void PICxel::clear()
{
  if (colorArraySizeBytes)
  {
    if (colorMode == GRB)
    {
      if (colorArray != NULL)
      {
        memset((void *)colorArray, 0x00, colorArraySizeBytes);
      }
      if (originalColorArray != NULL)
      {
        memset((void *)originalColorArray, 0x00, colorArraySizeBytes);
      }
      if (pixelBrightnessArray != NULL)
      {
        memset((void *)pixelBrightnessArray, 0xFF, numberOfLEDs);
      }
    }
    else // colorMode == HSV
    {
      /// TODO: Handle the HSV case like we do the RGB case
      if (colorArray != NULL)
      {
        memset((void *)colorArray, 0x00, colorArraySizeBytes);
      }
    }
  }
}

/************************************************************************/
/*  Clears just one LED                                                 */
/************************************************************************/
void PICxel::clear(uint8_t pixelNumber)
{
  if (colorArraySizeBytes)
  {
    if (colorMode == GRB)
    {
      if (colorArray != NULL)
      {
        colorArray[(pixelNumber * 3) + 0] = 0;
        colorArray[(pixelNumber * 3) + 1] = 0;
        colorArray[(pixelNumber * 3) + 2] = 0;
      }
      if (originalColorArray != NULL)
      {
        originalColorArray[(pixelNumber * 3) + 0] = 0;
        originalColorArray[(pixelNumber * 3) + 1] = 0;
        originalColorArray[(pixelNumber * 3) + 2] = 0;
      }
      if (pixelBrightnessArray != NULL)
      {
        pixelBrightnessArray[pixelNumber] = 0xFF;
      }
    }
    else
    {
      /// TODO: Handle HSV originalColorArray and pixelBrightnessArray
      if (colorArray != NULL)
      {
        colorArray[(pixelNumber * 4) + 0] = 0;
        colorArray[(pixelNumber * 4) + 1] = 0;
        colorArray[(pixelNumber * 4) + 2] = 0;
        colorArray[(pixelNumber * 4) + 3] = 0;
      }
    }
  }
}

/************************************************************************/
/*  sets the class global brightness                                    */
/************************************************************************/
void PICxel::setBrightness(uint8_t newBrightness)
{
  globalBrightness = newBrightness;
}

/************************************************************************/
/*  Take the original color values for a given pixel, and create the    */
/*  actual color values that get sent out to LEDs by adjusting the      */
/*  originals for global brightness and per-pixel brightness.           */
/*  Note: This routine only needs to be called if per-pixel brightness  */
/*  is being used, and normally is only called from within this library */
/************************************************************************/
void PICxel::updatePixelColorValues(uint16_t number)
{
  if (colorArraySizeBytes && brightnessMode == perPixel)
  {
    uint8_t *originalArrayPtr = &originalColorArray[number * 3];
    uint8_t *arrayPtr = &colorArray[number * 3];
    
    // green
    arrayPtr[0] = ((originalArrayPtr[0] * pixelBrightnessArray[number] * globalBrightness) >> 16);
    // red
    arrayPtr[1] = ((originalArrayPtr[1] * pixelBrightnessArray[number] * globalBrightness) >> 16);
    // blue
    arrayPtr[2] = ((originalArrayPtr[2] * pixelBrightnessArray[number] * globalBrightness) >> 16);
  }
}

/************************************************************************/
/*  Modifies the color matrix with the colors presented in 8-bit values */
/*  for green, red and blue.  The values are then scaled by the         */
/*  globalBrightness value and placed into the location of number       */
/*  This function leaves the global and per-pixel brightness values     */
/*  alone.                                                              */
/************************************************************************/
void PICxel::GRBsetLEDColor(
    uint16_t pixelNumber,
    uint8_t green,
    uint8_t red,
    uint8_t blue,
    uint8_t pixelBrightness)
{
  if ((colorArraySizeBytes) && (pixelNumber < numberOfLEDs))
  {
    if (brightnessMode == perPixel)
    {
      // When we are in perPixel brightness mode, we store the original RGB 
      // values in the Original array, and then update the colorArray values 
      // from that based on per pixel brightness
      uint8_t *originalArrayPtr = &originalColorArray[pixelNumber * 3];

      originalArrayPtr[0] = green;
      originalArrayPtr[1] = red;
      originalArrayPtr[2] = blue;

      // Update the pixel's brightness value
      pixelBrightnessArray[pixelNumber] = pixelBrightness;
      
      /* Now compute new actual color values */
      updatePixelColorValues(pixelNumber);
    }
    else  // brightnessMode == globalOnly
    {
      // If normal global-only brightness is used, then dump the RGB values
      // directly into the colorArray after modifying by global brightness
      uint8_t *arrayPtr = &colorArray[pixelNumber * 3];

      arrayPtr[0] = (green * globalBrightness) >> 8;
      arrayPtr[1] = (red   * globalBrightness) >> 8;
      arrayPtr[2] = (blue  * globalBrightness) >> 8;
    }
  }
}

/************************************************************************/
/*  Modifies the color matrix with the colors presented one 32-bit      */
/*  value. color is a 32-bit unsigned int that is organized into four   */
/*  bytes:                                                              */
/*  bits[31 - 24][23 - 16][15 - 8][7 - 0]                               */
/*      ( blank )(  red  )(green )(blue )                               */
/*                                                                      */
/*  Each color is scaled by the globalBrightness value and stored in the */
/*  color array.                                                        */
/************************************************************************/
void PICxel::GRBsetLEDColor(uint16_t pixelNumber, uint32_t color, uint8_t pixelBrightness)
{
  if ((colorArraySizeBytes) && (pixelNumber < numberOfLEDs))
  {
    if (brightnessMode == perPixel)
    {
      // When we are in perPixel brightness mode, we store the original RGB 
      // values in the Original array, and then update the colorArray values 
      // from that based on per pixel brightness
      uint8_t *originalArrayPtr = &originalColorArray[pixelNumber * 3];
      
      originalArrayPtr[0] = ((uint8_t)(color >>16));
      originalArrayPtr[1] = ((uint8_t)(color >> 8));
      originalArrayPtr[2] = ((uint8_t)(color));

      // Update the pixel's brightness value
      pixelBrightnessArray[pixelNumber] = pixelBrightness;
      
      /* Now compute new actual color values */
      updatePixelColorValues(pixelNumber);
    }
    else  // brightnessMode == globalOnly
    {
      // If normal global-only brightness is used, then dump the RGB values
      // directly into the colorArray
      uint8_t *arrayPtr = &colorArray[pixelNumber * 3];
      
      arrayPtr[0] = ((uint8_t)(color >>16) * globalBrightness) >> 8;
      arrayPtr[1] = ((uint8_t)(color >> 8) * globalBrightness) >> 8;
      arrayPtr[2] = ((uint8_t)(color)      * globalBrightness) >> 8; 
    }
  }
}

/************************************************************************/
/*  Modifies the color matrix with the passed values hue, sat and val.  */
/*                                                                      */
/*  Hue is a 16-bit value that ranges from 0-1535 representing the      */
/*  color wheel.  255 values per 60 degrees of the color wheel.  sat    */
/*  val are 8-bit values that control saturation and value.  If         */
/*  saturation is 0 then the LED will be off.                           */
/************************************************************************/
void PICxel::HSVsetLEDColor(
    uint16_t pixelNumber,
    uint16_t hue,
    uint8_t sat,
    uint8_t val)
{
  if (pixelNumber < numberOfLEDs)
  {
    uint8_t *arrayPtr = &colorArray[pixelNumber * 4];
    arrayPtr[0] = hue;
    arrayPtr[1] = hue >> 8;
    arrayPtr[2] = sat;
    arrayPtr[3] = val;
  }
}

/************************************************************************/
/*  Modifies the color matrix with the colors presented one 32-bit      */
/*  value. color is a 32-bit unsigned int that is organized into four   */
/*  bytes:                                                              */
/*  bits[ 31 - 24 ][ 23 - 16 ][15 - 8][7 - 0]                           */
/*      (upper hue)(lower hue)( sat  )( val )                           */
/*                                                                      */
/*  Hue is a 16-bit value that ranges from 0-1535 representing the      */
/*  color wheel.  255 values per 60 degrees of the color wheel.  sat    */
/*  val are 8-bit values that control saturation and value.  If         */
/*  saturation is 0 then the LED will be off.                           */
/************************************************************************/
void PICxel::HSVsetLEDColor(uint16_t pixelNumber, uint32_t color)
{
  if (pixelNumber < numberOfLEDs)
  {
    uint8_t *arrayPtr = &colorArray[pixelNumber * 4];
    arrayPtr[0] = color;
    arrayPtr[1] = color >> 8;
    arrayPtr[2] = color >> 16;
    arrayPtr[3] = color >> 24;
  }
}


/************************************************************************/
/*  When in per-pixel brightness mode, this function will update a      */
/*  single pixel's brightness, and then re-compute it's RGB values so   */
/*  that the new brightness immediately takes effect at the next refresh*/
/************************************************************************/
void PICxel::setPixelBrightness(uint16_t pixelNumber, uint8_t pixelBrightness)
{
  if ((colorArraySizeBytes) && (pixelNumber < numberOfLEDs) && (brightnessMode == perPixel))
  {
    // Update the pixel's brightness value
    pixelBrightnessArray[pixelNumber] = pixelBrightness;
    
    /* Now compute new actual color values */
    updatePixelColorValues(pixelNumber);
  }
}

/************************************************************************/
/*  When in per-pixel brightness mode, this function will return the    */
/*  brightness of a single pixel. This brightness value will be the     */
/*  per-pixel brightness set with the setPixelBrightness() function.    */
/************************************************************************/
uint8_t PICxel::getPixelBrightness(uint16_t pixelNumber)
{
  if ((colorArraySizeBytes) && (pixelNumber < numberOfLEDs) && (brightnessMode == perPixel))
  {
    // Get this pixel's brightness
    return(pixelBrightnessArray[pixelNumber]);
  }
  return 0;
}

/************************************************************************/
/*  Refreshed the LED strip with either GRBrefreshLEDs() or             */
/*  HSVrefreshLEDs() dependent on which color mode to use               */
/************************************************************************/
void PICxel::refreshLEDs(void)
{
  if (colorMode == GRB)
  {
    GRBrefreshLEDs();
  }
  else // colorMode == HSV
  {
    HSVrefreshLEDs();
  }
}

/************************************************************************/
/*  Generate the datastream to refresh the LEDs using the RGB color     */
/*  mode.  This function utilizes MIPS assembly no-op commands to       */
/*  ensure that the specific timing is met.                             */
/*                                                                      */
/*  For each bit in each byte, a pair of high and low delays is used.   */
/*                                                                      */
/*  There are preprocessor defined macros to control the variable       */
/*  number of no-ops required to meeting timing as easily as possible.  */
/*  preprocessor defined macros will allow for easier porting to other  */
/*  ChipKIT boards.                                                     */
/************************************************************************/
void PICxel::GRBrefreshLEDs(void)
{
  uint32_t interruptBits;
  uint8_t* colorArrayPtr = colorArray;
  uint8_t bitSelect;

  /* Disable interrupts, but save current bits so we can restore them later */
  interruptBits = disableInterrupts();
    
  for (int j = 0; j < colorArraySizeBytes; j++)
  {
    bitSelect = 0x80;

    while(bitSelect)
    {
      if (*colorArrayPtr & bitSelect)
      {
        *portSet = pinMask;
        GRB_delay_T1H();
        *portClr = pinMask;
        GRB_delay_T1L();
      }
      else
      {
        *portSet = pinMask;
        GRB_delay_T0H();
        *portClr = pinMask;
        GRB_delay_T0L();
      }
      bitSelect = (bitSelect >> 1);
    }
    colorArrayPtr++;
  }
  
  /* Restore the interrupts now */
  restoreInterrupts(interruptBits);
}

/************************************************************************/
/*  Generate the data stream to refresh the LEDs using the HSV color    */
/*  mode.  This function utilizes MIPS assembly to perform the          */
/*  HSV to GRB conversion for the next LED while sending data for the   */
/*  current LED.                                                        */
/*                                                                      */
/*  The overall structure is simple but is difficult to read with the   */
/*  HSV to GRB conversion inline.  In each frame of the bitstream there */
/*  are three components that allow for easier manipulation:            */
/*  - high sector - the first 350ns of the bitframe that is high        */
/*    regardless of the data bit being a 1 or 0                         */
/*  - variable sector - the second 350ns of the bitframe that is high   */
/*    if a 1 and low if a zero                                          */
/*  - low sector - third and final 550ns of the bitframe that is low    */
/*    regardless of the data bit being a 1 or 0                         */
/*                                                                      */
/*  There are preprocessor defined macros to control the variable       */
/*  number of no-ops required to meeting timing as easily as possible.  */
/*  preprocessor defined macros will allow for easier porting to other  */
/*  ChipKIT boards.                                                     */
/************************************************************************/
void PICxel::HSVrefreshLEDs(void)
{ 
  //do not allow bitstream to be interrupted
  noInterrupts();

asm volatile( 
"lw $s0, %4     \n\t" //load address of color_ptr

//compute first color
"j computeFirstColor  \n\t"
"colorComputed:     \n\t"

//loop through all LEDs
"forLoop:     \n\t"
  "move $t0, $t1    \n\t" //move the next_color to current_color

  "move $t1, $zero  \n\t" //clear next_color    

  "addi $s0, $s0, 4 \n\t"
  
  "lw $t2, ($s0)    \n\t"
  
  "move $t6, $zero  \n\t" //clear t6 for a byte iterator
  
"color_loop:      \n\t"
////////////////////
//Bit 0
////////////////////  
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"
  
  "andi $t3, $t2, 0xFFFF  \n\t" //get hue
  
  "srl $t4, $t2, 16     \n\t" //get saturation
  "andi $t4, $t4, 0xFF  \n\t"
  "addi $t4, $t4, 1   \n\t"
  
  "srl $t5, $t2, 24   \n\t" //get value
  "andi $t5, $t5, 0xFF  \n\t"
  "addi $t5, $t5, 1   \n\t"
  
  "multu $t5, $t4     \n\t" //chroma = sat * val
  "mflo $t4       \n\t"
  "srl $t4, $t4, 8    \n\t"
  
  "subu $t5, $t5, $t4   \n\t" //m_num = value - chroma

  //delay for the end of the first section
  HSV_bit_0_delay_0 //preprocessor macro
  
  "andi $t7, $t0, 0b10000000  \n\t" //bitmask out the value 
  "bne $t7, $zero, bit0IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2     \n\t" //clear pin
  "sw %0, ($t7)   \n\t"
  
"bit0IsHigh:      \n\t"

  //delay for the end of the second section
  HSV_bit_0_delay_1 //preprocessor macro

  "lw $t7, %2     \n\t"   //clear pin
  "sw %0, ($t7)   \n\t"

  //if(hue < 256)
  "li $t7, 256        \n\t" //load hue
  "sub $t7, $t3, $t7      \n\t" //store hue - 256
  "bgez $t7, didNotMakeHue0 \n\t" //if hue>256 branch
  
  //hue < 256
  "multu $t3, $t4       \n\t" //set green
  "mflo $t1         \n\t"
  "srl $t1, $t1, 8      \n\t"
  "add $t1, $t1, $t5      \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set red
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 8      \n\t"
  "andi $t7, $t7, 0xFF00    \n\t"
  "or $t1, $t1, $t7     \n\t"

  "andi $t7, $t5, 0xFF    \n\t" 
  "sll $t7, $t5, 16     \n\t" //set blue
  "or $t1, $t7, $t1     \n\t"
  
  //delay for the end of the third section when the hue has been made
  HSV_bit_0_delay_2 //preprocessor macro

  "j madeHue0       \n\t"

"didNotMakeHue0:        \n\t" 

  //delay for the end of the third section when the hue has not been made
  HSV_bit_0_delay_3 //preprocessor macro

"madeHue0:        \n\t"
  
////////////////////
//Bit 1
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro
  
  "andi $t7, $t0, 0b01000000  \n\t" //bitmask out the value 
  "bne $t7, $zero, bit1IsHigh \n\t" //branch if bit is high, branch over set low
  
  "lw $t7, %2     \n\t" //clear pin
  "sw %0, ($t7)   \n\t"
 
"bit1IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
  //if(next_color == 0) then continue
  "bne $t1, $zero, huePrevMade1       \n\t"
  
  //if(hue < 512)
  "li $t7, 512        \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, didNotMakeHue1 \n\t"
  
  //hue < 512
  "addu $t7, $t4, $t5     \n\t" //set green
  "addi $t7, $t7, -1      \n\t"
  "andi $t1, $t7, 0xFF    \n\t"
  
  "li $t7, 511        \n\t" //set red
  "subu $t7, $t7, $t3     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "and $t7, $t7, 0xFF     \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  "sll $t7, $t5, 16     \n\t" //set blue
  "or $t1, $t7, $t1     \n\t" 

  //delay for the end of the third section when hue is made   
  HSV_bit_1_delay_0
  
  "j madeHue1     \n\t" 

"huePrevMade1:      \n\t"

  //delay for the end of the third section hue was made earlier 
  HSV_bit_1_delay_1

  "j madeHue1     \n\t"
  
"didNotMakeHue1:  \n\t"

  //delay for the end of the third section hue has not been made  
  HSV_bit_1_delay_2

"madeHue1:    \n\t"


////////////////////
//Bit 2
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro
  
  "andi $t7, $t0, 0b00100000  \n\t" //bitmask out the value 
  "bne $t7, $zero, bit2IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
 
"bit2IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro
  "nop\n"

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"

  //if(next_color == 0) then continue
  "bne $t1, $zero, huePrevMade2       \n\t"

  //if(hue < 768)
  "li $t7, 768        \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, didNotMakeHue2 \n\t"
  
  //hue < 768
  "addu $t7, $t4, $t5     \n\t" //set green
  "addi $t7, $t7, -1      \n\t"
  "andi $t1, $t7, 0xFF    \n\t"
  
  "sll $t7, $t5, 8      \n\t" //set red
  "or $t1, $t7, $t1     \n\t"
  
  "li $t7, 512        \n\t" //set blue
  "subu $t7, $t3, $t7     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "andi $t7, $t7, 0xFF    \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  //delay for the end of the third section when hue is made   
  HSV_bit_2_delay_0 //preprocessor macro
  
  "j madeHue2     \n\t" 

"huePrevMade2:      \n\t"

  //delay for the end of the third section hue was made earlier 
  HSV_bit_2_delay_1 //preprocessor macro

  "j madeHue2     \n\t"
  
"didNotMakeHue2:  \n\t"
  //delay for the end of the third section when hue is not made 
  HSV_bit_2_delay_2 //preprocessor macro
  
"madeHue2:          \n\t"  
  
////////////////////
//Bit 3
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

//delay for the end of the first section  
HSV_universal_delay_1 //preprocessor macro

  "andi $t7, $t0, 0b00010000  \n\t" //
  "bne $t7, $zero, bit3IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
"bit3IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
  //if(next_color == 0) then continue
  "bne $t1, $zero, huePrevMade3       \n\t"

  //if(hue < 1024)
  "li $t7, 1024       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, didNotMakeHue3 \n\t"
  
  //hue < 1024
  "li $t7, 1023       \n\t" //set green
  "subu $t7, $t7, $t3     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t1, $t7, $t5     \n\t"
  //"and $t7, $t7, 0xFF     \n\t"

  "sll $t7, $t5, 8      \n\t" //set red
  "or $t1, $t7, $t1     \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set blue
  "addi $t7, $t7, -1      \n\t"
  "andi $t7, $t7, 0xFF    \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  //delay for the end of the third section when hue is made   
  HSV_bit_3_delay_0
  
  "j madeHue3     \n\t" 

"huePrevMade3:      \n\t" 

  //delay for the end of the third section hue was made earlier 
  HSV_bit_3_delay_1
  
  "j madeHue3     \n\t"
  
"didNotMakeHue3:  \n\t"
  
//delay for the end of the third section when hue is not made   
  HSV_bit_3_delay_2
  
"madeHue3:    \n\t"     

////////////////////
//Bit 4
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro

  "andi $t7, $t0, 0b00001000  \n\t" //
  "bne $t7, $zero, bit4IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
"bit4IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
  //if next_color is not zero then branch
  "bne $t1, $zero, huePrevMade4       \n\t"

  //if(hue < 1280)
  "li $t7, 1280       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, didNotMakeHue4 \n\t"
  
  //hue < 1280
  "andi $t1, $t5, 0xFF    \n\t" //set green

  "li $t7, 1024       \n\t" //set red
  "subu $t7, $t3, $t7     \n\t"
  "multu $t7, $t4       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set blue
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  //delay for the end of the third section when hue is made   
  HSV_bit_4_delay_0 //preprocessor macro
  
  "j madeHue4     \n\t" 

"huePrevMade4:      \n\t" 
  
  //delay for the end of the third section hue was made earlier 
  HSV_bit_4_delay_1 //preprocessor macro
  
  "j madeHue4     \n\t"
  
"didNotMakeHue4:  \n\t"
  
  //delay for the end of the third section when hue is not made   
  HSV_bit_4_delay_2
  
"madeHue4:    \n\t" 
 
////////////////////
//Bit 5
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro

  "andi $t7, $t0, 0b0000100 \n\t" //
  "bne $t7, $zero, bit5IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
"bit5IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro
  "nop \n"

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
  //if next_color is not zero then branch
  "bne $t1, $zero, huePrevMade5       \n\t"

  //if(hue < 1536)
  "li $t7, 1536       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, didNotMakeHue5 \n\t"
  
  //hue < 1536
  "andi $t1, $t5, 0xFF    \n\t" //set green

  "addu $t7, $t4, $t5     \n\t" //set red
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
  
  "li $t7, 1535       \n\t" //set blue
  "subu $t7, $t7, $t3     \n\t"
  "multu $t7, $t4       \n\t"
  "mflo $t7         \n\t"
  "andi $t7, $t7, 0xFF00    \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
  
  //delay for the end of the third section when hue is made   
  HSV_bit_5_delay_0 //preprocessor macro
  
  "j madeHue5     \n\t" 

"huePrevMade5:      \n\t"

  //delay for the end of the third section hue was made earlier 
  HSV_bit_5_delay_1 //preprocessor macro
  
  "j madeHue5     \n\t"
  
"didNotMakeHue5:  \n\t"
  
  //delay for the end of the third section when hue is not made   
  HSV_bit_5_delay_2 //preprocessor macro
  
"madeHue5:    \n\t"

////////////////////
//Bit 6
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"

  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro

  "andi $t7, $t0, 0b00000010  \n\t" //
  "bne $t7, $zero, bit6IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
"bit6IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"

  "srl $t7, $t2, 24     \n\t" //get value
  "andi $t7, $t7, 0xFF    \n\t"

  "bne $t7, $zero, zero_val_check_1 \n\t"
  "move $t1, $zero      \n\t"

"zero_val_check_1:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_3 //preprocessor macro
  
////////////////////
//Bit 7
////////////////////
  "lw $t7, %3     \n\t" //set pin
  "sw %0, ($t7)   \n\t"
  
  //delay for the end of the first section  
  HSV_universal_delay_1 //preprocessor macro

  "andi $t7, $t0, 0b00000001  \n\t" //
  "bne $t7, $zero, bit7IsHigh \n\t" //branch if bit is high, branch over set low

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
"bit7IsHigh:      \n\t"

  //delay for the end of the second section 
  HSV_universal_delay_2 //preprocessor macro

  "lw $t7, %2       \n\t" //clear pin
  "sw %0, ($t7)     \n\t"
  
  //delay for the end of the second section 
  HSV_universal_delay_3 //preprocessor macro

  //shift the data to the next color byte 
  "srl $t0, $t0, 8    \n\t"
    
  "addi $t6, $t6, 1     \n\t" //iterate t6      
  "li $t7, 3          \n\t" //load 
  "bne $t7, $t6, color_loop \n\t"
  
/////////////////////////////// 
//end of data stream  
  "addi %1, %1, -1    \n\t" //decrement the led 
  
  "bgt %1, $zero, forLoop \n\t"
//end of forLoop:
  "j HSVassemblyEnd       \n\t"

"computeFirstColor:   \n\t"
//color computation

  "lw $t2, ($s0)    \n\t"   //dereference color_ptr address
  "move $t1, $zero    \n\t" //clear next color
    
  "andi $t3, $t2, 0xFFFF  \n\t" //get hue
  
  "srl $t4, $t2, 16     \n\t" //get saturation
  "andi $t4, $t4, 0xFF  \n\t"
  "addi $t4, $t4, 1   \n\t"
  
  "srl $t5, $t2, 24   \n\t" //get value
  "andi $t5, $t5, 0xFF  \n\t"
  "addi $t5, $t5, 1   \n\t"
  
  "multu $t5, $t4     \n\t" //chroma = sat * val
  "mflo $t4       \n\t"
  "srl $t4, $t4, 8    \n\t"
  
  "subu $t5, $t5, $t4   \n\t" //m_num = value - chroma

  //if(hue < 256)
  "li $t7, 256        \n\t" //load hue
  "sub $t7, $t3, $t7      \n\t" //store hue - 256
  "bgez $t7, comp_hue1    \n\t" //if hue>256 branch
  
  //hue < 256
  "multu $t3, $t4       \n\t" //set green
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "add $t1, $t7, $t5      \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set red
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 8      \n\t"
  "andi $t7, $t7, 0xFF00    \n\t"
  "or $t1, $t1, $t7     \n\t"

  "andi $t7, $t5, 0xFF    \n\t" 
  "sll $t7, $t5, 16     \n\t" //set blue
  "or $t1, $t7, $t1     \n\t"

  "j comp_hue_end       \n\t" //jump to end

"comp_hue1:           \n\t" 
  //if(hue < 512)
  "li $t7, 512        \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, comp_hue2    \n\t"
  
  //hue < 512
  "addu $t7, $t4, $t5     \n\t" //set green
  "addi $t7, $t7, -1      \n\t"
  "andi $t1, $t7, 0xFF    \n\t"
  
  "li $t7, 511        \n\t" //set red
  "subu $t7, $t7, $t3     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "and $t7, $t7, 0xFF     \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  "sll $t7, $t5, 16     \n\t" //set blue
  "or $t1, $t7, $t1     \n\t"
  "j comp_hue_end       \n\t" //jump to end
  
"comp_hue2:           \n\t"
  //if(hue < 768)
  "li $t7, 768        \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, comp_hue3    \n\t"
  
  //hue < 768
  "addu $t7, $t4, $t5     \n\t" //set green
  "addi $t7, $t7, -1      \n\t"
  "andi $t1, $t7, 0xFF    \n\t"
  
  "sll $t7, $t5, 8      \n\t" //set red
  "or $t1, $t7, $t1     \n\t"
  
  "li $t7, 512        \n\t" //set blue
  "subu $t7, $t3, $t7     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "andi $t7, $t7, 0xFF    \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  "j comp_hue_end       \n\t" //jump to end
  
"comp_hue3:           \n\t"
  //if(hue < 1024)
  "li $t7, 1024       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, comp_hue4    \n\t"
  
  //hue < 1024
  "li $t7, 1023       \n\t" //set green
  "subu $t7, $t7, $t3     \n\t"
  "multu $t4, $t7       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t1, $t7, $t5     \n\t"
  //"and $t7, $t7, 0xFF     \n\t"
  

  "sll $t7, $t5, 8      \n\t" //set red
  "or $t1, $t7, $t1     \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set blue
  "addi $t7, $t7, -1      \n\t"
  "andi $t7, $t7, 0xFF    \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  "j comp_hue_end       \n\t" //jump to end

"comp_hue4:           \n\t"
  //if(hue < 1280)
  "li $t7, 1280       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, comp_hue5    \n\t"
  
  //hue < 1280
  "andi $t1, $t5, 0xFF    \n\t" //set green

  "li $t7, 1024       \n\t" //set red
  "subu $t7, $t3, $t7     \n\t"
  "multu $t7, $t4       \n\t"
  "mflo $t7         \n\t"
  "srl $t7, $t7, 8      \n\t"
  "addu $t7, $t7, $t5     \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
  
  "addu $t7, $t4, $t5     \n\t" //set blue
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 16     \n\t"
  "or $t1, $t7, $t1     \n\t"
  
  "j comp_hue_end       \n\t" //jump to end

"comp_hue5:           \n\t" 
  //if(hue < 1536)
  "li $t7, 1536       \n\t"
  "sub $t7, $t3, $t7      \n\t"
  "bgez $t7, comp_hue6    \n\t"
  
  //hue < 1536
  "andi $t1, $t5, 0xFF    \n\t" //set green

  "addu $t7, $t4, $t5     \n\t" //set red
  "addi $t7, $t7, -1      \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
  
  "li $t7, 1535       \n\t" //set blue
  "subu $t7, $t7, $t3     \n\t"
  "multu $t7, $t4       \n\t"
  "mflo $t7         \n\t"
  "andi $t7, $t7, 0xFF00    \n\t"
  "sll $t7, $t7, 8      \n\t"
  "or $t1, $t1, $t7     \n\t"
"comp_hue6:           \n\t" 

"comp_hue_end:          \n\t"
  
"srl $t7, $t2, 24     \n\t" //get value
"andi $t7, $t7, 0xFF    \n\t"

"bne $t7, $zero, zero_val_check \n\t"
"move $t1, $zero      \n\t"

"zero_val_check:      \n\t"
    
"j colorComputed    \n\t"
  
//end of assembly
  "HSVassemblyEnd:    \n\t"
  
  : //output
  : "r"(pinMask), "r"(numberOfLEDs), "m"(portClr), "m"(portSet), "m"(colorArray) //input
  : "%s0" //clobber-list
);
  
  //bitstream done, enable interrupts
  interrupts();
}

/************************************************************************/
/*  Creates three colors from the HSV and concatenates them into a      */
/*  32-bit unsigned int that is organized into four bytes:              */
/*  bits[31 - 24][23 - 16][15 - 8][7 - 0]                               */
/*      ( blank )(  red  )(green )(blue )                               */
/************************************************************************/
uint32_t PICxel::HSVToColor(unsigned int HSV)
{
  //hue 0-360 -> 0-1535
  //saturation 0-255
  //value 0-255
  
  uint16_t green, red, blue;
  asm volatile(
    "andi $t0, %3, 0xFFFF   \n\t" //get hue 
    
    "srl $t1, %3, 16      \n\t" //get saturation
    "andi $t1, $t1, 0xFF    \n\t"     
    "addi $t1, $t1, 1     \n\t"
    
    "srl $t2, %3, 24      \n\t" //get value
    "andi $t2, $t2, 0xFF    \n\t"     
    "addi $t2, $t2, 1     \n\t"
    
    "multu $t2, $t1       \n\t" //make chroma = saturation * value 
    "mflo $t1         \n\t"
    "srl $t1, $t1, 8      \n\t"
    
    "subu $t2, $t2, $t1     \n\t" //make m_num = value - chroma
    
    //have hue, chroma and m_num
    //$t0 = hue
    //$t1 = chroma
    //$t2 = m_num 
    
    //if(hue < 256)
    "li $t3, 256        \n\t"
    "sub $t4, $t0, $t3      \n\t"
    "bgez $t4, hue1       \n\t"
    
    //hue < 256
    "multu $t1, $t0       \n\t" //set green
    "mflo %0          \n\t"
    "srl %0, %0, 8        \n\t"
    "add %0, %0, $t2      \n\t"
    
    "addu %1, $t1, $t2      \n\t" //set red
    "addi %1, %1, -1      \n\t"
    
    "move %2, $t2       \n\t" //set blue

    "j assemblyEnd        \n\t"
    
"hue1:                \n\t"
    //if(hue < 512)
    "li $t3, 512        \n\t"
    "sub $t4, $t0, $t3      \n\t"
    "bgez $t4, hue2       \n\t"
    
    //hue < 512
    "addu %0, $t1, $t2      \n\t" //set green
    "addi %0, %0, -1      \n\t"
    
    "li $t3, 511        \n\t" //set red
    "subu %1, $t3, $t0      \n\t"
    "multu $t1, %1        \n\t"
    "mflo %1          \n\t"
    "srl %1, %1, 8        \n\t"
    "addu %1, %1, $t2     \n\t"
    
    "move %2, $t2       \n\t" //set blue
    
    "j assemblyEnd        \n\t"
    
"hue2:                \n\t"
    //if(hue < 768)
    "li $t3, 768        \n\t"
    "sub $t4, $t0, $t3      \n\t"
    "bgez $t4, hue3       \n\t"
    
    //hue < 768
    "addu %0, $t1, $t2      \n\t" //set green
    "addi %0, %0, -1      \n\t"
    
    "move %1, $t2       \n\t" //set red
    
    "li $t3, 512        \n\t" //set blue
    "subu $t4, $t0, $t3     \n\t" 
    "multu $t1, $t4       \n\t"
    "mflo %2          \n\t"
    "srl %2, %2, 8        \n\t"
    "addu %2, %2, $t2     \n\t"
    
    "j assemblyEnd        \n\t"
"hue3:                \n\t"
    //if(hue < 1024)
    "li $t3, 1024       \n\t"
    "sub $t4, $t0, $t3      \n\t"
    "bgez $t4, hue4       \n\t"
    
    //hue < 1024
    "li $t3, 1023       \n\t" //set green
    "subu $t3, $t3, $t0     \n\t"
    "multu $t1, $t3       \n\t"
    "mflo %0          \n\t"
    "srl %0, %0, 8        \n\t"
    "addu %0, %0, $t2     \n\t"
    
    "move %1, $t2       \n\t" //set red
    
    "addu %2, $t1, $t2      \n\t" //set blue
    "addi %2, %2, -1      \n\t"
    
    "j assemblyEnd        \n\t"
"hue4:                \n\t"
    //if(hue < 1280)
    "li $t3, 1280       \n\t"
    "sub $t4, $t0, $t3      \n\t"
    "bgez $t4, hue5       \n\t" 
  
    "move %0, $t2       \n\t" //set green
    
    "li $t3, 1024       \n\t" //set red
    "subu %1, $t0, $t3      \n\t"
    "multu %1, $t1        \n\t"
    "mflo %1          \n\t"
    "srl %1, %1, 8        \n\t"
    "addu %1, %1, $t2     \n\t"
    
    "addu %2, $t1, $t2      \n\t" //set blue
    "addi %2, %2, -1      \n\t"
    
    "j assemblyEnd        \n\t"
"hue5:                \n\t"
      
    "move %0, $t2       \n\t" //set green
    
    "addu %1, $t1, $t2      \n\t" //set red
    "addi %1, %1, -1      \n\t"
    
    "li $t3, 1535       \n\t" //set blue
    "subu %2, $t3, $t0      \n\t"
    "multu %2, $t1        \n\t"
    "mflo %2          \n\t"
    "srl %2, %2, 8        \n\t"
    "addu %2, %2, $t2     \n\t"
      
"assemblyEnd:           \n\t"

    : "=r"(green), "=r"(red), "=r"(blue)//output
    : "r"(HSV)//input
    : //clobber-list
  );
  
  return ((red & 0xFF) << 16 | (green & 0xFF) << 8 | (blue & 0xFF));
}

/************************************************************************/
/*  Returns the number of LEDs declared for the class                   */
/************************************************************************/
uint16_t PICxel::getNumberOfLEDs()
{
  return numberOfLEDs;
}

/************************************************************************/
/*  Returns the address of the beginning of the colorArray              */
/************************************************************************/
uint8_t* PICxel::getColorArray(void)
{
  return colorArray;
}

/************************************************************************/
/*  Returns the global brightness value                                 */
/************************************************************************/
uint8_t PICxel::getBrightness(void)
{
  return globalBrightness;
}