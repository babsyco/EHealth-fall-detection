//parts borrowed from: https://www.cooking-hacks.com/mysignals-hw-ehealth-medical-biometric-iot-platform-arduino-tutorial/#step6

#include <MySignals.h>
#include <Wire.h>
#include "SPI.h"
#include <Adafruit_ILI9341_AS.h>
//#include "Keyboard.h"
#include <string.h>

//for screen of device
Adafruit_ILI9341_AS tft = Adafruit_ILI9341_AS(TFT_CS, TFT_DC);

//body sensor data acquisistion/general variables
char bufferAcc[50];
char x_acc_string[10];
char y_acc_string[10];
char z_acc_string[10];
double x_val = 0;
double y_val = 0;
double z_val = 0;
uint8_t position = 0; //body position from sensor
int delay_time = 10; //milliseconds
uint8_t cancel = 0; //button
uint8_t confirm = 0; //button
char device_line1[30] = "";
char device_line2[30] = "";

//fall detection variables
uint8_t mode = 0; //0 = regular operation, 1 = countdown, 2 = send message
double threshold = 1.1; //threshold for fall detection, 1.9 probably ideal
int wait_time = 10; //wait time after fall (seconds)
int counter = 1; //countdown timer, default = 1 as 0 is a trigger value

//General functions
void print_body_measurements();
void display_on_device(char * message, int line);

//Fall detection functions
void fall_detection_main();
void regular_operation();
void countdown();
void fall_confirmed();
void send_message();

void setup() {

  Serial.begin(115200);
  MySignals.begin();
  MySignals.initBodyPosition();

  //setup screen on device
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);

  //just for testing
  //Keyboard.begin();
}

void loop() {

  //BODY SENSOR DATA ACQUISITION
  position = MySignals.getBodyPosition();
  MySignals.getAcceleration();

  //get accelerometer readings
  x_val = MySignals.x_data;
  y_val = MySignals.y_data;
  z_val = MySignals.z_data;

  //print values to serial port
  //print_body_measurements();

  //DEVICE FUNCTIONS
  fall_detection_main(); //primary function

  delay(delay_time);
} //end loop


//GENERAL FUNCTIONS
//print body position measurements to serial port
void print_body_measurements() {

  //print body position
  MySignals.printPosition(position);

  //convert X Y Z measurements to strings and print
  dtostrf (x_val, 2, 2, x_acc_string);
  dtostrf (y_val, 2, 2, y_acc_string);
  dtostrf (z_val, 2, 2, z_acc_string);
  sprintf (bufferAcc, "Acceleration: X= %s  Y= %s  Z= %s  ", x_acc_string, y_acc_string, z_acc_string);
  Serial.println(bufferAcc);
}

//displays message on device, max length of message string 30 chars.
//If line = 1, writes over first line. Line = 2: writes over second line.
//Line = 0: resets and writes to first line.
void display_on_device(char * message, int line) {

  //this must be done before any new writing on screen
  tft.fillScreen(ILI9341_WHITE);

  if (line == 0) {
    strcpy(device_line1, message);
    strcpy(device_line2, "");;
  }
  else if (line == 1) {
    strcpy(device_line1, message);
  }
  else if (line == 2) {
    strcpy(device_line2, message);
  }

  //write to screen
  tft.drawString(device_line1,5,5,4); //1st line
  tft.drawString(device_line2,5,40,4); //1st line
}

//read buttons
void get_button_values() {

  //All this does at the moment is set cancel = 1
  //when 'c' is typed into serial port
  char inChar = '0';
  inChar = Serial.read();
  if ('c' == inChar) {
    cancel = 1;
    inChar = '0';
  }
}


//FALL DETECTION FUNCTION
void fall_detection_main() {

  //mode = 0: default mode. Only job is to detect fall measurement.
  if (mode == 0) {
    regular_operation();
  }
  //mode 1: triggered after fall detected in mode 0.
  else if (mode == 1) {
    countdown();
  }
  //mode 2: triggered after fall wait-time elapses in mode 1.
  else if (mode == 2) {
    fall_confirmed();
  }
}

//Fall detection, mode 0: regular operation (buttons not monitored)
void regular_operation() {

  //display message on device
  display_on_device("Fall status: safe.", 0);

  //detect fall
  if (((x_val > threshold) || (y_val > threshold)) || (x_val > threshold)) {

    display_on_device("Fall detected!", 1);

    //short delay should go here, then test for body position. If lying down:
    counter = wait_time*1000; //set counter up for countdown (milliseconds)
    mode = 1; //switch to mode 1
  }
}

//Fall detection, mode 1: countdown (buttons monitored)
void countdown() {

  //display prompt to cancel + countdown (in seconds)
  if (counter % 1000 == 0) {

    char buf[50];
    sprintf (buf, "Hit 'cancel': %d", counter/1000);
    display_on_device(buf, 2);
  }

  //decrement count. Count is in ms, decrement = delay time of loop
  //CHANGE THIS LINE: *10 is added to compensate for current loop lag
  counter = counter - delay_time*10;

  //monitor buttons
  get_button_values();

  //cancel button is pushed
  if (cancel == 1) {

    display_on_device("Cancelled: you're ok", 0);
    delay(3000); //to let message be read on device

    //reset to regular operation (mode 0), reset button values
    mode = 0;
    cancel = 0;
    confirm = 0;
    counter = 1;
  }

  //countdown has reached end, switch to mode 2: fall confirmed
  if (counter == 0) {
    mode = 2;
  }
}

//Fall detection, mode 2: fall confirmed (buttons monitored)
void fall_confirmed() {

  //send message on first run of loop only
  if (counter == 0) {

    display_on_device("Alerting carer!", 1);
    send_message(); //send message to guardian
    counter = 1;
  }

  //monitor buttons
  get_button_values();

  //cancel button is pushed
  if (cancel == 1) {
    display_on_device("You have cancelled alarm.", 1);
    display_on_device("Resuming regular function..", 2);
    delay(4000); //just time to read message on screen
    //reset to regular operation, reset button values
    mode = 0;
    cancel = 0;
    confirm = 0;
    counter = 1;
  }
}

//Fall detection, send message to registered party
void send_message() {

  //replace this code block with actual message code
  Serial.print("TO GUARDIAN: ");
  Serial.println("Your elderly loved one has had a fall!");

  //confirm sending on device
  display_on_device("Message to carer sent.", 2);
}
