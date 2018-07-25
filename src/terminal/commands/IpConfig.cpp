/*
 * IfpCnfig.cpp
 *
 *  Created on: 15 lip 2018
 *      Author: witek
 */

#include "IpConfig.h"

#include "../../mesh/network/interface/Interface.h"
#include "../../mesh/network/node/MeshNode.h"

const __FlashStringHelper* IpConfig::getName()
{
   return F("ipconfig");
}

void IpConfig::process(CommandParams *params, HardwareSerial *serial)
{
   if (params->getNumberOfParameters() == 1)
   {
      processIpConfig(serial);

      return;
   }

   if (params->getNumberOfParameters() == 2)
   {
      String subcommand = params->getParam(1);
      uint8_t address = subcommand.toInt();
      processIpConfigSetIp(address, serial);

      return;
   }

   serial->println(F("Unknown parameters"));
}

void IpConfig::processIpConfigSetIp(uint8_t address, HardwareSerial *serial)
{
   LocalMeshNode.setIpAddress(address);
   processIpConfig(serial);
}

void IpConfig::processBackground(HardwareSerial *serial)
{

}

void IpConfig::processIpConfig(HardwareSerial *serial)
{
   Serial.print(F("IP Address : "));
   Serial.println(LocalMeshNode.getIpAddress());
}
