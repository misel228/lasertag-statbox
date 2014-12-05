#include <PWM.h>

#include <font5x7.h>
#include <7segment.h>
#include <space01.h>
#include <MicroView.h>
#include <fontlargenumber.h>
#include <font8x16.h>
#include <space02.h>
#include <space03.h>
#include <string.h>

#define IR_PIN  3
#define IR_PIN2  6
#define IR_FREQ 56000 //56kHz
#define IR_HIGH 128 //50% duty cycle
#define IR_LOW  0 //0% duty cycle
#define IR_ONE  60 //time to wait in ms
#define IR_ZERO 30
#define IR_PAUSE 10

#define DT_PIN  5
#define SWT_PIN A0
#define CLK_PIN 2

int menu_index=0;
int sub_menu_index = -1; //-1 = no sub menu selected
int current_sub_index;
int current_menu_index;
volatile boolean turn_detected = 0;
volatile boolean up;
boolean submitting = false;

String main_menu_entries[] = {"Shoot1", "Shoot2", "Respawn","Stats", "Health", "Test2"};
#define MAIN_MENU_SIZE 6

//ir codes stored as string because it's easier to store large amounts of bits (>32) in a somewhat human-readable manner
// code assumes to be carefully handcrafted including preamble and checksum where necessary
String ir_codes[] = {
	"010101110111010101", //Shoot1  e.g. shoot from DAISY with x hit points....
	"101010101010101010", //Shoot2 e.g. shoot from DAISY with x hit points....
	"110110110110110110", //Respawn e.g. shoot from DAISY with x hit points....
	"111000111000111000", //Stats e.g. shoot from DAISY with x hit points....
	"010101110111010101", //Health e.g. shoot from DAISY with x hit points....
	"010101110111010101"  //Test2 menu e.g. shoot from DAISY with x hit points....
};

void setup() {
        int success = 1;
	//TODO attach isr_toggle to CLK_PIN
        attachInterrupt(0,isr,FALLING);

	pinMode(IR_PIN, OUTPUT);
	pinMode(IR_PIN2, OUTPUT);

	pinMode(DT_PIN, INPUT);
	pinMode(CLK_PIN, INPUT);

	pinMode(SWT_PIN, INPUT);
        Serial.begin(9600);

	//set IR_PIN (4) to 56kHz
        //initialize all timers except for 0, to save time keeping functions
        InitTimersSafe();

	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("Initiating");	// display HelloWorld
	uView.display();
      
        //sets the frequency for the specified pin
        success = SetPinFrequencySafe(IR_PIN, IR_FREQ);


        uView.print("Setting up frequency ");
	//uView.setCursor(0,0);
	if(!success){
		uView.print("failed");
	} else {
		uView.print("success");
	}
	uView.display();
	delay(1000);
	pwmWrite(IR_PIN, IR_LOW);
	uView.clear(PAGE);			// clear page
	uView.display();
}


void isr() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  //do not do anything when a transmission is in progress
  if(submitting) {
    return;
  }
  
  if(interrupt_time - last_interrupt_time > 5) {
    if(digitalRead(CLK_PIN)){
      up = digitalRead(DT_PIN);
    }else{
      up = !digitalRead(DT_PIN);
    }
    turn_detected = true;
    last_interrupt_time = interrupt_time;
  }
}


void loop(){
	//rotary switch is read by interrupt here we handle the rest
        if(turn_detected) {
            uView.setCursor(4,0);
            if(!up){
              menu_index++;
            } else{
              menu_index--;
            }
            if(menu_index < 0){
              menu_index = 0;
            }

            if(menu_index > (MAIN_MENU_SIZE -1)){
              menu_index = (MAIN_MENU_SIZE -1);
            }
                 
          turn_detected = false;
        }



	//read button
        int swt_state;
        swt_state = analogRead(SWT_PIN);
        //Serial.println(swt_state);
	if(swt_state < 10){
          delay(50);//debounce
          digitalWrite(IR_PIN2,HIGH);
          submitting = true;
          transmit_code(ir_codes[menu_index]);
          submitting = false;
          digitalWrite(IR_PIN2,LOW);
	}

	draw_main_menu(menu_index);

	//sleep 5ms
	delay(5);
}

void draw_main_menu(int current_index){
        String menu_string;
        int i;

	for(i = 0; i < MAIN_MENU_SIZE; i++) {
		if(i==current_index) {
			menu_string += "#";
		} else {
			menu_string += " ";
                }
                menu_string = (menu_string + main_menu_entries[i] + "\n");
	}
	write_on_screen(menu_string);
}

void write_on_screen(String text){
  //uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.print(text);
  uView.display();
};

void transmit_code(String code) {
  int i = 0;
  Serial.println("BEGIN");
  pwmWrite(IR_PIN,IR_LOW);


  for(i=0;i < code.length() ;i++){
    if(code[i]=='1'){
      pwmWrite(IR_PIN,IR_HIGH);
      delay(IR_ONE);
    } else {
      pwmWrite(IR_PIN,IR_LOW);
      delay(IR_ZERO);
    }
    pwmWrite(IR_PIN,IR_HIGH);
    delay(IR_PAUSE);
  }
  pwmWrite(IR_PIN,IR_LOW);
  Serial.println("END");
};


