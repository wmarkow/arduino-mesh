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
   return header[0];
}

void IotPacketHeader::setSrcAddress(uint8_t address)
{
   header[0] = address;
}

uint8_t IotPacketHeader::getDstAddress()
{
   return header[1];
}

void IotPacketHeader::setDstAddress(uint8_t address)
{
   header[1] = address;
}

IotProtocol IotPacketHeader::getProtocol()
{
   if((header[2] & 0b10000000) == 0)
   {
      return IotProtocol::UDP;
   }

   return IotProtocol::TCP;
}

void IotPacketHeader::setProtocol(IotProtocol protocol)
{
   header[2] &= 0b01111111; // by default mark as UDP

   if(IotProtocol::TCP == protocol)
   {
      header[2] |= 0b10000000;
   }
}

IotPacketType IotPacketHeader::getType()
{
   if((header[2] & 0b01000000) == 0)
   {
      return IotPacketType::REGULAR;
   }

   return IotPacketType::ACK;
}

void IotPacketHeader::setType(IotPacketType type)
{
   header[2] &= 0b10111111; // by default mark as REGULAR

   if(IotPacketType::ACK == type)
   {
      header[2] |= 0b01000000;
   }
}

uint8_t IotPacketHeader::getPort()
{
   return (header[2] & 0b00111110) >> 1;
}

void IotPacketHeader::setPort(uint8_t port)
{
   header[2] &= 0b11000001;
   header[2] |= (port & 0b00011111) << 1;
}

uint8_t IotPacketHeader::getTTL()
{
   uint8_t ttl = header[2] << 4;
   ttl |= header[3] >> 5;

   return ttl & 0b00011111;
}

void IotPacketHeader::setTTL(uint8_t ttl)
{
   header[2] &= 0b11111110;
   header[3] &= 0b00011111;

   header[2] |= (ttl & 0b00001111) >> 3;
   header[3] |= (ttl & 0b00001111) << 5;
}

uint8_t IotPacketHeader::getPayloadSize()
{
   return header[3] & 0b00011111;
}

void IotPacketHeader::setPayloadSize(uint8_t size)
{
   header[3] &= 0b11100000;

   if (size <= DEFAULT_PACKET_PAYLOAD_SIZE)
   {
      header[3] |= size & 0b00011111;
   }
   else
   {
      header[3] |= DEFAULT_PACKET_PAYLOAD_SIZE & 0b00011111;
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

