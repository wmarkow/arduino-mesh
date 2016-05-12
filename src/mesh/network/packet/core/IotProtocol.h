/*
 * IotProtocol.h
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#ifndef PACKET_IOTPROTOCOL_H_
#define PACKET_IOTPROTOCOL_H_

#include <stdint.h>

enum IotProtocol
{
	ICMP = (uint8_t)0x01,
	TCP = (uint8_t)0x06,
	UDP = (uint8_t)0x11
};

#endif /* PACKET_IOTPROTOCOL_H_ */
