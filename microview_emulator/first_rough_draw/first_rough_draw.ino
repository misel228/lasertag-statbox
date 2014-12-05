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
#define IR_ONE  600 //time to wait in ms
#define IR_ZERO 300

#define DT_PIN  5
#define SWT_PIN 4
#define CLK_PIN 2

int menu_index=0;
int sub_menu_index = -1; //-1 = no sub menu selected
int current_sub_index;
int current_menu_index;
volatile boolean turn_detected = 0;
volatile boolean up; 
/*
//TODO define submenu structure
struct submenu{
	int sub_menu_no, //sub menu ID
	char* menu_entry,  //string to display
	unsigned int ir_code //IR code to be transmitted when selected
};
*/
//do everything in flat arrays to avoid nasty pointer actions

String main_menu_entries[]={"Shoot", "Health", "Stats","God mode"};
#define MAIN_MENU_SIZE 4
String sub_menu_entries[]={"foo", "bar", "moep","blubb", "huhu"};
#define sub_main_menu_size 5

//ir codes stored as string because it's easier to store large amounts of bits (>32) in a somewhat human-readable manner
// code assumes to be carefully handcrafted including preamble and checksum where necessary
String ir_codes[] = {
	"010101110111010101", //e.g. shoot from DAISY with x hit points....
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

	//set IR_PIN (4) to 56kHz
        //initialize all timers except for 0, to save time keeping functions
        //InitTimersSafe();

	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("Initiating");	// display HelloWorld
	uView.display();
      
        //sets the frequency for the specified pin
        //success = SetPinFrequencySafe(IR_PIN, IR_FREQ);
        digitalWrite(IR_PIN,IR_HIGH);
        digitalWrite(IR_PIN2,HIGH);


        uView.print("Setting up frequency ");
	//uView.setCursor(0,0);
	if(!success){
		uView.print("failed");
	} else {
		uView.print("success");
		pwmWrite(IR_PIN, IR_HIGH);
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
  digitalWrite(IR_PIN,HIGH);
  
  if(interrupt_time - last_interrupt_time > 5) {
    if(digitalRead(CLK_PIN)){
      up = digitalRead(DT_PIN);
    }else{
      up = !digitalRead(DT_PIN);
    }
    turn_detected = true;
    last_interrupt_time = interrupt_time;
  }

 /* */
}


void loop(){
	//rotary switch is read by interrupt here we handle the rest
        if(turn_detected) {
            uView.setCursor(4,0);
            if(!up){
              digitalWrite(IR_PIN,HIGH);
              digitalWrite(IR_PIN2,LOW);
              menu_index++;
            } else{
              digitalWrite(IR_PIN,LOW);
              digitalWrite(IR_PIN2,HIGH);
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
        swt_state = digitalRead(SWT_PIN);
	if(swt_state == HIGH){
		if(sub_menu_index == -1) {
			//no sub selected -> select one now
			sub_menu_index = 0;
		} else {
			if(sub_menu_index == 0){
				sub_menu_index = -1;
			} else {
				//do stuff
			}
		}
	}

	//draw
	if(sub_menu_index == -1) {
		//no sub selected -> move menu index

		draw_main_menu(menu_index);

	} else {
		draw_sub_menu(menu_index, current_sub_index);
	}

	//sleep 5ms
	//delay(5);
}

void draw_main_menu(int current_index){
        String menu_string;
        String main_menu_string;
        int i;
	/*int ibegin = menu_index - 1; //always show at least one on top
	int i,iend;
	if(ibegin < 1) {
		ibegin = 1;
	}
	iend = ibegin + 4;
        if(iend > MAIN_MENU_SIZE) {
          iend = MAIN_MENU_SIZE - 1;
        }*/
	for(i = 0; i < MAIN_MENU_SIZE; i++) {
		if(i==current_index) {
			menu_string += "# ";
		} else {
			menu_string += "  ";
                }
                menu_string = (menu_string + main_menu_entries[i] + "\n");
		//menu_string += main_menu_entries[i] + "\n";
	}
	write_on_screen(menu_string);
}

void draw_sub_menu(int main_menu, int current_sub_index){
//	write_on_screen(string);
};

void write_on_screen(String text){
	//uView.clear(PAGE);
	uView.setCursor(0,0);
	uView.print(text);
	uView.display();
};

void transmit_code(String code) {
/*
	int i = 0;
	for(i=0;i < str_len(code);i++){
		if(code[i]=='1'){
			pwm_write(IR_PIN,IR_HIGH);
			delay(IR_ONE);
		} else {
			pwm_write(IR_PIN,IR_LOW);
			delay(IR_ZERO);
		}
		pwm_write(IR_PIN,IR_HIGH);
		delay(IR_PAUSE);
	}
*/
};


