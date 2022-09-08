/*
 * TcpPacket.cpp
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#include "TcpPacket.h"

#include <string.h>

TcpPacket::TcpPacket(uint8_t* data, uint8_t length) : IotPacket()
{
	setProtocol(TCP);

	memcpy(payload, data, length);
};


