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
#include "../../../mesh/network/interface/Receiver.h"
#include "../../../mesh/network/interface/Transmitter.h"
#include "../../../mesh/network/packet/core/IotPacket.h"
#include "../../../mesh/transport/packet/IncomingTransportPacket.h"
#include "../flooder/Flooder.h"

#define INCOMMING_TRANSPORT_PACKETS_SIZE 3

enum TcpTransmitionState
{
	IDLE = 0,
	WAITING_FOR_ACK = 1
};

struct PingResult
{
	bool success;
	uint8_t packetSize;
	unsigned long timeInUs;
};

class Flooder;

class Interface
{
	friend class Flooder;

private:
	Flooder *flooder;
	Device* device;
	Receiver receiver;
	Transmitter transmitter;
	TcpTransmitionState tcpTransmitionState = IDLE;
	PacketCounters packetCounters;

	FixedSizeArray<IncomingTransportPacket, INCOMMING_TRANSPORT_PACKETS_SIZE> incomingTransportPackets;

	bool sendPacket(IotPacket* packet, uint8_t dstAddress);
	bool sendPacket(IotPacket* packet);
	bool sendTcpPacket(IotPacket* packet);
	bool sendUdpPacket(IotPacket* packet);

	bool hasAckArrived(IotPacket* sentPacket);
	void processIncomingPackets();

	bool floodToTransmitter(IotPacket* sentPacket);
public:
	Interface(Device *device);
	Device* getDevice();
	String getName();
	void setFlooder(Flooder *flooder);
	bool up();
	bool isUp();
	bool powerDown();
	bool isChipConnected();
	PingResult ping(uint8_t dstAddress);
	bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
	PacketCounters* getCounters();
	FixedSizeArray<IncomingTransportPacket, INCOMMING_TRANSPORT_PACKETS_SIZE>* getIncomingTransportPackets();

    void loop();
};

#endif /* INTERFACE_INTERFACE_H_ */
