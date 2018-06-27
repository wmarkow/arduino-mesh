/*
 * RF24Device.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef MESH_LINK_HC12DEVICE_H_
#define MESH_LINK_HC12DEVICE_H_

#include "HC12.h"

#include "../../mesh/link/Device.h"

class HC12Device : public Device
{
private:
   HC12 hc12;
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
	String getInterfaceName();
};

#endif /* MESH_LINK_HC12DEVICE_H_ */
