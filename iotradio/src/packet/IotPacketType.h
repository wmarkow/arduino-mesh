/*
 * IotPacketType.h
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#ifndef PACKET_IOTPACKETTYPE_H_
#define PACKET_IOTPACKETTYPE_H_

#include <stdint.h>

enum IotPacketType
{
	REGULAR = (uint8_t)0x00,
	ACK = (uint8_t)0x01
};

#endif /* PACKET_IOTPACKETTYPE_H_ */
