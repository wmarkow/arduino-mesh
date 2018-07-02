/*
 * RF24Device.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#include <SoftwareSerial.h>

#include "HC12Device.h"
#include "../network/packet/core/IotPacketHeader.h"

HC12Device::HC12Device() :
      hc12()
{

}

bool HC12Device::up()
{
   hc12.begin();

   return isChipConnected();
}

bool HC12Device::powerDown()
{
   return true;
}

bool HC12Device::isChipConnected()
{
   if (!chipConnected)
   {
      chipConnected = hc12.icChipConnected();
   }

   return chipConnected;
}

bool HC12Device::available()
{
   if(hc12.available() >= DEFAULT_PACKET_SIZE)
   {
      return true;
   }

   return false;
}

bool HC12Device::readPacket(IotPacket* data)
{
   if(hc12.available() >= DEFAULT_PACKET_SIZE)
   {
      hc12.read((uint8_t*)data, DEFAULT_PACKET_SIZE);

      return true;
   }

   return false;
}

bool HC12Device::writePacket(IotPacket* packet)
{
   return hc12.write((uint8_t*)packet, DEFAULT_PACKET_SIZE);
}

String HC12Device::getLinkAddress()
{
   return F("NA");
}

int8_t HC12Device::getPALevelInDbm()
{
   return hc12.getTransmitterPowerInDbm();
}

uint16_t HC12Device::getDataRateInKbs()
{
   return hc12.getDataRateInKbs();

}

uint8_t HC12Device::getPayloadSize()
{
   return DEFAULT_PACKET_SIZE;
}

uint8_t HC12Device::getRFChannel()
{
   return hc12.getRFChannel();
}

String HC12Device::getModel()
{
   return F("HC12");
}

String HC12Device::getInterfaceName()
{
   return F("hc12");
}
