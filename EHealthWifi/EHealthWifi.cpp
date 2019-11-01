/*
    Copyright (C) 2017 Libelium Comunicaciones Distribuidas S.L.
   http://www.libelium.com

    By using it you accept the MySignals Terms and Conditions.
    You can find them at: http://libelium.com/legal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Version:           2.0
    Design:            David Gascon
    Implementation:    Luis Martin / Victor Boria
*/

/*
This is a libary for connecting the MySignals EHealth shield + ESP8266 to a
server via a wifi network, and uploading data to it. Written by Daniel Babekuhl.
*/

#include <MySignals.h>
#include "Wire.h"
#include "SPI.h"
#include "EHealthWifi.h"
#include "Arduino.h"

void EHealthWifi::wifiConnect(char* network, char* password)
{

  //Enable WiFi ESP8266 Power -> bit1:1
  bitSet(MySignals.expanderState, EXP_ESP8266_POWER);
  MySignals.expanderWrite(MySignals.expanderState);

  MySignals.initSensorUART();
  MySignals.enableSensorUART(WIFI_ESP8266);

  //Checks if the WiFi module is started
  int8_t answer = sendATcommand("AT", "OK", 5000);
  if (answer == 0)
  {
    MySignals.println("Error");
    //waits for an answer from the module
    while (answer == 0)
    {
      //Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 5000);
    }
  }
  else if (answer == 1)
  {
    MySignals.println("WiFi module working\n");
  }

  //Configure module and start wifi connection
  tcpInit(network, password);
}


/*
Sends an AT command and compares to expected response. Returns 1 if response
is as expected, 0 otherwise. Will wait for timeout in ms before returning
with no response. If expected_answer1 = NULL, assumes no answer is expected,
returns 0.
*/
int8_t EHealthWifi::sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

  uint8_t x = 0,  answer = 0;
  unsigned long previous;

  memset(response, '\0', sizeof(response));    // Initialize the string

  while ( Serial.available() > 0) Serial.read();   // Clean the input buffer

  Serial.println(ATcommand);    // send the AT command

  x = 0;
  previous = millis();

  // Loop to read response data while available (data is sent as separate bytes),
  // or before timeout. flag = 1 means some response data has been recieved.
  // answer = 1 means full response has been recieved.
  uint8_t flag = 0;
  while (((millis() - previous) < timeout) && (answer == 0)) {

      //Time for another byte to be sent
      delay(1);

     //Monitor for start of response
     if (Serial.available()) {
      response[x] = Serial.read();
      x++;
      flag = 1;
     }

     //Monitor for end of response
     if ((flag == 1) && !Serial.available()) {
      answer = 1;
     }
  }

  //Return correct value
  if (strstr(response, expected_answer1) != NULL) {
      return 1;
  } else return 0;
}

/*
Configures module, initiates wifi connection. Args are as labeled.
*/
void EHealthWifi::tcpInit(char* network, char* password){

  //Set state of module
  sendATcommand("AT+CWMODE=3", "OK", 1000);

  //create AT connection command
  char AT_connect[100];
  sprintf(AT_connect, "AT+CWJAP=\"%s\",\"%s\"", network, password);
  
  //Connect to wifi
  sendATcommand(AT_connect, "OK", 2000);

  // this ATcommand gets IP address of module, however response takes a long time.
  // Will need specific function for this.
  sendATcommand("AT+CIFSR", "NULL", 6000);
}

/*
Connects to server and sends data. Args are as labeled. Will send exactly 20-byte blocks
of data. Make sure you are sending no less than 20 bytes. Extra bytes will be lost. 
Tip: pad with whitespace.
*/
void EHealthWifi::sendData(char* data, char* IP, char* port) {

  //AT command to connect to server
  char conn_server[100];
  sprintf(conn_server, "AT+CIPSTART=\"TCP\",\"%s\",%s", IP, port);

  //Make connection
  sendATcommand(conn_server, NULL, 1000);

  //Send data size
  Serial.println("AT+CIPSEND=" + String(strlen(data))); //String(data.length()+2)
  delay(5); // minimum 5ms delay
  //Send data
  Serial.println(data);
}
