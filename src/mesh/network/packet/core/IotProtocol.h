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
	UDP = (uint8_t)0x00,
	TCP = (uint8_t)0x01,
};

#endif /* PACKET_IOTPROTOCOL_H_ */
