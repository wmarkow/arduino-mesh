/*
 * RF24Device.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#include <SoftwareSerial.h>

#include "HC12Device.h"

HC12Device::HC12Device() :
      hc12()
{

}

bool HC12Device::up()
{
   hc12.begin();

   return isChipConnected();
}

bool HC12Device::powerDown()
{
   return true;
}

bool HC12Device::isChipConnected()
{
   if (!chipConnected)
   {
      chipConnected = hc12.icChipConnected();
   }

   return chipConnected;
}

bool HC12Device::readPacket(IotPacket* data)
{
   do
   {
      switch (receiverState)
      {
         case HC12_RECEIVE_WAIT_FOR_PREAMBLE:
         {
            int receivedByte = hc12.read();
            if (receivedByte == HC12_PREAMBLE)
            {
               this->receiverState = HC12_RECEIVE_WAIT_FOR_PAYLOAD_SIZE;
               lastReceivedByteInMillis = millis();
            }
            else
            {
               resetReceiver();
            }
            break;
         }
         case HC12_RECEIVE_WAIT_FOR_PAYLOAD_SIZE:
         {
            if (millis()
                  - lastReceivedByteInMillis> HC12_OUT_OF_SYNC_TIMEOUT_IN_MILLIS)
            {
               // out of sync
               resetReceiver();

               break;
            }

            if (hc12.available() == 0)
            {
               return false;
            }

            int receivedByte = hc12.read();
            if (receivedByte >= 0 && receivedByte <= DEFAULT_PACKET_SIZE)
            {
               this->receiverState = HC12_RECEIVE_WAIT_FOR_PAYLOAD;
               this->receiverPacketSize = receivedByte;
               lastReceivedByteInMillis = millis();
            }
            else
            {
               resetReceiver();
            }
            break;
         }
         case HC12_RECEIVE_WAIT_FOR_PAYLOAD:
         {
            if (millis()
                  - lastReceivedByteInMillis> HC12_OUT_OF_SYNC_TIMEOUT_IN_MILLIS)
            {
               // out of sync
               resetReceiver();

               break;
            }

            if (hc12.available() == 0)
            {
               return false;
            }

            int receivedByte = hc12.read();
            unsigned long currentReceivedByteInMillis = millis();

            receivedPacket[receiverIndex] = receivedByte;
            lastReceivedByteInMillis = currentReceivedByteInMillis;
            receiverIndex++;

            if (receiverIndex == receiverPacketSize)
            {
               // packet received
               memcpy(data, receivedPacket, receiverPacketSize);
               resetReceiver();

               return true;
            }
            break;
         }
         default:
         {
            resetReceiver();
         }
      }
   }
   while (hc12.available() > 0);

   return false;
}

bool HC12Device::writePacket(IotPacket* packet)
{
   hc12.write(HC12_PREAMBLE);
   hc12.write(packet->getPacketSize());
   return hc12.write((uint8_t*) packet, packet->getPacketSize());
}

String HC12Device::getLinkAddress()
{
   return F("NA");
}

int8_t HC12Device::getPALevelInDbm()
{
   return hc12.getTransmitterPowerInDbm();
}

uint16_t HC12Device::getDataRateInKbs()
{
   return hc12.getDataRateInKbs();

}

uint8_t HC12Device::getPayloadSize()
{
   return DEFAULT_PACKET_SIZE;
}

uint8_t HC12Device::getRFChannel()
{
   return hc12.getRFChannel();
}

String HC12Device::getModel()
{
   return F("HC12");
}

String HC12Device::getInterfaceName()
{
   return F("hc12");
}

void HC12Device::resetReceiver()
{
   this->receiverIndex = 0;
   this->receiverState = HC12_RECEIVE_WAIT_FOR_PREAMBLE;
   this->lastReceivedByteInMillis = 0;
}
