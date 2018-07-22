/*
 * RF24Interface.h
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include "../../../mesh/link/Device.h"
#include "../../../mesh/network/interface/PacketCounters.h"
#include "../../../mesh/network/interface/Transmitter.h"
#include "../../../mesh/network/packet/core/IotPacket.h"
#include "../../../mesh/network/packet/AckPacket.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3

struct PingResult
{
	bool success;
	uint8_t packetSize;
	unsigned long timeInUs;
};

class Flooder;
class Host;

class Interface
{
	friend class Flooder;
	friend class Host;
private:
	Flooder *flooder;
	Device* device;
	Transmitter transmitter;
	IotPacket* tcpPacketWaitingForAck;
	bool ackReceived;
	PacketCounters packetCounters;

	FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE> incomingPackets;

	bool sendPacket(IotPacket* packet, uint8_t dstAddress);
	bool sendPacket(IotPacket* packet);
	bool sendTcpPacket(IotPacket* packet);
	bool sendUdpPacket(IotPacket* packet);

	bool doesAckMatchToPacket(AckPacket* ackPacket, IotPacket* tcpPacket);
	bool readIncomingPacket();

	bool floodToTransmitter(IotPacket* sentPacket);
	void setFlooder(Flooder *flooder);
public:
	Interface(Device *device);
	Device* getDevice();
	String getName();
	bool up();
	bool isUp();
	bool powerDown();
	bool isChipConnected();
	PingResult ping(uint8_t dstAddress);
	bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
	PacketCounters* getCounters();

    void loop();
};

#endif /* INTERFACE_INTERFACE_H_ */
