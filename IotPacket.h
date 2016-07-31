/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include <stdint.h>

enum IotProtocol
{
	ICMP = (uint8_t)0x01,
	TCP = (uint8_t)0x06,
	UDP = (uint8_t)0x11
};

enum IotPacketType
{
	REGULAR = (uint8_t)0x00,
	ACK = (uint8_t)0x01
};

class PacketHeader
{
public:
	uint8_t id;
	IotProtocol protocol;
	IotPacketType type;
	uint8_t srcAddress;
	uint8_t dstAddress;
};

class GenericPacketData : public PacketHeader
{
public:
	uint8_t payload[27];
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
