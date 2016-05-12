/*
 * AckPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "../../../mesh/network/packet/AckPacket.h"

AckPacket::AckPacket(IotPacket* packet)
{
	setId(packet->getId());
	setProtocol(packet->getProtocol());
	setTTL(DEFAULT_TTL);
	setType(ACK);
	setSrcAddress(packet->getDstAddress());
	setDstAddress(packet->getSrcAddress());
};
