/*
 * RF24Interface.h
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24INTERFACE_H_
#define INTERFACE_RF24INTERFACE_H_

#include <RF24.h>
#include "../packet/network/core/IotPacket.h"
#include "PacketCounters.h"
#include "RF24Receiver.h"
#include "RF24Transmitter.h"
#include "../flooder/Flooder.h"

#if defined(ESP8266)
	#define IOT_HARDWARE_CE_PIN D4
	#define IOT_HARDWARE_CS_PIN D8
#else
	#define IOT_HARDWARE_CE_PIN 7
	#define IOT_HARDWARE_CS_PIN 8
#endif

#define ON 1
#define OFF 0
#define IOT_DEBUG_WRITE_RADIO ON

#define IOT_ADDRESS_LENGTH 5
#define NETWORK_LAYER_INCOMING_PACKETS_NUMBER 5

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
	RF24 rf24;
	RF24Receiver receiver;
	RF24Transmitter transmitter;
	byte linkAddress[IOT_ADDRESS_LENGTH];
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
	RF24Interface();
	void setFlooder(Flooder *flooder);
	bool up();
	bool isUp();
	bool isChipConnected();
	void setIpAddress(uint8_t address);
	uint8_t getIpAddress();
	PingResult ping(uint8_t dstAddress);
	bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
	PacketCounters* getCounters();

    String getLinkAddress();
    uint8_t getPALevel();
    uint8_t getDataRate();
    uint8_t getPayloadSize();
    uint8_t getRFChannel();
    String getModel();

    void loop();
};

#endif /* INTERFACE_RF24INTERFACE_H_ */
