/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE
  
  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel
  
  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look. 
  
  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide
  
  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 4
#define LED_COUNT 10
#define BRIGHTNESS 0x10
#define SPEED 100

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  //Serial.begin(115200);
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop()
{
  // Ride the Rainbow Road
  for (int i=1; i<=LED_COUNT; i++)
  {
    rainbow(i);
    delay(SPEED);  // Delay between rainbow slides
  }
  //delay(1000);
  /*
  // Ride the Rainbow Road
  for (int i=LED_COUNT; i>0; i--)
  {
    rainbow(i);
    delay(SPEED);  // Delay between rainbow slides
  }
  //delay(1000);
  */
}


// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position.
void rainbow(byte startPosition) 
{
  // Need to scale our rainbow. We want a variety of colors, even if there
  // are just 10 or so pixels.
  float rainbowScale = 1.0 / (LED_COUNT);
  
  // Next we setup each pixel with the right color
  for (int i=0; i<LED_COUNT; i++)
  {
    int colorIndex = ((i + startPosition) % (LED_COUNT));
    //Serial.println(colorIndex);
    // It'll return a color between red->orange->green->...->violet for 0-191.
    leds.setPixelColor(i, rainbowColor(rainbowScale * colorIndex));
  }
  // Finally, actually turn the LEDs on:
  leds.show();
}

// Input a value 0.0 to 1.0 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
uint32_t rainbowColor(float progress) 
{
  // Precondition:
  //assert(0 <= progress && progress <= 1);
  
  // state goes from 0 to 5
  // 0    = (0/6 <= progress < 1/6)
  // 1    = (1/6 <= progress < 2/6)
  // ...  = (... <= progress < ...)
  // 5    = (5/6 <= progress <= 6/6)
  // State 0: Red     --> Yellow
  // State 1: Yellow  --> Green
  // State 2: Green   --> Aqua
  // State 3: Aqua    --> Blue
  // State 4: Blue    --> Fuchsia
  // State 5: Fuchsia --> Red
  int state = floor(progress * 6);
  
  // progess is 0 --> 1, over the entire interval
  //    remap it for each state.
  // State 0: 0   --> 1/6 becomes 0 --> 1
  // State 1: 1/6 --> 2/6 becomes 0 --> 1
  // ...:     ... --> ... becomes 0 --> 1
  // State 5: 5/6 --> 6/6 becomes 0 --> 1
  progress -= state/6.;
  progress *= 6;

  // progress-based regulation
  int regulation = round(progress * BRIGHTNESS);
  
  switch(state)
  {
    // State 0: Red     --> Yellow
    case 0:   
      return leds.Color(BRIGHTNESS, regulation, 0);
    
    // State 1: Yellow  --> Green
    case 1:   
      return leds.Color(BRIGHTNESS - regulation, BRIGHTNESS, 0);
    
    // State 2: Green   --> Aqua
    case 2:   
      return leds.Color(0, BRIGHTNESS, regulation);
    
    // State 3: Aqua    --> Blue
    case 3:   
      return leds.Color(0, BRIGHTNESS - regulation, BRIGHTNESS);
    
    // State 4: Blue    --> Fuchsia
    case 4:   
      return leds.Color(regulation, 0, BRIGHTNESS);
    
    // State 5: Fuchsia --> Red
    default:  
      return leds.Color(BRIGHTNESS, 0, BRIGHTNESS - regulation);
  }
}
