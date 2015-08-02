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
#include "IotPacket.h"

#define IOT_HARDWARE_CE_PIN 7
#define IOT_HARDWARE_CS_PIN 8
#define IOT_ADDRESS_LENGTH 5

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

	bool sendPacket(GenericPacketData* packet, uint8_t dstAddress);
	bool sendPacket(GenericPacketData* packet);
	bool sendPacketWaitForAck(GenericPacketData* packet);
	bool write(GenericPacketData* packet);
	bool hasAckArrived(GenericPacketData* sentPacket);
	bool readIncomingPacket(GenericPacketData* incomingPacket);
	void processIncomingPackets();
	void debugHexPrintToSerial(void* object, uint8_t length);

  public:
	IotRadio();
	bool begin();
	void setIpAddress(uint8_t address);
    bool ping(uint8_t dstAddress);
};



#endif /* IOTRADIO_H_ */
