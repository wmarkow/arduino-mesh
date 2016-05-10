/*
 * Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>
#include <string.h>

class Device
{
public:
	virtual bool up() = 0;
	virtual bool isChipConnected() = 0;
	virtual bool available() = 0;
	virtual void read(void* data, uint8_t size) = 0;
	virtual bool write(void* data, uint8_t size) = 0;

	virtual String getLinkAddress() = 0;
	virtual uint8_t getPALevel() = 0;
	virtual uint8_t getDataRate() = 0;
	virtual uint8_t getPayloadSize() = 0;
	virtual uint8_t getRFChannel() = 0;
	virtual String getModel() = 0;

};



#endif /* DEVICE_H_ */
