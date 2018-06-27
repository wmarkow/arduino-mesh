/*
 * RF24Device.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#include <SoftwareSerial.h>

#include "HC12Device.h"
#include "../network/packet/core/IotPacketHeader.h"

SoftwareSerial mySerial(HC12_RXD_PIN, HC12_TXD_PIN);

HC12Device::HC12Device()
{
   mySerial.begin(HC12_DEFAULT_BAUDRATE);
}

bool HC12Device::up()
{
   mySerial.begin(HC12_DEFAULT_BAUDRATE);
   mySerial.flush();

   return isChipConnected();
}

bool HC12Device::powerDown()
{
   return true;
}

bool HC12Device::isChipConnected()
{
   return true;
}

bool HC12Device::available()
{
   return (mySerial.available() >= DEFAULT_PACKET_SIZE) && isChipConnected();
}

void HC12Device::read(void* data, uint8_t size)
{
//	rf24.read(data, size);
}

bool HC12Device::write(void* data, uint8_t size)
{
   const uint8_t* current = reinterpret_cast<const uint8_t*>(data);

   for (uint8_t q = 0; q < size; q++)
   {
      mySerial.write(*current);
      current++;
   }

   return true;
}

String HC12Device::getLinkAddress()
{
   return "";
}

int8_t HC12Device::getPALevelInDbm()
{
   return -99;
}

uint16_t HC12Device::getDataRateInKbs()
{
   return 0;

}

uint8_t HC12Device::getPayloadSize()
{
   return 0;
}

uint8_t HC12Device::getRFChannel()
{
   return 1;
}

String HC12Device::getModel()
{
   return F("HC12");
}
