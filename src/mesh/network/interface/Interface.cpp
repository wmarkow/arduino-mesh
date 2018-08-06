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
   packet->setId(IotPacket::generateNextId());

   return sendPacket(packet);
}

void Interface::setIpAddress(uint8_t ipAddress)
{
   this->ipAddress = ipAddress;
}

void Interface::setWiresharkEnabled(bool enabled)
{
   this->wiresharkEnabled = enabled;
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
   if (wiresharkEnabled == false)
   {
      return;
   }

   Serial.print(F("><(((*>   "));
   Serial.print(millisToHMS(millis()));
   Serial.print(F(" "));
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

// macros from DateTime.h
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

String Interface::millisToHMS(unsigned long millis)
{
   unsigned long time = millis / 1000;
   uint8_t hours = numberOfHours(time);
   uint8_t minutes = numberOfMinutes(time);
   uint8_t seconds = numberOfSeconds(time);
   uint16_t secondFraction = millis % 1000;

   String result;
   if (hours < 10)
   {
      result += "0";
   }
   result += hours;
   result += ":";
   if (minutes < 10)
   {
      result += "0";
   }
   result += minutes;
   result += ":";
   if (seconds < 10)
   {
      result += "0";
   }
   result += seconds;
   result += ".";
   if (secondFraction < 10)
   {
      result += "00";
   }
   else if (secondFraction < 100)
   {
      result += "0";
   }
   result += secondFraction;

   return result;
}
