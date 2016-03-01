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

#define DEFAULT_TTL 3
#define DEFAULT_PACKET_SIZE 32
#define DEFAULT_PACKET_HEADER_SIZE 6
#define DEFAULT_PACKET_PAYLOAD_SIZE DEFAULT_PACKET_SIZE - DEFAULT_PACKET_HEADER_SIZE

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
	uint8_t header[DEFAULT_PACKET_HEADER_SIZE];
public:
	uint8_t getId();
	IotProtocol getProtocol();
	IotPacketType getType();
	uint8_t getTTL();
	uint8_t getSrcAddress();
	uint8_t getDstAddress();

	void setId(uint8_t id);
	void setProtocol(IotProtocol protocol);
	void setType(IotPacketType type);
	void setTTL(uint8_t ttl);
	uint8_t decrementTTL();
	void setSrcAddress(uint8_t address);
	void setDstAddress(uint8_t address);
};

class GenericPacketData : public PacketHeader
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
