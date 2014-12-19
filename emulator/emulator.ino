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
#include <avr/pgmspace.h>

//IR transmission
#define IR_PIN     3     //IR LED
#define IR_FREQ    56000 //56kHz
#define IR_HIGH    128   //50% duty cycle
#define IR_LOW     0     //0% duty cycle
#define IR_ONE     1200  //time to wait in µs
#define IR_ZERO    600
#define IR_SPACE   600
#define IR_HEADER  2400

//rotary dialer
#define DT_PIN   5
#define SWT_PIN A0     //no more digital pins left for MicroView (#0 and #1 are used for programming and stuff gets icky)
#define CLK_PIN  2

#define RED_LED  6
#define CHUNK_SIZE 80

//#define DEBUG true

int menu_index=5;
int count,looper = 0;
volatile boolean turn_detected = 0;
volatile boolean up;
boolean submitting = false;

String main_menu_entries[] = {
  "Shoot1", "Shoot2", "Respawn", "Health", "Test2","Stats"};
#define MAIN_MENU_SIZE 6


// IR codes stored as string because it's easier to store large amounts of bits (>32) in a somewhat human-readable manner
// code assumes to be carefully handcrafted including checksum where necessary
prog_char ir_code_0[] PROGMEM = "00000000000000";
prog_char ir_code_1[] PROGMEM = "00011010101110";
prog_char ir_code_2[] PROGMEM = "111010000000010011101000";
prog_char ir_code_3[] PROGMEM = "100000000110000011101000";
prog_char ir_code_4[] PROGMEM = "Test2";
prog_char ir_code_5[] PROGMEM = "Stats";

PROGMEM const char *ir_codes_table[] = {   
  ir_code_0,
  ir_code_1,
  ir_code_2,
  ir_code_3,
  ir_code_4,
  ir_code_5
};

char buffer[100];


// statistic data actually come in three packages which are described here
prog_char stat_code_0[] PROGMEM = "100001110000001111101000000001010000001101100011000000100010110000000011000001010001011000001101000000000000000011000100";
prog_char stat_code_1[] PROGMEM = "10000101000000000011010100000001111010000000000100011110000000011001111100000001"
    "00000000000000001101101100000001110000110000000111001100000000000011001000000000"
    "11000101000000011001010100000001110011000000000100101001000000000110010000000001"
    "00001001000000011110111100000001110100010000000100001110000000010000011100000001"
    "01011101000000010000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000011101001";
prog_char stat_code_2[] PROGMEM = "00010110000000001010111100000010000110000000000110000010000000111011001000000010"
    "00000000000000001100100000000011000110110000000000011010000000011001111100000011"
    "11110100000000000111100000000001100111000000001111011010000000000100010000000001"
    "00010001000000000000101100000000000010110000000001011111000000010110101100000000"
    "01111011000000110000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000111111";

PROGMEM const char *stat_codes_table[] = {   
  stat_code_0,
  stat_code_1,
  stat_code_2
};

//stat code sizes +1 for final null byte
PROGMEM const int stat_code_sizes[] = {   
  121,
  1705,
  1705
};

