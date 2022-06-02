/*
 * AckPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "../../../mesh/network/packet/AckPacket.h"

AckPacket::AckPacket(IotPacket* packet) :
      IotPacket()
{
   setId(packet->getId());
   setProtocol(packet->getProtocol());
   setTTL(DEFAULT_TTL);
   setType(ACK);
   setSrcAddress(packet->getDstAddress());
   setDstAddress(packet->getSrcAddress());
   setPayloadSize(0);
}
;

bool AckPacket::doesAckMatchToPacket(IotPacket* tcpPacket)
{
   if (getDstAddress() != tcpPacket->getSrcAddress())
   {
      return false;
   }

   if (getSrcAddress() != tcpPacket->getDstAddress())
   {
      return false;
   }

   if (getId() != tcpPacket->getId())
   {
      return false;
   }

   if (getProtocol() != tcpPacket->getProtocol())
   {
      return false;
   }

   return true;
}
