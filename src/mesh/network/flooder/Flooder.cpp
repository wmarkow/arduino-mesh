/*
 * Flooder.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "../../../mesh/network/flooder/Flooder.h"
#include "../host/Host.h"

Flooder::Flooder()
{
   rf24interface = NULL;
   hc12interface = NULL;
}
void Flooder::setRF24Interface(Interface *interface)
{
   this->rf24interface = interface;
}

void Flooder::setHC12Interface(Interface *interface)
{
   this->hc12interface = interface;
}

void Flooder::flood(IotPacket* packet)
{
   if (packet->getSrcAddress() == Localhost.getIpAddress())
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

MeshNodeCounters* Flooder::getCounters()
{
   return &counters;
}
