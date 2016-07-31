/*
 * IotPacket.cpp
 *
 *  Created on: 27 lip 2015
 *      Author: witek
 */

#include "IotPacket.h"

uint8_t IdSequence = 0;

PingPacket::PingPacket()
{
	id = IdSequence;
	protocol = ICMP;
	type = REGULAR;
	srcAddress = 0;
	dstAddress = 0;

	IdSequence ++;
};

AckPacket::AckPacket(GenericPacketData* packet)
{
	id = packet->id;
	protocol = packet->protocol;
	type = ACK;
	srcAddress = packet->dstAddress;
	dstAddress = packet->srcAddress;
};
