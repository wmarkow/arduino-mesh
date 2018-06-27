/*
 * Ifconfig.cpp
 *
 *  Created on: 12 lut 2016
 *      Author: witek
 */

#include "Ifconfig.h"

#include "../../mesh/network/interface/Interface.h"

extern Interface radioRF24;

const __FlashStringHelper* Ifconfig::getName()
{
   return F("ifconfig");
}

void Ifconfig::process(CommandParams *params, HardwareSerial *serial)
{
   if (params->getNumberOfParameters() == 1)
   {
      processIfconfig(serial);

      return;
   }

   if (params->getNumberOfParameters() == 3)
   {
      String interfaceName = params->getParam(1);
      Interface* interface = getInterfaceByName(interfaceName);
      if (interface == NULL)
      {
         serial->print(F("Unknown interface "));
         serial->println(interfaceName);
         return;
      }

      String subcommand = params->getParam(2);
      if (subcommand.equals(F("up")))
      {
         processIfconfigUp(serial, interface);
         return;
      }

      uint8_t address = subcommand.toInt();
      processIfconfigSetIp(address, serial);

      return;
   }

   serial->println(F("Unknown parameters"));
}

void Ifconfig::processIfconfigSetIp(uint8_t address, HardwareSerial *serial)
{
   radioRF24.setIpAddress(address);
   processIfconfig(serial, &radioRF24);
}

void Ifconfig::processBackground(HardwareSerial *serial)
{

}

void Ifconfig::processIfconfig(HardwareSerial *serial)
{
   processIfconfig(serial, &radioRF24);
}

void Ifconfig::processIfconfig(HardwareSerial *serial, Interface *interface)
{
   serial->println(interface->getName());

   if (!interface->isChipConnected())
   {
      Serial.println(F("    down, chip not connected "));

      return;
   }

   serial->print(F("  Model         "));
   serial->println(interface->getDevice()->getModel());
   serial->print(F("  inet addr     "));
   serial->println((int) interface->getIpAddress(), DEC);
   serial->print(F("  HWaddr        "));
   serial->println(interface->getDevice()->getLinkAddress());

   serial->print(F("  PA level      "));
   serial->print(interface->getDevice()->getPALevelInDbm());
   serial->println(F("dBm"));

   serial->print(F("  Data rate     "));
   serial->print(interface->getDevice()->getDataRateInKbs());
   serial->println(F("kbps"));

   serial->print(F("  RF channel    "));
   serial->println(interface->getDevice()->getRFChannel());

   serial->print(F("  Payload size  "));
   serial->println(interface->getDevice()->getPayloadSize());

   serial->println(F("  TX"));
   serial->print(F("   + TCP success "));
   Serial.println(interface->getCounters()->getTransmittedTcpSuccess());
   serial->print(F("   + TCP fail "));
   Serial.println(interface->getCounters()->getTransmittedTcpFailed());
   serial->print(F("   + UDP ACK "));
   Serial.println(interface->getCounters()->getTransmittedUdpAck());
   serial->print(F("   \\ UDP other "));
   Serial.println(interface->getCounters()->getTransmittedUdpOther());
}

void Ifconfig::processIfconfigUp(HardwareSerial *serial, Interface *interface)
{
   interface->up();
   processIfconfig(serial, interface);
}

Interface* Ifconfig::getInterfaceByName(String name)
{
   if(name.equals(radioRF24.getName()))
   {
      return &radioRF24;
   }

   return NULL;
}
