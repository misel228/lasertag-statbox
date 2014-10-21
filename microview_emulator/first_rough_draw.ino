#include <PWM.h>

#include <font5x7.h>
#include <7segment.h>
#include <space01.h>
#include <MicroView.h>
#include <fontlargenumber.h>
#include <font8x16.h>
#include <space02.h>
#include <space03.h>

#DEFINE IR_PIN  = 4;
#DEFINE IR_FREQ = 56000; //56kHz
#DEFINE IR_HIGH = 128; //50% duty cycle
#DEFINE IR_LOW  = 0; //0% duty cycle
#DEFINE IR_ONE  = 600; //time to wait in ms
#DEFINE IR_ZERO = 300;

#DEFINE SWITCH_PIN = x;
#DEFINE BUTTON_PIN = y;
#DEFINE REACT_PIN  = z; // TODO find better name

int menu_index=0;
int sub_menu_index = -1; //-1 = no sub menu selected

char* main_menu_entries[]={"Shoot", "Health", "Stats","God mode"};


//TODO define submenu structure
struct submenu{
	int sub_menu_no, //sub menu ID
	char* menu_entry,  //string to display
	unsigned int ir_code //IR code to be transmitted when selected
};

//do everything in flat arrays to avoid nasty pointer actions
//char* sub_menu_entries[]={"Shoot", "Health", "Stats","God mode"};

//ir codes stored as string because it's easier to store large amounts of bits (>32) in a somewhat human-readable manner
// code assumes to be carefully handcrafted including preamble and checksum where necessary
char* ir_codes[] = {
	"010101110111010101", //e.g. shoot from DAISY with x hit points....
	}

void setup() {
	//TODO attach isr_toggle to REACT_PIN

	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("HelloWorld");	// display HelloWorld
	uView.display();


	pinMode(SWITCH_PIN,INPUT);
	pinMode(BUTTON_PIN,OUTPUT);

	//set IR_PIN (4) to 56kHz
        //initialize all timers except for 0, to save time keeping functions
        InitTimersSafe();
      
        //sets the frequency for the specified pin
        bool success = SetPinFrequencySafe(IR_PIN, IR_FREQ);

	uView.setCursor(0,0);
	if(!success){
		uView.print("Setting up frequency failed");
	} else {
		uView.print("Setting up frequency success");
		pwmWrite(IR_PIN, IR_HIGH);
	}
	uView.display();
	sleep(1000);
	pwmWrite(IR_PIN, IR_LOW);
}

function isr_toggle() {
	int up_down = -1;
	//signal is toggled - thus the interrupt
	//read switch code
	if(digitalRead(pin_4) == HIGH){
		up_down = 1;
	}
	if(sub_menu_index == -1) {
		//no sub selected -> move menu index
		menu_index += up_down;

		//TODO min max handle
	} else {
		//submenu
		sub_menu_index += up_down;
		//TODO min max handle
	}
}

void loop(){
	//rotary switch is read by interrupt so no need to handle it in loop
	//read button
	if(digitalRead(BUTTON_PIN) == HIGH){
		if(sub_menu_index == -1) {
			//no sub selected -> select one now
			sub_menu_index = 0;
		} else {
			if(sub_menu_index == 0){
				sub_menu_index = -1
			} else {
				//do stuff
			}
		}

	}

	//draw
	if(sub_menu_index == -1) {
		//no sub selected -> move menu index

		draw_main_menu(menu_index)

	} else {
		draw_sub_menu(menu_index, current_sub_index){
	}

	//sleep 5ms
	delay(5);
}

void draw_main_menu(current_index){
	int begin = menu_index - 1; //always show at least one on top
	int i,end;
	if(begin < 0) {
		begin = 0;
	}
	end = begin + 3;
	for(i = begin; i < end; i++) {
		if(i==current_index) {
			menu_string += '> ';
		}
		menu_string += main_menu_entries[i] + "\n";
	}
	write_on_screen(menu_string);
}

void draw_sub_menu(main_menu, current_sub_index){
	write_on_screen(string);
}

void write_on_screen(string){
	uView.clear(PAGE);
	uView.setCursor(0,0);
	uView.print(string);
	uView.display();
}

void transmit_code(string code) {
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
}




