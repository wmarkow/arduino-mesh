/*
 * Flooder.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "Flooder.h"

void Flooder::setRF24Interface(RF24Interface *rf24Interface)
{
	this->rf24Interface = rf24Interface;
}

void Flooder::flood(GenericPacketData* packet)
{
	if(packet->getSrcAddress() == rf24Interface->getIpAddress())
	{
		// I'm originator of this packet; drop it
		counters.incDroppedCount();

		return;
	}

	if(packet->decrementTTL() == 0)
	{
		// TTL reached 0; drop packet
		counters.incDroppedCount();

		return;
	}

	// flood packet
	rf24Interface->write(packet);
	counters.incFloodedCount();
}

FlooderCounters* Flooder::getCounters()
{
	return &counters;
}


