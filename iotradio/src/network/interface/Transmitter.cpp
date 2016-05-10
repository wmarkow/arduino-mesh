/*
 * RF24Transmitter.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#include "Transmitter.h"

IotPacket outgoingPacketsTable[OUTGOING_PACKETS_BUFFER_SIZE];

Transmitter::Transmitter(Device* device) : outgoingPackets(StaticList<IotPacket>(outgoingPacketsTable, OUTGOING_PACKETS_BUFFER_SIZE))
{
	this->device = device;
}

void Transmitter::loop()
{
	if(this->state == TRANSMITTER_STATE_IDLE) {
		if(outgoingPackets.getSize() > 0){

			unsigned long randomMillis = random(0, 20);
			unsigned long currentMillis = millis();

			this->waitFinishTimeInMillis = currentMillis + randomMillis;
			this->state = TRANSMITTER_STATE_WAITING;
		}
		return;
	}

	if(this->state == TRANSMITTER_STATE_WAITING) {
		if(millis() > this->waitFinishTimeInMillis) {
			this->state = TRANSMITTER_STATE_SENDING;
		}

		return;
	}

	if(this->state == TRANSMITTER_STATE_SENDING) {
		if(outgoingPackets.getSize() > 0) {
			write(outgoingPackets.get(0));
			outgoingPackets.remove(0);
		}

		this->state = TRANSMITTER_STATE_IDLE;

		return;
	}
}

bool Transmitter::addPacketToTransmissionQueue(IotPacket* packet)
{
	return outgoingPackets.add(packet);
}

bool Transmitter::write(IotPacket* packet)
{
	return device->write(packet, DEFAULT_PACKET_SIZE);
}

