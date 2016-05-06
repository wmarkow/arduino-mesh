/*
 * PingPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include <string.h>
#include "PingPacket.h"

PingPacket::PingPacket() : IotPacket()
{
	setProtocol(ICMP);
	setType(REGULAR);

	memset(payload, 0, DEFAULT_PACKET_PAYLOAD_SIZE);
};
