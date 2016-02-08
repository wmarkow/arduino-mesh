/*
 * IotRadio.cpp
 *
 *  Created on: 25 lip 2015
 *      Author: witek
 */

#include "IotRadio.h"

#define PAYLOAD_SIZE 32

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

Counters* IotRadio::getCounters()
{
	return &counters;
}

void IotRadio::arduinoLoop()
{
	processIncomingPackets();
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
	transmitterState = WAITING_FOR_ACK;

	unsigned long startedWaitingAtMicros = micros();               // Set up a timeout period, get the current microseconds

	while (!hasAckArrived(packet)){
		if (micros() - startedWaitingAtMicros > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			Serial.println(F("Failed, response timed out."));

			transmitterState = IDLE;
			return false;
		}
	}

	transmitterState = IDLE;
	return true;
}

bool IotRadio::write(GenericPacketData* packet)
{
	radio.stopListening();

	bool result = radio.write(packet, PAYLOAD_SIZE);

	radio.startListening();

	updateSentCounters(packet);

	#if (IOT_DEBUG_WRITE_RADIO == ON)
		debugHexPrintToSerial(packet, PAYLOAD_SIZE);
	#endif

	return result;
}

bool IotRadio::hasAckArrived(GenericPacketData* sentPacket)
{
	processIncomingPackets();
	bool result = false;

	for (SimpleList<GenericPacketData>::iterator itr = preProcessedIncomingPackets.begin(); itr != preProcessedIncomingPackets.end();)
	{
		if(itr->getType() != ACK) {
			continue;
		}

		if(itr->getDstAddress() != sentPacket->getSrcAddress()) {
			continue;
		}

		if(itr->getSrcAddress() != sentPacket->getDstAddress()) {
			continue;
		}

		if(itr->getId() != sentPacket->getId()) {
			continue;
		}

		if(itr->getProtocol() != sentPacket->getProtocol()) {
			continue;
		}

		itr = preProcessedIncomingPackets.erase(itr);

		result = true;
	}

	return result;
}

void IotRadio::processIncomingPackets()
{
	readIncomingPacket();

	for (SimpleList<GenericPacketData>::iterator itr = preProcessedIncomingPackets.begin(); itr != preProcessedIncomingPackets.end();)
	{
		if(itr->getDstAddress() != ipAddress)
		{
			// this packet is not addressed for me; purge that packet
			// TODO: implement flooding retransmissions here
			itr = preProcessedIncomingPackets.erase(itr);
			continue;
		}

		if(itr->getType() == ACK && transmitterState != WAITING_FOR_ACK)
		{
			// the transmitter is not waiting for ACK; purge that ACK
			itr = preProcessedIncomingPackets.erase(itr);
			continue;
		}

		if(itr->getProtocol() == ICMP && itr->getType() == REGULAR)
		{
			AckPacket ackPacket(itr);
			write(&ackPacket);

			// ACK sent. Purge incoming ICMP packet
			itr = preProcessedIncomingPackets.erase(itr);
			continue;
		}

/*
		if(itr->getProtocol() == TCP && itr->getType() == REGULAR)
		{
			// ACK sent; move incoming packet to incoming queue
			if(incomingPackets.size() >= APPLICATION_LAYER_INCOMING_PACKETS_NUMBER)
			{
				//incoming buffer is full; purge that packet
				itr = preProcessedIncomingPackets.erase(itr);
				continue;
			}

			// send ack back
			AckPacket ackPacket(itr);
			write(&ackPacket);
		}
*/
	    ++itr;
	}
}

bool IotRadio::readIncomingPacket()
{
	if( !radio.available()) {
		return false;
	}

	GenericPacketData incomingPacket;
	radio.read(&incomingPacket, PAYLOAD_SIZE);

	if(preProcessedIncomingPackets.size() >= NETWORK_LAYER_INCOMING_PACKETS_NUMBER)
	{
		counters.totalPacketIncomingDiscarded ++;
		// incoming queue is full, discard new packet
		#if IOT_DEBUG_WRITE_RADIO == ON
			Serial.println("Discarding packet because incoming buffer is full");
		#endif

			return false;
	}

	preProcessedIncomingPackets.push_back(incomingPacket);

	#if IOT_DEBUG_WRITE_RADIO == ON
		debugHexPrintToSerial(incomingPacket, PAYLOAD_SIZE);
	#endif

	updateReceivedCounters(&incomingPacket);

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

void IotRadio::updateSentCounters(GenericPacketData* packet)
{
	counters.totalPacketsSent ++;


	if(packet->getSrcAddress() == this->ipAddress )
	{
		counters.totalPacketsLocalSent ++;

		if(packet->getProtocol() != UDP )
		{
			counters.totalPacketsLocalTcpSent ++;
			if(packet->getType() == REGULAR)
			{
				counters.totalPacketsLocalTcpRegularSent ++;
			}

			if(packet->getType() == ACK)
			{
				counters.totalPacketsLocalTcpAckSent ++;
			}
		}
	}
}

void IotRadio::updateReceivedCounters(GenericPacketData* packet)
{
	counters.totalPacketsReceived ++;


	if(packet->getDstAddress() == this->ipAddress )
	{
		counters.totalPacketsLocalReceived ++;

		if(packet->getProtocol() == TCP)
		{
			counters.totalPacketsLocalTcpReceived ++;
			if(packet->getType() == REGULAR)
			{
				counters.totalPacketsLocalTcpRegularReceived ++;
			}

			if(packet->getType() == ACK)
			{
				counters.totalPacketsLocalTcpAckReceived ++;
			}
		}
	}
}
