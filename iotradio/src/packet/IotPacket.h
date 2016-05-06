/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include <string.h>

#include "IotProtocol.h"
#include "IotPacketType.h"
#include "IotPacketHeader.h"

class GenericPacketData : public IotPacketHeader
{
public:
	uint8_t payload[DEFAULT_PACKET_PAYLOAD_SIZE];
};

class PingPacket : public GenericPacketData
{
public:
	PingPacket();
};

class AckPacket : public GenericPacketData
{
public:
	AckPacket(GenericPacketData* packet);
};

#endif /* IOTPACKET_H_ */
