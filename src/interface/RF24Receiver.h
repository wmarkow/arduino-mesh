/*
 * RF24Receiver.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24RECEIVER_H_
#define INTERFACE_RF24RECEIVER_H_

#include <RF24.h>
#include <SimpleList.h>
#include "IotPacket.h"

#define INCOMMING_PACKETS_BUFFER_SIZE 3

class RF24Receiver
{
private:
	RF24 *rf24;
	SimpleList<GenericPacketData> incomingPackets;

	bool available();
	bool readIncomingPacket();
	/*void debugHexPrintToSerial(void* object, uint8_t length);*/
public:
	RF24Receiver(RF24* rf24);

	void loop();
	SimpleList<GenericPacketData>* getIncomingPackets();
};

#endif /* INTERFACE_RF24RECEIVER_H_ */
