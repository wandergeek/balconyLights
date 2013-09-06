// Uncomment this line if you have any interrupts that are changing pins - this causes the library to be a little bit more cautious
// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1

// Uncomment this line to force always using software, instead of hardware, SPI (why?)
// #define FORCE_SOFTWARE_SPI 1

// Uncomment this line if you want to talk to DMX controllers
// #define FASTSPI_USE_DMX_SIMPLE 1

#include "FastSPI_LED2.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// test code
//
//////////////////////////////////////////////////

#define NUM_LEDS 254
#define slen 7        // 7 characters, e.g. '#ff6666'

CRGB leds[NUM_LEDS];

byte in;
CRGB orange = CRGB(255, 21, 0);  //b,g,r
CRGB yellow = CRGB(0,255,242);
CRGB curColor;

const int DATAPIN = 3;
const int CLOCKPIN = 4;
int redVal = 0;
int grnVal = 0;
int bluVal = 0;
int wait = 1;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 3;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;
int numLoops = 300;

char serInStr[slen];  // array to hold the incoming serial string bytes



void setup() {
  	// sanity check delay - allows reprogramming if accidently blowing power w/leds
  Serial.begin(9600);
  
   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
   	LEDS.setBrightness(100);
  
  LEDS.addLeds<WS2801, DATAPIN, CLOCKPIN, RGB>(leds, NUM_LEDS);
  curColor = orange;
  setStripColor(curColor);
  Serial.println("yo nigga");
}

void loop() {   
  
  if(Serial.available() >=9) {
    int r = Serial.parseInt();
    int c = Serial.read();
    int g = Serial.parseInt();
        c = Serial.read();
    int b = Serial.parseInt();
    setStripColor(CRGB(r,g,b));
  }

}


void setStripColor(CRGB color) {
//  CRGB fixedColor = CRGB(color.b,color.g,color.r); //ordering is fucked, cbf fixing it
  CRGB fixedColor = CRGB(color.r,color.g,color.b); 
  LEDS.showColor(fixedColor);
  delay(1);
}



//http://www.arduino.cc/en/Tutorial/ColorCrossfader

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = numLoops/step;              //   divide by 1020
  } 
  return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(CRGB newColor) {
  // Convert to 0-255
  int R = newColor.r;
  int G = newColor.g;
  int B = newColor.b;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= numLoops; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);
    
    setStripColor(CRGB(redVal,grnVal,bluVal));

//    delay(wait); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        Serial.print("Loop/RGB: #");
        Serial.print(i);
        Serial.print(" | ");
        Serial.print(redVal);
        Serial.print(" / ");
        Serial.print(grnVal);
        Serial.print(" / ");  
        Serial.println(bluVal); 
      } 
      DEBUG += 1;
    }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}

