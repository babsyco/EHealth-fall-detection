/*
This is a libary for connecting the MySignals EHealth shield + ESP8266 to a
server via wifi, and uploading data to it. Written by Daniel Babekuhl.
*/

#ifndef EHEALTH_WIFI_H
#define EHEALTH_WIFI_H

#include <MySignals.h>
#include "Wire.h"
#include "SPI.h"
#include "Arduino.h"

class EHealthWifi {
  public:
  	void wifiConnect(char* network, char* password);
    char response[200];
    int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout);
    void tcpInit(char* network, char* password);
    void sendData(char* data, char* IP, char* port);
  private:
};

#endif
