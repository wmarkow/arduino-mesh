/*
 * RF24Transmitter.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */


#include "RF24Transmitter.h"

RF24Transmitter::RF24Transmitter(RF24* rf24)
{
	this->rf24 = rf24;
}

void RF24Transmitter::loop()
{
	// do nothing for now
}

bool RF24Transmitter::addPacketToTransmissionQueue(GenericPacketData* packet)
{
	return write(packet);
}

bool RF24Transmitter::write(GenericPacketData* packet)
{
	rf24->stopListening();

	bool result = rf24->write(packet, DEFAULT_PACKET_SIZE);

	rf24->startListening();

	return result;
}

