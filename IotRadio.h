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

#define IOT_HARDWARE_CE_PIN 7
#define IOT_HARDWARE_CS_PIN 8
#define IOT_ADDRESS_LENGTH 5

#define NETWORK_LAYER_INCOMING_PACKETS_NUMBER 5
#define APPLICATION_LAYER_INCOMING_PACKETS_NUMBER 5

enum TransmitterState
{
	IDLE = 0,
	WAITING_FOR_ACK = 1
};

class IotRadio
{
  private:
	byte linkAddress[IOT_ADDRESS_LENGTH];
	byte ipAddress;
	RF24 radio;
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

  public:
	IotRadio();
	bool begin();
	void setIpAddress(uint8_t address);
    bool ping(uint8_t dstAddress);
    void arduinoLoop();
};



#endif /* IOTRADIO_H_ */
