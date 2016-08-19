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

#if defined(ESP8266)
	#define IOT_HARDWARE_CE_PIN D4
	#define IOT_HARDWARE_CS_PIN D8
#else
	#define IOT_HARDWARE_CE_PIN 9
	#define IOT_HARDWARE_CS_PIN 10
#endif

#define IOT_ADDRESS_LENGTH 5
#define RF24_DEFAULT_PACKET_SIZE 32

class RF24Device : public Device
{
private:
	RF24 rf24;
	byte linkAddress[IOT_ADDRESS_LENGTH];

	// helper methods to extends RF24 functionality
	uint8_t read_register(uint8_t reg);
	inline void beginTransaction();
	inline void endTransaction();
	void csn(bool mode);
public:
	RF24Device();
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
