/*
 * RF24Interface.h
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include <FixedSizeArray.h>

#include "../../../mesh/link/Device.h"
#include "../../../mesh/network/packet/core/IotPacket.h"
#include "../../../mesh/network/packet/AckPacket.h"
#include "InterfaceCounters.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3
#define OUTGOING_PACKETS_BUFFER_SIZE 3

#define TRANSMITTER_STATE_IDLE 0
#define TRANSMITTER_STATE_WAITING 1
#define TRANSMITTER_STATE_SENDING 2

struct PingResult
{
   bool success;
   uint8_t packetSize;
   unsigned long timeInUs;
};

class MeshNode;

class Interface
{
   friend class MeshNode;
private:
   uint8_t ipAddress;
   InterfaceCounters counters;
   Device* device;
   bool isUpFlag = false;

   /** Receiver related variables **/
   FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE> incomingPackets;

   /** Transmitter related variables **/
   FixedSizeArray<IotPacket, OUTGOING_PACKETS_BUFFER_SIZE> outgoingPackets;
   uint8_t state = TRANSMITTER_STATE_IDLE;
   unsigned long waitFinishTimeInMillis = 0;
   IotPacket* tcpPacketWaitingForAck;
   bool ackReceived;

   bool sendPacket(IotPacket* packet, uint8_t dstAddress);
   bool sendPacket(IotPacket* packet);
   bool sendTcpPacket(IotPacket* packet);
   bool sendUdpPacket(IotPacket* packet);

   bool readIncomingPacket();
   void writeOutgoingPacket();

   bool addPacketToTransmissionQueue(IotPacket* packet);
   void wiresharkPacket(IotPacket* packet, bool isIncomingPacket);
public:
   Interface(Device *device);

   void setIpAddress(uint8_t ipAddress);
   InterfaceCounters* getCounters();
   FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE>* getIncomingPackets();
   Device* getDevice();

   String getName();
   bool up();
   bool isUp();
   bool powerDown();
   bool isChipConnected();

   PingResult ping(uint8_t dstAddress);
   bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);

   void loop();
};

#endif /* INTERFACE_INTERFACE_H_ */
