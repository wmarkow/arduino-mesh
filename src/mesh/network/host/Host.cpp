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

   if (rf24interface != NULL)
   {
      rf24interface->setIpAddress(ipAddress);
   }
   if (hc12interface != NULL)
   {
      hc12interface->setIpAddress(ipAddress);
   }
}

void Host::setRF24Interface(Interface *interface)
{
   this->rf24interface = interface;
}

void Host::setHC12Interface(Interface *interface)
{
   this->hc12interface = interface;
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

MeshNodeCounters* Host::getCounters()
{
   return &counters;
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
         flood(packet);
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

void Host::flood(IotPacket* packet)
{
   if (packet->getSrcAddress() == getIpAddress())
   {
      // I'm originator of this packet; drop it
      counters.incDroppedCount();

      return;
   }

   if (packet->decrementTTL() == 0)
   {
      // TTL reached 0; drop packet
      counters.incDroppedCount();

      return;
   }

   // flood packet
   if (rf24interface != NULL)
   {
      rf24interface->floodToTransmitter(packet);
   }
   if (hc12interface != NULL)
   {
      hc12interface->floodToTransmitter(packet);
   }

   counters.incFloodedCount();
}

Host Localhost;
