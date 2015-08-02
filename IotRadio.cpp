/*
 * IotRadio.cpp
 *
 *  Created on: 25 lip 2015
 *      Author: witek
 */

#include "IotRadio.h"

#define PAYLOAD_SIZE 32

GenericPacketData lastIncomingPacket;

IotRadio::IotRadio() : radio(RF24(IOT_HARDWARE_CE_PIN, IOT_HARDWARE_CS_PIN)) {
	linkAddress[0] = 0xc1;
	linkAddress[1] = 0xc1;
	linkAddress[2] = 0xc1;
	linkAddress[3] = 0xc1;
	linkAddress[4] = 0xc1;

	ipAddress = 0;
}

bool IotRadio::begin()
{
	if(radio.begin()){
		Serial.println(F("Chip connected"));
	}else{
		Serial.println("Chip looks disconnected");
	}

	radio.openWritingPipe(linkAddress);
	radio.openReadingPipe(1, linkAddress);
	radio.setPALevel(RF24_PA_MAX);
	radio.setAutoAck(false);
	radio.setPayloadSize(PAYLOAD_SIZE);
	radio.openWritingPipe(linkAddress);
	radio.openReadingPipe(1, linkAddress);
	radio.startListening();

	return true;
}

void IotRadio::setIpAddress(uint8_t address)
{
	ipAddress = address;
}

bool IotRadio::ping(uint8_t dstAddress)
{
	PingPacket pingPacket;

	unsigned long sentTime = micros();
	if (sendPacket(&pingPacket, dstAddress)) {
		unsigned long gotTime = micros();

		Serial.print(PAYLOAD_SIZE);
		Serial.print(F(" bytes from '"));
		Serial.print(dstAddress);
		Serial.print(F("': time= "));
		Serial.print(gotTime - sentTime);
		Serial.println(F(" us"));

		return true;
	}

	Serial.print(F("Destination host '"));
	Serial.print(dstAddress);
	Serial.println(F("' is unreachable."));

	return false;
}

bool IotRadio::sendPacket(GenericPacketData* packet, uint8_t dstAddress)
{
	packet->setSrcAddress(ipAddress);
	packet->setDstAddress(dstAddress);

	return sendPacket(packet);
}

bool IotRadio::sendPacket(GenericPacketData* packet)
{
	if(packet->getProtocol() == ICMP || packet->getProtocol() == TCP)
	{
		return sendPacketWaitForAck(packet);
	}

	return write(packet);
}

bool IotRadio::sendPacketWaitForAck(GenericPacketData* packet)
{
	write(packet);

	unsigned long startedWaitingAtMicros = micros();               // Set up a timeout period, get the current microseconds

	while (!hasAckArrived(packet)){
		if (micros() - startedWaitingAtMicros > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			Serial.println(F("Failed, response timed out."));

			return false;
		}
	}

	return true;
}

bool IotRadio::write(GenericPacketData* packet)
{
	radio.stopListening();

	bool result = radio.write(packet, PAYLOAD_SIZE);

	debugHexPrintToSerial(packet, PAYLOAD_SIZE);

	radio.startListening();

	return result;
}

bool IotRadio::hasAckArrived(GenericPacketData* sentPacket)
{
	processIncomingPackets();

	if(lastIncomingPacket.getType() != ACK) {
		return false;
	}

	if(lastIncomingPacket.getDstAddress() != sentPacket->getSrcAddress()) {
		return false;
	}

	if(lastIncomingPacket.getSrcAddress() != sentPacket->getDstAddress()) {
		return false;
	}

	if(lastIncomingPacket.getId() != sentPacket->getId()) {
		return false;
	}

	if(lastIncomingPacket.getProtocol() != sentPacket->getProtocol()) {
		return false;
	}

	// clear the incoming ACK packet
	memset(&lastIncomingPacket, 0, PAYLOAD_SIZE);

	return true;
}

void IotRadio::processIncomingPackets()
{
	if(!readIncomingPacket(&lastIncomingPacket)) {
		return;
	}

	if((lastIncomingPacket.getProtocol() == ICMP || lastIncomingPacket.getProtocol() == TCP) && lastIncomingPacket.getType() == REGULAR) {
		// send ack back
		AckPacket ackPacket(&lastIncomingPacket);

		if(write(&ackPacket)){
			// ack sent
		}
	}
}

bool IotRadio::readIncomingPacket(GenericPacketData* incomingPacket)
{
	if( !radio.available()) {
		return false;
	}

	radio.read(incomingPacket, PAYLOAD_SIZE);

	debugHexPrintToSerial(incomingPacket, PAYLOAD_SIZE);

	return true;
}

void IotRadio::debugHexPrintToSerial(void* object, uint8_t length) {
	uint8_t* wsk = reinterpret_cast<uint8_t*>(object);
	for(uint8_t q = 0 ;q < PAYLOAD_SIZE ; q ++){
		Serial.print(*wsk++, HEX);
		Serial.print(" ");
	}
	Serial.println();
}
