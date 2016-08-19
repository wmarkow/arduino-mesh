/*
 * Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <Arduino.h>

class Device
{
public:
	virtual bool up() = 0;
	virtual bool powerDown() = 0;
	virtual bool isChipConnected() = 0;
	virtual bool available() = 0;
	virtual void read(void* data, uint8_t size) = 0;
	virtual bool write(void* data, uint8_t size) = 0;

	virtual String getLinkAddress() = 0;
	virtual int8_t getPALevelInDbm() = 0;
	virtual uint16_t getDataRateInKbs() = 0;
	virtual uint8_t getPayloadSize() = 0;
	virtual uint8_t getRFChannel() = 0;
	virtual String getModel() = 0;

};



#endif /* DEVICE_H_ */
