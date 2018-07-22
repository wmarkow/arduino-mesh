/*
 * RF24Interface.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#include "../../../mesh/network/interface/Interface.h"

#include "../../../mesh/network/packet/AckPacket.h"
#include "../../../mesh/network/packet/PingPacket.h"
#include "../../../mesh/network/packet/TcpPacket.h"

#include "../host/Host.h"

Interface::Interface(Device *device) :
      transmitter(Transmitter(device))
{
   this->device = device;
   flooder = NULL;
   tcpPacketWaitingForAck = NULL;
   ackReceived = false;
}

Device* Interface::getDevice()
{
   return device;
}

String Interface::getName()
{
   return device->getInterfaceName();
}

void Interface::setFlooder(Flooder *flooder)
{
   this->flooder = flooder;
}

bool Interface::up()
{
   return device->up();
}

bool Interface::isUp()
{
   return isChipConnected();
}

bool Interface::powerDown()
{
   return device->powerDown();
}

bool Interface::isChipConnected()
{
   return device->isChipConnected();
}

PingResult Interface::ping(uint8_t dstAddress)
{
   PingPacket pingPacket;

   PingResult pingResult;
   pingResult.packetSize = pingPacket.getPacketSize();
   pingResult.success = false;
   pingResult.timeInUs = 0;

   unsigned long sentTime = micros();
   if (sendPacket(&pingPacket, dstAddress))
   {
      unsigned long gotTime = micros();
      pingResult.timeInUs = gotTime - sentTime;
      pingResult.success = true;
   }

   return pingResult;
}

bool Interface::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
   if (length > DEFAULT_PACKET_PAYLOAD_SIZE)
   {
      return false;
   }

   TcpPacket packet = TcpPacket(data, length);

   return sendPacket(&packet, dstAddress);
}

void Interface::loop()
{
   transmitter.loop();
   readIncomingPacket();
}

PacketCounters* Interface::getCounters()
{
   return &packetCounters;
}

bool Interface::sendPacket(IotPacket* packet, uint8_t dstAddress)
{
   packet->setSrcAddress(Localhost.getIpAddress());
   packet->setDstAddress(dstAddress);

   return sendPacket(packet);
}

bool Interface::sendPacket(IotPacket* packet)
{
   if (packet->getProtocol() == ICMP || packet->getProtocol() == TCP)
   {
      return sendTcpPacket(packet);
   }

   return sendUdpPacket(packet);
}

bool Interface::sendTcpPacket(IotPacket* packet)
{
   transmitter.addPacketToTransmissionQueue(packet);
   tcpPacketWaitingForAck = packet;
   ackReceived = false;

   unsigned long startedWaitingAtMicros = micros();

   while (ackReceived == false)
   {
      loop();
      if (micros() - startedWaitingAtMicros > 400000)
      {
         tcpPacketWaitingForAck = NULL;
         packetCounters.incTransmittedTcpFailed();

         return false;
      }
   }

   tcpPacketWaitingForAck = NULL;
   packetCounters.incTransmittedTcpSuccess();

   return true;
}

bool Interface::sendUdpPacket(IotPacket* packet)
{
   if (packet->getType() == ACK)
   {
      packetCounters.incTransmittedUdpAck();
   }
   else
   {
      packetCounters.incTransmittedUdpOther();
   }

   return transmitter.addPacketToTransmissionQueue(packet);
}

bool Interface::doesAckMatchToPacket(AckPacket* ackPacket, IotPacket* tcpPacket)
{
   if (ackPacket->getDstAddress() != tcpPacket->getSrcAddress())
   {
      return false;
   }

   if (ackPacket->getSrcAddress() != tcpPacket->getDstAddress())
   {
      return false;
   }

   if (ackPacket->getId() != tcpPacket->getId())
   {
      return false;
   }

   if (ackPacket->getProtocol() != tcpPacket->getProtocol())
   {
      return false;
   }

   return true;
}

bool Interface::floodToTransmitter(IotPacket* packet)
{
   return transmitter.addPacketToTransmissionQueue(packet);
}

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

   if (incomingPacket.getDstAddress() != Localhost.getIpAddress())
   {
      // this packet is not addressed for me; flood that packet
      flooder->flood(&incomingPacket);
      return false;
   }

   // Special packet: ACK addressed to me
   if (incomingPacket.getType() == ACK)
   {
      if (tcpPacketWaitingForAck != NULL)
      {
         if (doesAckMatchToPacket((AckPacket*) &incomingPacket,
               tcpPacketWaitingForAck))
         {
            ackReceived = true;
         }
      }

      return false;
   }

   // Special packet: ping (ICMP)
   if (incomingPacket.getProtocol() == ICMP
         && incomingPacket.getType() == REGULAR)
   {
      AckPacket ackPacket(&incomingPacket);
      transmitter.addPacketToTransmissionQueue(&ackPacket);
      this->packetCounters.incTransmittedUdpAck();

      // ACK sent. Purge incoming ICMP packet
      return false;
   }

   // other incoming packet
   incomingPackets.add(&incomingPacket);

   return true;
}
