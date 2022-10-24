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

uint8_t IotPacket::getId()
{
   uint8_t id = 0;
   uint8_t* ptr = (uint8_t*)(&header);
   for(uint8_t index = 0 ; index < this->getPacketSize(); index ++)
   {
      // iterate over the header and payload (payload is located directly after header)
      // and calculate the sum
      id += ptr[index];
   }

   return id;
}