void setup() {
  int success;
  Serial.begin(9600);

  Serial.println("attach ISR");
  //attach isr_toggle to CLK_PIN
  attachInterrupt(0,isr,FALLING);

  Serial.println("setting up pins");
  pinMode(IR_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(DT_PIN, INPUT);
  pinMode(CLK_PIN, INPUT);

  pinMode(SWT_PIN, INPUT);

  Serial.println("init timer");
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe();

  Serial.println("init Display");
  //IMPORTANT - init MicroView *after* timer!
  uView.begin();
  uView.clear(PAGE);
  uView.print("Initiating");
  uView.display();

  uView.print("Setting up frequency ");

  //set IR_PIN (4) to 56kHz
  success = SetPinFrequencySafe(IR_PIN, IR_FREQ);

  if(!success){
    uView.print("failed");
  } 
  else {
    uView.print("success");
    pwmWrite(RED_LED,HIGH);
  }
  uView.display();

  pwmWrite(RED_LED,LOW);
  pwmWrite(IR_PIN, IR_LOW);
  uView.clear(PAGE);
  uView.display();
}


void isr() {  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if(interrupt_time - last_interrupt_time > 5) {
    if(digitalRead(CLK_PIN)){
      up = digitalRead(DT_PIN);
    } 
    else {
      up = !digitalRead(DT_PIN);
    }
    turn_detected = true;
    last_interrupt_time = interrupt_time;
  }
}


void loop(){
  if(count > 100) {
    count = 0;
    Serial.println(".");
  } else {
    if(looper++ > 100) {
      Serial.print('.');
      looper = 0;
      count++;
    }
  }
  //check if rotary switch was turned and add accordingly
  if(turn_detected) {
    if(!up){
      menu_index++;
    } 
    else{
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

  if(swt_state < 10){
    delay(150);  //debounce
    count=0;
    Serial.println("#");
    digitalWrite(RED_LED,HIGH);
    submitting = true;
    if(menu_index < 5){
      strcpy_P(buffer, (char*)pgm_read_word(&(ir_codes_table[menu_index])));
      transmit_code(buffer);
    } else {
      //special treatment for STATS packages
      Serial.println("Transmit stats");
      delay(500);
      for(int i=0; i < 3; i++) {
        void* address   = (void*)pgm_read_word(stat_codes_table + i);
        Serial.print("Address is ");
        Serial.println((int)address);

        int code_size = (int)pgm_read_word(stat_code_sizes + i);
        Serial.print("Codesize is ");
        Serial.println(code_size);
        transmit_large(address,code_size);
      }
    }
    submitting = false;
    digitalWrite(RED_LED,LOW);
  }

  draw_main_menu(menu_index);

  delay(5);
}

void draw_main_menu(int current_index){
  String menu_string;
  int i;

  for(i = 0; i < MAIN_MENU_SIZE; i++) {
    if(i==current_index) {
      menu_string += "#";
    } 
    else {
      menu_string += " ";
    }
    menu_string = (menu_string + main_menu_entries[i] + "\n");
  }
  uView.setCursor(0,0);
  uView.print(menu_string);
  uView.display();
}

void transmit_code(String code) {
  int i = 0;

  Serial.println(".");
  Serial.println("BEGIN transmit code");

  pwmWrite(IR_PIN,IR_HIGH);
  delayMicroseconds(IR_HEADER);
  pwmWrite(IR_PIN,IR_LOW);
  delayMicroseconds(IR_SPACE);

  for(i=0;i < code.length() ;i++){
    pwmWrite(IR_PIN,IR_HIGH);
    if(code[i]=='1'){
      delayMicroseconds(IR_ONE);
    } 
    else {
      delayMicroseconds(IR_ZERO);
    }
    pwmWrite(IR_PIN,IR_LOW);
    delayMicroseconds(IR_SPACE);
  }
  pwmWrite(IR_PIN,IR_LOW);
  Serial.println("END");
};

void transmit_large(void* stat_code, int bytes) {
  int i,j = 0;
  int bytes_left = bytes;
  int bytes_to_read = CHUNK_SIZE;
  char buffer[CHUNK_SIZE + 1] = {0};

  Serial.println("BEGIN RAW");

  pwmWrite(IR_PIN,IR_HIGH);
  delayMicroseconds(IR_HEADER);
  pwmWrite(IR_PIN,IR_LOW);
  delayMicroseconds(IR_SPACE);

  do{
    if(bytes_left   > CHUNK_SIZE ){
      bytes_to_read = CHUNK_SIZE;
      bytes_left   -= CHUNK_SIZE;
    } else {
      bytes_to_read = bytes_left;
      bytes_left = 0;
    }
    void* address = (void*)((int)stat_code + (i * CHUNK_SIZE));
    memcpy_P(buffer, address, bytes_to_read);


    for(j = 0; j < bytes_to_read ;j++){
      pwmWrite(IR_PIN,IR_HIGH);
      if(buffer[j]=='1'){
        delayMicroseconds(IR_ONE);
      } 
      else {
        delayMicroseconds(IR_ZERO);
      }
      pwmWrite(IR_PIN,IR_LOW);
      delayMicroseconds(IR_SPACE);
    }

    #if defined(DEBUG)
      Serial.println((char*)buffer);
    #endif
    i++;
  } while(bytes_left > 0);


  Serial.println("END");
};

