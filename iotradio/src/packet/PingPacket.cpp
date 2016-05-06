/*
 * PingPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include <string.h>
#include "PingPacket.h"

uint8_t IdSequence = 0;

PingPacket::PingPacket()
{
	setId(IdSequence);
	setProtocol(ICMP);
	setType(REGULAR);
	setTTL(DEFAULT_TTL);
	setSrcAddress(0);
	setDstAddress(0);

	IdSequence ++;

	memset(payload, 0, DEFAULT_PACKET_PAYLOAD_SIZE);
};
