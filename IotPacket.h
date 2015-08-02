/*
 * IotPacket.h
 *
 *  Created on: 26 lip 2015
 *      Author: witek
 */

#ifndef IOTPACKET_H_
#define IOTPACKET_H_

#include <stdint.h>
#include <string.h>

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
protected:
	uint8_t header[5];
public:
	uint8_t getId();
	IotProtocol getProtocol();
	IotPacketType getType();
	uint8_t getSrcAddress();
	uint8_t getDstAddress();

	void setId(uint8_t id);
	void setProtocol(IotProtocol protocol);
	void setType(IotPacketType type);
	void setSrcAddress(uint8_t address);
	void setDstAddress(uint8_t address);
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
