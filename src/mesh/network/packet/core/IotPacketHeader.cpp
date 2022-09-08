/*
 * IotPacketHeader.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "IotPacketHeader.h"

IotPacketHeader::IotPacketHeader()
{
   setId(0);
   setProtocol(TCP);
   setType(REGULAR);
   setTTL(DEFAULT_TTL);
   setSrcAddress(0);
   setDstAddress(0);
   setPayloadSize(DEFAULT_PACKET_PAYLOAD_SIZE);
}

uint8_t IotPacketHeader::getId()
{
   return header[0];
}
IotProtocol IotPacketHeader::getProtocol()
{
   return (IotProtocol) header[1];
}

IotPacketType IotPacketHeader::getType()
{
   return (IotPacketType) header[2];
}

uint8_t IotPacketHeader::getSrcAddress()
{
   return header[3];
}
uint8_t IotPacketHeader::getDstAddress()
{
   return header[4];
}

uint8_t IotPacketHeader::getTTL()
{
   return header[5];
}

uint8_t IotPacketHeader::getPayloadSize()
{
   return header[6];
}

uint8_t IotPacketHeader::decrementTTL()
{
   uint8_t newTTL = getTTL() - 1;

   if (newTTL > DEFAULT_TTL)
   {
      newTTL = 0;
   }

   setTTL(newTTL);

   return newTTL;
}

void IotPacketHeader::setId(uint8_t id)
{
   header[0] = id;
}
void IotPacketHeader::setProtocol(IotProtocol protocol)
{
   header[1] = (uint8_t) protocol;
}

void IotPacketHeader::setType(IotPacketType type)
{
   header[2] = (uint8_t) type;
}

void IotPacketHeader::setSrcAddress(uint8_t address)
{
   header[3] = address;
}

void IotPacketHeader::setDstAddress(uint8_t address)
{
   header[4] = address;
}

void IotPacketHeader::setTTL(uint8_t ttl)
{
   header[5] = ttl;
}

void IotPacketHeader::setPayloadSize(uint8_t size)
{
   if (size <= DEFAULT_PACKET_PAYLOAD_SIZE)
   {
      header[6] = size;
   }
   else
   {
      header[6] = DEFAULT_PACKET_PAYLOAD_SIZE;
   }
}

