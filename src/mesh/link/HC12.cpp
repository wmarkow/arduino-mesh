/*
 * HC12.cpp
 *
 *  Created on: 27.06.2018
 *      Author: wmarkowski
 */

#include "HC12.h"

HC12::HC12() :
      softwareSerial(HC12_RXD_PIN, HC12_TXD_PIN)
{

}

void HC12::begin()
{
   softwareSerial.begin(HC12_DEFAULT_BAUDRATE);
}

bool HC12::icChipConnected()
{
   // switch to AT mode
   // send some command
   // wait with timeout for response
   // switch back to direct send mode

   // return result
   return false;
}

