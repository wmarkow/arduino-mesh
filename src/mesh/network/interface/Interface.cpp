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

Interface::Interface(Device *device)
{
   this->device = device;
   tcpPacketWaitingForAck = NULL;
   ackReceived = false;
   ipAddress = 1;
}

Device* Interface::getDevice()
{
   return device;
}

String Interface::getName()
{
   return device->getInterfaceName();
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
   writeOutgoingPacket();
   readIncomingPacket();
}

InterfaceCounters* Interface::getCounters()
{
   return &counters;
}

FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE>* Interface::getIncomingPackets()
{
   return &incomingPackets;
}

bool Interface::sendPacket(IotPacket* packet, uint8_t dstAddress)
{
   packet->setSrcAddress(ipAddress);
   packet->setDstAddress(dstAddress);

   return sendPacket(packet);
}

void Interface::setIpAddress(uint8_t ipAddress)
{
   this->ipAddress = ipAddress;
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
   addPacketToTransmissionQueue(packet);
   tcpPacketWaitingForAck = packet;
   ackReceived = false;

   unsigned long startedWaitingAtMicros = micros();

   while (ackReceived == false)
   {
      loop();
      if (micros() - startedWaitingAtMicros > 400000)
      {
         tcpPacketWaitingForAck = NULL;
         counters.incTransmittedTcpFailed();

         return false;
      }
   }

   tcpPacketWaitingForAck = NULL;
   counters.incTransmittedTcpSuccess();

   return true;
}

bool Interface::sendUdpPacket(IotPacket* packet)
{
   if (packet->getType() == ACK)
   {
      counters.incTransmittedUdpAck();
   }
   else
   {
      counters.incTransmittedUdpOther();
   }

   return addPacketToTransmissionQueue(packet);
}
