/*
 * RF24Receiver.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RECEIVER_H_
#define INTERFACE_RECEIVER_H_

#include <FixedSizeArray.h>
#include "../../link/Device.h"
#include "../packet/core/IotPacket.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3

class Receiver
{
private:
	Device *device;
	FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE> incomingPackets;

	bool available();
	bool readIncomingPacket();
	/*void debugHexPrintToSerial(void* object, uint8_t length);*/
public:
	Receiver(Device* rf24Device);

	void loop();
	FixedSizeArray<IotPacket, INCOMMING_PACKETS_BUFFER_SIZE>* getIncomingPackets();
};

#endif /* INTERFACE_RECEIVER_H_ */
