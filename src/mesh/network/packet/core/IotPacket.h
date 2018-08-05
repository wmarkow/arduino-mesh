/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include "IotPacketHeader.h"
#include "IotPacketType.h"
#include "IotProtocol.h"

class IotPacket : public IotPacketHeader
{
private:
   static uint8_t idSequence;
public:
	IotPacket();
	uint8_t payload[DEFAULT_PACKET_PAYLOAD_SIZE];

	uint8_t getPacketSize();
	static uint8_t generateNextId();
};

#endif /* IOTPACKET_H_ */
