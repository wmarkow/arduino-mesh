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
   setSrcAddress(packet->getDstAddress());
   setDstAddress(packet->getSrcAddress());
   setProtocol(packet->getProtocol());
   setType(ACK);
   setPort(packet->getPort());
   setTTL(DEFAULT_TTL);
   setPayloadSize(1);
   
   setAckedPacketId(packet->getId());
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