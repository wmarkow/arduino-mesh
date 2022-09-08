/*
 * RF24Transmitter.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#include <Arduino.h>
#include "Interface.h"

void Interface::writeOutgoingPacket()
{
   if (this->state == TRANSMITTER_STATE_IDLE)
   {
      if (outgoingPackets.getSize() > 0)
      {

         unsigned long randomMillis = random(0, 20);
         unsigned long currentMillis = millis();

         this->waitFinishTimeInMillis = currentMillis + randomMillis;
         this->state = TRANSMITTER_STATE_WAITING;
      }
      return;
   }

   if (this->state == TRANSMITTER_STATE_WAITING)
   {
      if (millis() > this->waitFinishTimeInMillis)
      {
         this->state = TRANSMITTER_STATE_SENDING;
      }

      return;
   }

   if (this->state == TRANSMITTER_STATE_SENDING)
   {
      if (outgoingPackets.getSize() > 0)
      {
         wiresharkPacket(outgoingPackets.peek(0), false);

         if(outgoingPackets.peek(0)->getDstAddress() == this->ipAddress 
            && outgoingPackets.peek(0)->getSrcAddress() == this->ipAddress)
         {
            // Packet sent from localhost to localhost:
            // use virtual loopback interface to process it.
            this->processIncomingPacket(outgoingPackets.peek(0));
         }
         else
         {
            // Packet designated to other node:
            // use a real device to sent it.
            device->writePacket(outgoingPackets.peek(0));
         }

         outgoingPackets.remove(0);
      }

      this->state = TRANSMITTER_STATE_IDLE;

      return;
   }
}

bool Interface::floodPacket(IotPacket* packet)
{
   if (!isUp())
   {
      return false;
   }

   return outgoingPackets.add(packet);
}
