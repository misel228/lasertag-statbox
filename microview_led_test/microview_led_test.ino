#include <PWM.h>

#include <font5x7.h>
#include <7segment.h>
#include <space01.h>
#include <MicroView.h>
#include <fontlargenumber.h>
#include <font8x16.h>
#include <space02.h>
#include <space03.h>

int redPin = 6;
int greenPin = 3;
int myDelay = 1000;
int i;
int32_t frequency = 76000; //frequency (in Hz)
//int32_t frequency = 10000; //frequency (in Hz)

int brightness = 0;         // how bright the LED is
int fadeAmount = 5;         // how many points to fade the LED by


void setup() {
	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("HelloWorld");	// display HelloWorld
	uView.display();

        //initialize all timers except for 0, to save time keeping functions
        InitTimersSafe(); 
      
        //sets the frequency for the specified pin
        bool success = SetPinFrequencySafe(greenPin, frequency);
        
        pwmWrite(greenPin, 128);
}

void loop() {

    /*
  //use this functions instead of analogWrite on 'initialized' pins
  pwmWrite(greenPin, brightness);

  brightness = brightness + fadeAmount;

  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     

  uView.clear(PAGE);			// clear page
  uView.setCursor(0,0);  
  uView.println(brightness);	// display HelloWorld
  uView.display();
  
  delay(300); 
  */
}

  
