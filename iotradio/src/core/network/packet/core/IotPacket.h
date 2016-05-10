/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include "IotProtocol.h"
#include "IotPacketType.h"
#include "IotPacketHeader.h"

class IotPacket : public IotPacketHeader
{
public:
	IotPacket();
	uint8_t payload[DEFAULT_PACKET_PAYLOAD_SIZE];
};

#endif /* IOTPACKET_H_ */
