/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include "../../../../mesh/network/packet/core/IotPacketHeader.h"
#include "../../../../mesh/network/packet/core/IotPacketType.h"
#include "../../../../mesh/network/packet/core/IotProtocol.h"

class IotPacket : public IotPacketHeader
{
public:
	IotPacket();
	uint8_t payload[DEFAULT_PACKET_PAYLOAD_SIZE];
};

#endif /* IOTPACKET_H_ */
