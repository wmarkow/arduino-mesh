/*
 * IotPacket.cpp
 *
 *  Created on: 27 lip 2015
 *      Author: witek
 */

#include "IotPacket.h"

IotPacket::IotPacket() :
      IotPacketHeader()
{

}

uint8_t IotPacket::getPacketSize()
{
   return DEFAULT_PACKET_HEADER_SIZE + getPayloadSize();
}

uint8_t IotPacket::generateNextId()
{
   idSequence++;
   return idSequence;
}

uint8_t IotPacket::idSequence = 0;
