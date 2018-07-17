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
      transmitter(Transmitter(device)), receiver(Receiver(device))
{
   this->device = device;
   flooder = NULL;
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
   processIncomingPackets();
   receiver.loop();
}

PacketCounters* Interface::getCounters()
{
   return &packetCounters;
}

FixedSizeArray<IncomingTransportPacket, INCOMMING_TRANSPORT_PACKETS_SIZE>* Interface::getIncomingTransportPackets()
{
   return &incomingTransportPackets;
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
   tcpTransmitionState = WAITING_FOR_ACK;

   unsigned long startedWaitingAtMicros = micros();

   while (!hasAckArrived(packet))
   {
      if (micros() - startedWaitingAtMicros > 400000)
      {
         tcpTransmitionState = IDLE;
         packetCounters.incTransmittedTcpFailed();

         return false;
      }
   }

   tcpTransmitionState = IDLE;
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

bool Interface::hasAckArrived(IotPacket* sentPacket)
{
   loop();

   for (uint8_t index = 0; index < receiver.getIncomingPackets()->getSize();
         index++)
   {
      IotPacket* itr = receiver.getIncomingPackets()->peek(index);

      if (itr->getType() != ACK)
      {
         continue;
      }

      if (itr->getDstAddress() != sentPacket->getSrcAddress())
      {
         continue;
      }

      if (itr->getSrcAddress() != sentPacket->getDstAddress())
      {
         continue;
      }

      if (itr->getId() != sentPacket->getId())
      {
         continue;
      }

      if (itr->getProtocol() != sentPacket->getProtocol())
      {
         continue;
      }

      receiver.getIncomingPackets()->remove(index);

      return true;
   }

   return false;
}

void Interface::processIncomingPackets()
{
   for (uint8_t index = 0; index < receiver.getIncomingPackets()->getSize();
         index++)
   {
      IotPacket* itr = receiver.getIncomingPackets()->peek(index);

      if (itr->getDstAddress() != Localhost.getIpAddress())
      {
         // this packet is not addressed for me; flood that packet
         flooder->flood(itr);
         receiver.getIncomingPackets()->remove(index);
         index--;
         continue;
      }

      if (itr->getType() == ACK && tcpTransmitionState != WAITING_FOR_ACK)
      {
         // the transmitter is not waiting for ACK; purge that ACK
         receiver.getIncomingPackets()->remove(index);
         index--;
         continue;
      }

      if (itr->getProtocol() == ICMP && itr->getType() == REGULAR)
      {
         AckPacket ackPacket(itr);
         transmitter.addPacketToTransmissionQueue(&ackPacket);
         this->packetCounters.incTransmittedUdpAck();

         // ACK sent. Purge incoming ICMP packet
         receiver.getIncomingPackets()->remove(index);
         index--;
         continue;
      }

      // Some other packet addressed to me
      if (itr->getProtocol() == TCP && itr->getType() == REGULAR)
      {
         AckPacket ackPacket(itr);
         transmitter.addPacketToTransmissionQueue(&ackPacket);
         this->packetCounters.incTransmittedUdpAck();

         IncomingTransportPacket itp;
         itp.srcAddress =
               receiver.getIncomingPackets()->peek(index)->getSrcAddress();
         memcpy(itp.payload,
               receiver.getIncomingPackets()->peek(index)->payload,
               DEFAULT_PACKET_PAYLOAD_SIZE);

         if (this->incomingTransportPackets.isFull())
         {
            this->incomingTransportPackets.remove(0);
         }
         this->incomingTransportPackets.add(&itp);

         receiver.getIncomingPackets()->remove(index);

         index--;
         continue;
      }

      // some kind of unknown packet; purge it
      receiver.getIncomingPackets()->remove(index);
      index--;
   }
}

bool Interface::floodToTransmitter(IotPacket* packet)
{
   return transmitter.addPacketToTransmissionQueue(packet);
}
