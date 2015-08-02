/*
 * IotPacket.cpp
 *
 *  Created on: 27 lip 2015
 *      Author: witek
 */

#include "IotPacket.h"

uint8_t IdSequence = 0;

uint8_t PacketHeader::getId() {
	return header[0];
}
IotProtocol PacketHeader::getProtocol() {
	return (IotProtocol)header[1];
}

IotPacketType PacketHeader::getType() {
	return (IotPacketType)header[2];
}

uint8_t PacketHeader::getSrcAddress() {
	return header[3];
}
uint8_t PacketHeader::getDstAddress() {
	return header[4];
}

void PacketHeader::setId(uint8_t id) {
	header[0] = id;
}
void PacketHeader::setProtocol(IotProtocol protocol) {
	header[1] = (uint8_t)protocol;
}

void PacketHeader::setType(IotPacketType type) {
	header[2] = (uint8_t)type;
}

void PacketHeader::setSrcAddress(uint8_t address) {
	header[3] = address;
}

void PacketHeader::setDstAddress(uint8_t address) {
	header[4] = address;
}


PingPacket::PingPacket()
{
	setId(IdSequence);
	setProtocol(ICMP);
	setType(REGULAR);
	setSrcAddress(0);
	setDstAddress(0);

	IdSequence ++;

	memset(payload, 0, 27);
};

AckPacket::AckPacket(GenericPacketData* packet)
{
	setId(packet->getId());
	setProtocol(packet->getProtocol());
	setType(ACK);
	setSrcAddress(packet->getDstAddress());
	setDstAddress(packet->getSrcAddress());
};
