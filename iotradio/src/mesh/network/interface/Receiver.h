/*
 * RF24Receiver.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RECEIVER_H_
#define INTERFACE_RECEIVER_H_

#include <StaticVector.h>
#include "../../link/Device.h"
#include "../packet/core/IotPacket.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3

class Receiver
{
private:
	Device *device;
	StaticVector<IotPacket> incomingPackets;

	bool available();
	bool readIncomingPacket();
	/*void debugHexPrintToSerial(void* object, uint8_t length);*/
public:
	Receiver(Device* rf24Device);

	void loop();
	StaticVector<IotPacket>* getIncomingPackets();
};

#endif /* INTERFACE_RECEIVER_H_ */
