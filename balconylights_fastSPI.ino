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

CRGB leds[NUM_LEDS];

byte in;
CRGB orange = CRGB(0, 21, 255);
CRGB yellow = CRGB(0,255,242);
//tracers
CRGB traceBGColor = CRGB::Black;
CRGB traceColor = CRGB::Yellow;
int numTracerPx = 15;

CRGB curColor;

CRGB notificationColor = CRGB(0,0,255);

const int DATAPIN = 3;
const int CLOCKPIN = 4;

void setup() {
  	// sanity check delay - allows reprogramming if accidently blowing power w/leds
  Serial.begin(9600);
  
   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
   	LEDS.setBrightness(50);
  
  LEDS.addLeds<WS2801, DATAPIN, CLOCKPIN, RGB>(leds, NUM_LEDS);
  curColor = orange;
  setStripColor(curColor);
  in = 0;
  Serial.println("yo nigga");
}

//void serialEvent() {
//  if(Serial.available() > 0) {
//    in = Serial.read();       
//    Serial.print("received: ");
//    Serial.write(in);
//    Serial.println();
//  }
//}


void loop() {   
  
    while (Serial.available() > 0) {

      // look for the next valid integer in the incoming serial stream:
      int red = Serial.parseInt();
      // do it again:
      int green = Serial.parseInt();
      // do it again:
      int blue = Serial.parseInt();
  
      if (Serial.read() == '\n') {

        Serial.print("Setting to ");
        Serial.print(red);
        Serial.print(",");
        Serial.print(green);
        Serial.print(",");
        Serial.println(blue);
        setStripColor(CRGB(red,blue,green));
    }
  
    }
  
//    if(in == '0') {
//      setStripColor(CRGB::Black);
//  } else if(in == '1') {
//      setStripColor(orange);
//  } else if(in == '2') {
//      tracer();
//  } else if(in == '3') {
//      notification();
//  }
}

void setStripColor(CRGB color) {
  LEDS.showColor(color);
  delay(1);
}


void fade(CRGB newColor) {
 
 
  
  
}

void tracer() {
  for(int i=0; i<NUM_LEDS-numTracerPx; i++) {
    for(int j=0; j<numTracerPx; j++) {
      leds[i+j] = traceColor;
    }
    LEDS.show();
    leds[i] = traceBGColor;
    delay(1);
  }
}


void notification() {
  
  int r = random(256);
  int g = random(256);
  int b = random(256);
  Serial.print("Setting Color: ");
  Serial.print(b);
  Serial.print(",");
  Serial.print(g);
  Serial.print(",");
  Serial.println(r);
  CRGB randColor = CRGB(b,g,r);
//  setStripColor(CRGB(b,g,r));
  
  for(int scale = 0; scale < 128; scale++) { 
    LEDS.showColor(randColor, scale);
    delay(10);
   }

  for(int scale = 128; scale > 0; scale--) { 
    LEDS.showColor(randColor, scale);
    delay(10);  
    }
  
}


