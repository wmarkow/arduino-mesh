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
   return hc12.icChipConnected();
}

bool HC12Device::available()
{
   return false;
}

void HC12Device::read(void* data, uint8_t size)
{

}

bool HC12Device::write(void* data, uint8_t size)
{
   return hc12.write(data, size);
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
   return 0;
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
