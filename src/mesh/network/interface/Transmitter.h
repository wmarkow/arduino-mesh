/*
 * RF24Transmitter.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_TRANSMITTER_H_
#define INTERFACE_TRANSMITTER_H_

#include <Array.h>

#include "../../link/Device.h"
#include "../packet/core/IotPacket.h"

#define OUTGOING_PACKETS_BUFFER_SIZE 3

#define TRANSMITTER_STATE_IDLE 0
#define TRANSMITTER_STATE_WAITING 1
#define TRANSMITTER_STATE_SENDING 2

class Transmitter
{
private:
	Device *device;
	Array<IotPacket, OUTGOING_PACKETS_BUFFER_SIZE> outgoingPackets;
	uint8_t state = TRANSMITTER_STATE_IDLE;
	unsigned long waitFinishTimeInMillis = 0;

	bool write(IotPacket* packet);
public:
	Transmitter(Device* device);

	void loop();
	bool addPacketToTransmissionQueue(IotPacket* packet);
};


#endif /* INTERFACE_TRANSMITTER_H_ */
