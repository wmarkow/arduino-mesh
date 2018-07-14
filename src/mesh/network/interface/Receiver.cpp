/*
 * RF24Receiver.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#include "../../../mesh/network/interface/Receiver.h"

Receiver::Receiver(Device* device) :
      incomingPackets(
            FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE>())
{
   this->device = device;
}

void Receiver::loop()
{
   readIncomingPacket();
}

FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE>* Receiver::getIncomingPackets()
{
   return &incomingPackets;
}

bool Receiver::readIncomingPacket()
{
   IotPacket incomingPacket;
   if (device->readPacket(&incomingPacket) == false)
   {
      return false;
   }

   if (!device->isChipConnected())
   {
      return false;
   }

   if (incomingPackets.getSize() >= INCOMMING_PACKETS_BUFFER_SIZE)
   {
      // incoming queue is full, discard new packet
#if IOT_DEBUG_WRITE_RADIO == ON
      Serial.println(F("Discarding packet because incoming buffer is full"));
#endif

      return false;
   }

   incomingPackets.add(&incomingPacket);

   return true;
}
/*
 void RF24Interface::debugHexPrintToSerial(void* object, uint8_t length) {
 uint8_t* wsk = reinterpret_cast<uint8_t*>(object);
 for(uint8_t q = 0 ;q < PAYLOAD_SIZE ; q ++){
 Serial.print(*wsk++, HEX);
 Serial.print(" ");
 }
 Serial.println();
 }*/
