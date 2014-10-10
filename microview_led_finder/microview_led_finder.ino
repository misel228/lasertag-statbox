#include <font5x7.h>
#include <7segment.h>
#include <space01.h>
#include <MicroView.h>
#include <fontlargenumber.h>
#include <font8x16.h>
#include <space02.h>
#include <space03.h>

int ledPin0 = 12;
int myDelay = 1000;
int i;

int pins[] = { 1,2,3,5 };

void setup() {
	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("HelloWorld");	// display HelloWorld
	uView.display();
}

void loop() {
  delay(myDelay);
 for (i = 0; i < 4; i++) {
        ledPin0 = pins[i];
        uView.setCursor(0,0);
	uView.clear(PAGE);			// clear page
	uView.print(ledPin0);	// display HelloWorld
	uView.display();

        pinMode(ledPin0, OUTPUT);
        digitalWrite(ledPin0, HIGH);

        delay(myDelay);
        digitalWrite(ledPin0, LOW);


 }
}

  
