/*
 * HC12.h
 *
 *  Created on: 27.06.2018
 *      Author: wmarkowski
 */

#ifndef MESH_LINK_HC12_H_
#define MESH_LINK_HC12_H_

#include <SoftwareSerial.h>

#define HC12_TXD_UNO_RX_PIN 8
#define HC12_RXD_UNO_TX_PIN 7
#define HC12_SET_PIN 6

#define HC12_DEFAULT_BAUDRATE 9600

class HC12
{
private:
   SoftwareSerial softwareSerial;
   void enterTransparentMode();
   void enterCommandMode();
public:
   HC12();
   void begin();
   bool icChipConnected();
};


#endif /* MESH_LINK_HC12_H_ */
