/*
 * IotPacketHeader.h
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#ifndef PACKET_IOTPACKETHEADER_H_
#define PACKET_IOTPACKETHEADER_H_

#include "IotPacketType.h"
#include "IotProtocol.h"

#define DEFAULT_TTL 3
#define DEFAULT_PACKET_SIZE 32
#define DEFAULT_PACKET_HEADER_SIZE 6
#define DEFAULT_PACKET_PAYLOAD_SIZE DEFAULT_PACKET_SIZE - DEFAULT_PACKET_HEADER_SIZE


class IotPacketHeader
{
protected:
	uint8_t header[DEFAULT_PACKET_HEADER_SIZE];
public:
	IotPacketHeader();
	IotProtocol getProtocol();
	IotPacketType getType();
	uint8_t getTTL();
	uint8_t getSrcAddress();
	uint8_t getDstAddress();
	uint8_t getPayloadSize();

	void setProtocol(IotProtocol protocol);
	void setType(IotPacketType type);
	void setTTL(uint8_t ttl);
	uint8_t decrementTTL();
	void setSrcAddress(uint8_t address);
	void setDstAddress(uint8_t address);
	void setPayloadSize(uint8_t size);
};

#endif /* PACKET_IOTPACKETHEADER_H_ */
