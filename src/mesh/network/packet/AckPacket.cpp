/*
 * AckPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "AckPacket.h"

AckPacket::AckPacket(IotPacket* packet) :
      IotPacket()
{
   setAckedPacketId(packet->getId());
   setProtocol(packet->getProtocol());
   setTTL(DEFAULT_TTL);
   setType(ACK);
   setSrcAddress(packet->getDstAddress());
   setDstAddress(packet->getSrcAddress());
   setPayloadSize(1);
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

   if (getAckedPacketId() != tcpPacket->getId())
   {
      return false;
   }

   if (getProtocol() != tcpPacket->getProtocol())
   {
      return false;
   }

   return true;
}

void AckPacket::setAckedPacketId(uint8_t id)
{
   this->payload[0] = id;
}

uint8_t AckPacket::getAckedPacketId()
{
   return this->payload[0];
}