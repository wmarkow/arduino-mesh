/*
 * RF24Transmitter.h
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#ifndef INTERFACE_RF24TRANSMITTER_H_
#define INTERFACE_RF24TRANSMITTER_H_

#include <RF24.h>
#include "IotPacket.h"

class RF24Transmitter
{
private:
	RF24 *rf24;

	bool write(GenericPacketData* packet);
public:
	RF24Transmitter(RF24* rf24);

	void loop();
	bool addPacketToTransmissionQueue(GenericPacketData* packet);
};


#endif /* INTERFACE_RF24TRANSMITTER_H_ */
