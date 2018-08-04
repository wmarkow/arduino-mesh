/*
 * Receiver.cpp
 *
 *  Created on: 26.07.2018
 *      Author: wmarkowski
 */

#include "../../../mesh/network/interface/Interface.h"

bool Interface::readIncomingPacket()
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

   wiresharkPacket(&incomingPacket, true);

   // Special packet: ACK addressed to me
   if (incomingPacket.getType() == ACK
         && incomingPacket.getDstAddress() == ipAddress)
   {
      if (tcpPacketWaitingForAck != NULL)
      {
         if (((AckPacket*) &incomingPacket)->doesAckMatchToPacket(
               tcpPacketWaitingForAck))
         {
            ackReceived = true;
         }
      }

      return false;
   }

   // Special packet: ping (ICMP) addressed to me
   if (incomingPacket.getProtocol() == ICMP
         && incomingPacket.getType() == REGULAR
         && incomingPacket.getDstAddress() == ipAddress)
   {
      AckPacket ackPacket(&incomingPacket);
      addPacketToTransmissionQueue(&ackPacket);
      counters.incTransmittedUdpAck();

      // ACK sent. Purge incoming ICMP packet
      return false;
   }

   // other incoming packet
   incomingPackets.add(&incomingPacket);

   return true;
}

