/*
 * RF24Transmitter.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24TRANSMITTER_H_
#define INTERFACE_RF24TRANSMITTER_H_

#include <RF24.h>
#include "../packet/IotPacket.h"
#include "../list/StaticList.h"

#define OUTGOING_PACKETS_BUFFER_SIZE 3

#define TRANSMITTER_STATE_IDLE 0
#define TRANSMITTER_STATE_WAITING 1
#define TRANSMITTER_STATE_SENDING 2

class RF24Transmitter
{
private:
	RF24 *rf24;
	StaticList<GenericPacketData> outgoingPackets;
	uint8_t state = TRANSMITTER_STATE_IDLE;
	unsigned long waitFinishTimeInMillis = 0;

	bool write(GenericPacketData* packet);
public:
	RF24Transmitter(RF24* rf24);

	void loop();
	bool addPacketToTransmissionQueue(GenericPacketData* packet);
};


#endif /* INTERFACE_RF24TRANSMITTER_H_ */
