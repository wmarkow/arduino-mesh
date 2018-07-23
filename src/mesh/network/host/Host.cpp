/*
 * Host.cpp
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#include "Host.h"

Host::Host()
{
   ipAddress = 1;
   rf24interface = NULL;
   hc12interface = NULL;
}

uint8_t Host::getIpAddress()
{
   return ipAddress;
}

void Host::setIpAddress(uint8_t ipAddress)
{
   this->ipAddress = ipAddress;
}

void Host::setRF24Interface(Interface *interface)
{
   this->rf24interface = interface;
   this->flooder.setRF24Interface(interface);
}

void Host::setHC12Interface(Interface *interface)
{
   this->hc12interface = interface;
   this->flooder.setHC12Interface(interface);
}

Flooder* Host::getFlooder()
{
   return &flooder;
}

PingResult Host::ping(uint8_t dstAddress)
{
   if (rf24interface != NULL)
   {
      PingResult pingResult = rf24interface->ping(dstAddress);
      if (pingResult.success)
      {
         return pingResult;
      }
   }

   if (hc12interface != NULL)
   {
      PingResult pingResult = hc12interface->ping(dstAddress);
      if (pingResult.success)
      {
         return pingResult;
      }
   }

   PingResult pingResult;
   pingResult.success = false;

   return pingResult;
}

void Host::loop()
{
   if (rf24interface != NULL)
   {
      processIncomingPackets(rf24interface);
      rf24interface->loop();
   }

   if (hc12interface != NULL)
   {
      processIncomingPackets(hc12interface);
      hc12interface->loop();
   }
}

void Host::processIncomingPackets(Interface* interface)
{
   if (interface == NULL)
   {
      return;
   }

   for (uint8_t index = 0; index < interface->getIncomingPackets()->getSize();
         index++)
   {
      IotPacket* packet = interface->getIncomingPackets()->peek(index);

      if (packet->getDstAddress() != getIpAddress())
      {
         // this packet is not addressed for me; flood that packet
         flooder.flood(packet);
         interface->getIncomingPackets()->remove(index);
         index--;
         continue;
      }

      // here we have some other packet addressed to me, purge it for now
      // TODO: implement transport layer incoming packet queue
      interface->getIncomingPackets()->remove(index);
      index--;
   }
}

Host Localhost;
