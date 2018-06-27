/*
 * HC12.h
 *
 *  Created on: 27.06.2018
 *      Author: wmarkowski
 */

#ifndef MESH_LINK_HC12_H_
#define MESH_LINK_HC12_H_

#include <SoftwareSerial.h>

#define HC12_TXD_PIN 2
#define HC12_RXD_PIN 3
#define HC12_SET_PIN 1

#define HC12_DEFAULT_BAUDRATE 9600

class HC12
{
private:
   SoftwareSerial softwareSerial;
public:
   HC12();
   void begin();
   bool icChipConnected();
};


#endif /* MESH_LINK_HC12_H_ */
