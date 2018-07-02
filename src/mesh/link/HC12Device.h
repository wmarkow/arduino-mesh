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
#include "../../mesh/network/packet/core/IotPacket.h"

class HC12Device : public Device
{
private:
   HC12 hc12;
   bool chipConnected = false;
public:
	HC12Device();
	bool up();
	bool powerDown();
	bool isChipConnected();
	bool available();
	bool readPacket(IotPacket* data);
	bool writePacket(IotPacket* packet);

	String getLinkAddress();
	int8_t getPALevelInDbm();
	uint16_t getDataRateInKbs();
	uint8_t getPayloadSize();
	uint8_t getRFChannel();
	String getModel();
	String getInterfaceName();
};

#endif /* MESH_LINK_HC12DEVICE_H_ */
