/*
This is a test program for the EHealthWifi.h library. Installing this library
requires the EHealthWifi.zip file, containing 'EHealthWifi.ccp' and 
'EHealthWifi.h'. To install:

1) In the Arduino IDE, go to Sketch > Include Library > Add .ZIP library...
2) Locate EHealthWifi.zip
3) Click 'Choose'

After that library should be installed: you can check this by going to
Sketch > Include Library. EHealthWifi should be listed under
'Contributed libraries'. Once installed, this program should run. It will
send data to the specified server at approximately 8Hz (possibly dependent on
size of data).
*/

#include <MySignals.h>
#include "Wire.h"
#include "SPI.h"
#include <EHealthWifi.h>

//create instance of EHealthWifi class
EHealthWifi wifi;

void setup() {

  //standard setup functions
  Serial.begin(115200);
  MySignals.begin();

  //Make wifi connection. Args: (char* network, char* password)
  wifi.wifiConnect("Dans_iPhone", "Dans_Password");
}

void loop() {

  //Connect to server and send data. Args: (char* data, char* IP, char* port)
  wifi.sendData("hello", "192.20.10.2", "20000");
}
