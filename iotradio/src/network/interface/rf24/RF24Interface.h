/*
 * RF24Interface.h
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24INTERFACE_H_
#define INTERFACE_RF24INTERFACE_H_

#include "../../../link/rf24/RF24Device.h"
#include "../../packet/core/IotPacket.h"
#include "../PacketCounters.h"
#include "../../flooder/Flooder.h"
#include "Receiver.h"
#include "Transmitter.h"

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

class RF24Interface
{
	friend class Flooder;

private:
	Flooder *flooder;
	RF24Device* rf24;
	Receiver receiver;
	Transmitter transmitter;
	byte ipAddress;
	TcpTransmitionState tcpTransmitionState = IDLE;
	PacketCounters packetCounters;

	bool sendPacket(IotPacket* packet, uint8_t dstAddress);
	bool sendPacket(IotPacket* packet);
	bool sendTcpPacket(IotPacket* packet);
	bool sendUdpPacket(IotPacket* packet);

	bool hasAckArrived(IotPacket* sentPacket);
	void processIncomingPackets();

	bool floodToTransmitter(IotPacket* sentPacket);
public:
	RF24Interface(RF24Device *rf24Device);
	RF24Device* getRF24Device();
	void setFlooder(Flooder *flooder);
	bool up();
	bool isUp();
	bool isChipConnected();
	void setIpAddress(uint8_t address);
	uint8_t getIpAddress();
	PingResult ping(uint8_t dstAddress);
	bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
	PacketCounters* getCounters();

    void loop();
};

#endif /* INTERFACE_RF24INTERFACE_H_ */
