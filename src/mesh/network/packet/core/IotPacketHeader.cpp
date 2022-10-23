/*
 * IotPacketHeader.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "IotPacketHeader.h"
#include "IotPort.h"

IotPacketHeader::IotPacketHeader()
{
   setSrcAddress(0);
   setDstAddress(0);
   setProtocol(TCP);
   setType(REGULAR);
   setPort(IotPort::ICMP);
   setTTL(DEFAULT_TTL);
   setPayloadSize(DEFAULT_PACKET_PAYLOAD_SIZE);
}

uint8_t IotPacketHeader::getSrcAddress()
{
   return header.srcAddress;
}

void IotPacketHeader::setSrcAddress(uint8_t address)
{
   header.srcAddress = address;
}

uint8_t IotPacketHeader::getDstAddress()
{
   return header.dstAddress;
}

void IotPacketHeader::setDstAddress(uint8_t address)
{
   header.dstAddress = address;
}

IotProtocol IotPacketHeader::getProtocol()
{
   if(header.protocol == 0)
   {
      return IotProtocol::UDP;
   }

   return IotProtocol::TCP;
}

void IotPacketHeader::setProtocol(IotProtocol protocol)
{
   header.protocol = protocol;
}

IotPacketType IotPacketHeader::getType()
{
   if(header.type == 0)
   {
      return IotPacketType::REGULAR;
   }

   return IotPacketType::ACK;
}

void IotPacketHeader::setType(IotPacketType type)
{
   header.type = type;
}

uint8_t IotPacketHeader::getPort()
{
   return header.port;
}

void IotPacketHeader::setPort(uint8_t port)
{
   header.port = port;
}

uint8_t IotPacketHeader::getTTL()
{
   return header.ttl;
}

void IotPacketHeader::setTTL(uint8_t ttl)
{
   header.ttl = ttl;
}

uint8_t IotPacketHeader::getPayloadSize()
{
   return header.payloadLength;
}

void IotPacketHeader::setPayloadSize(uint8_t size)
{
   if (size <= DEFAULT_PACKET_PAYLOAD_SIZE)
   {
      header.payloadLength= size;
   }
   else
   {
      header.payloadLength = DEFAULT_PACKET_PAYLOAD_SIZE;
   }
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

