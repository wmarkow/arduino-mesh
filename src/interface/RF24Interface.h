/*
 * RF24Interface.h
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24INTERFACE_H_
#define INTERFACE_RF24INTERFACE_H_

#include <RF24.h>
#include <SimpleList.h>
#include "IotPacket.h"

#if defined(ESP8266)
	#define IOT_HARDWARE_CE_PIN D4
	#define IOT_HARDWARE_CS_PIN D8
#else
	#define IOT_HARDWARE_CE_PIN 7
	#define IOT_HARDWARE_CS_PIN 8
#endif

#define IOT_ADDRESS_LENGTH 5
#define PAYLOAD_SIZE 32
#define NETWORK_LAYER_INCOMING_PACKETS_NUMBER 5

enum TransmitterState
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

class RF24Interface
{
private:
	RF24 rf24;
	byte linkAddress[IOT_ADDRESS_LENGTH];
	byte ipAddress;
	TransmitterState transmitterState = IDLE;
	SimpleList<GenericPacketData> preProcessedIncomingPackets;

	bool available();
	bool sendPacket(GenericPacketData* packet, uint8_t dstAddress);
	bool sendPacket(GenericPacketData* packet);
	bool sendPacketWaitForAck(GenericPacketData* packet);
	bool write(GenericPacketData* packet);
	bool hasAckArrived(GenericPacketData* sentPacket);
	void processIncomingPackets();
	bool readIncomingPacket();
	void debugHexPrintToSerial(void* object, uint8_t length);

public:
	RF24Interface();
	bool up();
	bool isUp();
	bool isChipConnected();
	void setIpAddress(uint8_t address);uint8_t getIpAddress();
	PingResult ping(uint8_t dstAddress);

    String getLinkAddress();
    uint8_t getPALevel();
    uint8_t getDataRate();
    uint8_t getPayloadSize();
    uint8_t getRFChannel();
    String getModel();

    void loop();
};

#endif /* INTERFACE_RF24INTERFACE_H_ */
