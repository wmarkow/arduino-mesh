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
	setId(IdSequence);
	setProtocol(ICMP);
	setType(REGULAR);
	setTTL(DEFAULT_TTL);
	setSrcAddress(0);
	setDstAddress(0);

	IdSequence ++;

	memset(payload, 0, DEFAULT_PACKET_PAYLOAD_SIZE);
};

AckPacket::AckPacket(GenericPacketData* packet)
{
	setId(packet->getId());
	setProtocol(packet->getProtocol());
	setTTL(DEFAULT_TTL);
	setType(ACK);
	setSrcAddress(packet->getDstAddress());
	setDstAddress(packet->getSrcAddress());
};
