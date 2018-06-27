/*
 * RF24Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef LINK_RF24_RF24DEVICE_H_
#define LINK_RF24_RF24DEVICE_H_

#include <RF24.h>

#include "../../mesh/link/Device.h"

#define HC12_TXD_PIN 2
#define HC12_RXD_PIN 3
#define HC12_SET_PIN 1

#define HC12_DEFAULT_BAUDRATE 9600

#define IOT_ADDRESS_LENGTH 5
#define RF24_DEFAULT_PACKET_SIZE 32

class HC12Device : public Device
{
private:

public:
	HC12Device();
	bool up();
	bool powerDown();
	bool isChipConnected();
	bool available();
	void read(void* data, uint8_t size);
	bool write(void* data, uint8_t size);

	String getLinkAddress();
	int8_t getPALevelInDbm();
	uint16_t getDataRateInKbs();
	uint8_t getPayloadSize();
	uint8_t getRFChannel();
	String getModel();
};

#endif /* LINK_RF24_RF24DEVICE_H_ */
