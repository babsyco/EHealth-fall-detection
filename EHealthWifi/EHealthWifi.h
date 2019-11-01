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
