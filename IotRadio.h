/*
 * IotRadio.h
 *
 *  Created on: 25 lip 2015
 *      Author: witek
 */

#ifndef IOTRADIO_H_
#define IOTRADIO_H_

#include <SPI.h>
#include <RF24.h>
#include <SimpleList.h>
#include "IotPacket.h"

#define OFF 0
#define ON 1
#define IOT_DEBUG_WRITE_RADIO OFF

#if defined(ESP8266)
	#define IOT_HARDWARE_CE_PIN D1
	#define IOT_HARDWARE_CS_PIN D8
#else
	#define IOT_HARDWARE_CE_PIN 7
	#define IOT_HARDWARE_CS_PIN 8
#endif
#define IOT_ADDRESS_LENGTH 5

#define NETWORK_LAYER_INCOMING_PACKETS_NUMBER 5
#define APPLICATION_LAYER_INCOMING_PACKETS_NUMBER 5

enum TransmitterState
{
	IDLE = 0,
	WAITING_FOR_ACK = 1
};

class Counters
{
public:
	uint16_t totalPacketsSent;
	uint16_t totalPacketsLocalSent;
	uint16_t totalPacketsLocalTcpSent;
	uint16_t totalPacketsLocalTcpRegularSent;
	uint16_t totalPacketsLocalTcpAckSent;

	uint16_t totalPacketsReceived;
	uint16_t totalPacketsLocalReceived;
	uint16_t totalPacketsLocalTcpReceived;
	uint16_t totalPacketsLocalTcpRegularReceived;
	uint16_t totalPacketsLocalTcpAckReceived;

	uint16_t totalPacketIncomingDiscarded;

	Counters()
	{
		totalPacketsSent = 0;
		totalPacketsLocalSent = 0;
		totalPacketsLocalTcpSent = 0;
		totalPacketsLocalTcpRegularSent = 0;
		totalPacketsLocalTcpAckSent = 0;

		totalPacketsReceived = 0;
		totalPacketsLocalReceived = 0;
		totalPacketsLocalTcpReceived = 0;
		totalPacketsLocalTcpRegularReceived = 0;
		totalPacketsLocalTcpAckReceived = 0;

		totalPacketIncomingDiscarded = 0;
	}

	void printCounters()
	{
		Serial.print("totalPacketsSent                    : ");
		Serial.println(totalPacketsSent);
		Serial.print("totalPacketsLocalSent               : ");
		Serial.println(totalPacketsLocalSent);
		Serial.print("totalPacketsLocalTcpSent            : ");
		Serial.println(totalPacketsLocalTcpSent);
		Serial.print("totalPacketsLocalTcpRegularSent     : ");
		Serial.println(totalPacketsLocalTcpRegularSent);
		Serial.print("totalPacketsLocalTcpAckSent         : ");
		Serial.println(totalPacketsLocalTcpAckSent);

		Serial.print("totalPacketsReceived                : ");
		Serial.println(totalPacketsReceived);
		Serial.print("totalPacketsLocalReceived           : ");
		Serial.println(totalPacketsLocalReceived);
		Serial.print("totalPacketsLocalTcpReceived        : ");
		Serial.println(totalPacketsLocalTcpReceived);
		Serial.print("totalPacketsLocalTcpRegularReceived : ");
		Serial.println(totalPacketsLocalTcpRegularReceived);
		Serial.print("totalPacketsLocalTcpAckReceived     : ");
		Serial.println(totalPacketsLocalTcpAckReceived);

		Serial.print("totalPacketIncomingDiscarded        : ");
		Serial.println(totalPacketIncomingDiscarded);
	}
};

class IotRadio
{
  private:
	byte linkAddress[IOT_ADDRESS_LENGTH];
	byte ipAddress;
	RF24 radio;
	bool beginResult;
	Counters counters;
	TransmitterState transmitterState = IDLE;

	SimpleList<GenericPacketData> preProcessedIncomingPackets;
	SimpleList<GenericPacketData> incomingPackets;

	bool sendPacket(GenericPacketData* packet, uint8_t dstAddress);
	bool sendPacket(GenericPacketData* packet);
	bool sendPacketWaitForAck(GenericPacketData* packet);
	bool write(GenericPacketData* packet);
	bool hasAckArrived(GenericPacketData* sentPacket);
	bool readIncomingPacket();
	void debugHexPrintToSerial(void* object, uint8_t length);
	void processIncomingPackets();
	void updateSentCounters(GenericPacketData* packet);
	void updateReceivedCounters(GenericPacketData* packet);
	bool beginLowLevel();
  public:
	IotRadio();
	bool begin();
	bool isChipConnected();
	void setIpAddress(uint8_t address);
    bool ping(uint8_t dstAddress);
    Counters* getCounters();
    uint8_t getIpAddress();
    String getLinkAddress();
    uint8_t getPALevel();
    uint8_t getDataRate();
    uint8_t getPayloadSize();
    uint8_t getRFChannel();
    String getModel();
    void arduinoLoop();
};



#endif /* IOTRADIO_H_ */
