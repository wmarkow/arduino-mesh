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
   isUpFlag = device->up();

   return isUpFlag;
}

bool Interface::isUp()
{
   if (!isUpFlag)
   {
      return false;
   }

   return isChipConnected();
}

bool Interface::powerDown()
{
   isUpFlag = !device->powerDown();

   return !isUpFlag;
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

   if (!isUp())
   {
      return pingResult;
   }

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

void Interface::wiresharkPacket(IotPacket* packet, bool isIncomingPacket)
{
   Serial.print(F("><(((*>   "));
   Serial.print(getName());
   Serial.print(F(" "));
   if (isIncomingPacket)
   {
      Serial.print(F("   In"));
   }
   else
   {
      Serial.print(F("Out"));
   }
   Serial.print(F(" "));
   Serial.print(packet->getId(), HEX);
   Serial.print(F(" "));
   Serial.print(packet->getSrcAddress(), HEX);
   Serial.print(F(" -> "));
   Serial.print(packet->getDstAddress(), HEX);
   Serial.print(F(" "));
   switch (packet->getProtocol())
   {
      case ICMP:
         Serial.print(F("ICMP"));
         break;
      case TCP:
         Serial.print(F(" TCP"));
         break;
      case UDP:
         Serial.print(F(" UDP"));
         break;
      default:
         Serial.print(packet->getProtocol());
   }
   Serial.print(F(" "));
   if (packet->getType() == REGULAR)
   {
      Serial.print(F("REG"));
   }
   else
   {
      Serial.print(F("ACK"));
   }
   Serial.print(F(" "));
   Serial.print(packet->getTTL(), HEX);
   Serial.println();
}
