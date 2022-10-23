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
#define DEFAULT_PACKET_HEADER_SIZE 4
#define DEFAULT_PACKET_PAYLOAD_SIZE (DEFAULT_PACKET_SIZE - DEFAULT_PACKET_HEADER_SIZE)

class IotPacketHeader
{
protected:

	typedef struct __attribute__((packed))
	{
		uint8_t srcAddress;
		uint8_t dstAddress;
		uint16_t protocol : 1; 
		uint16_t type :1;
		uint16_t port : 5;
		uint16_t ttl : 4;
		uint16_t payloadLength : 5;
	} Header;

	Header header;

public:
	IotPacketHeader();
	IotProtocol getProtocol();
	IotPacketType getType();
	uint8_t getTTL();
	uint8_t getSrcAddress();
	uint8_t getDstAddress();
	uint8_t getPayloadSize();
	uint8_t getPort();

	void setProtocol(IotProtocol protocol);
	void setType(IotPacketType type);
	void setTTL(uint8_t ttl);
	uint8_t decrementTTL();
	void setSrcAddress(uint8_t address);
	void setDstAddress(uint8_t address);
	void setPayloadSize(uint8_t size);
	void setPort(uint8_t port);
};

#endif /* PACKET_IOTPACKETHEADER_H_ */
