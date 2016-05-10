/*
 * RF24Receiver.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24RECEIVER_H_
#define INTERFACE_RF24RECEIVER_H_

#include "../../../link/rf24/RF24Device.h"
#include "../../packet/core/IotPacket.h"
#include "../../../list/StaticList.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3

class RF24Receiver
{
private:
	RF24Device *rf24Device;
	StaticList<IotPacket> incomingPackets;

	bool available();
	bool readIncomingPacket();
	/*void debugHexPrintToSerial(void* object, uint8_t length);*/
public:
	RF24Receiver(RF24Device* rf24Device);

	void loop();
	StaticList<IotPacket>* getIncomingPackets();
};

#endif /* INTERFACE_RF24RECEIVER_H_ */
