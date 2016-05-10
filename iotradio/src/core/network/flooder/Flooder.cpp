/*
 * Flooder.cpp
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#include "Flooder.h"

void Flooder::setRF24Interface(Interface *interface)
{
	this->interface = interface;
}

void Flooder::flood(IotPacket* packet)
{
	if(packet->getSrcAddress() == interface->getIpAddress())
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
	interface->floodToTransmitter(packet);
	counters.incFloodedCount();
}

FlooderCounters* Flooder::getCounters()
{
	return &counters;
}
