/*
 * RF24Interface.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#include "RF24Interface.h"


RF24Interface::RF24Interface() : rf24(RF24(IOT_HARDWARE_CE_PIN, IOT_HARDWARE_CS_PIN))
{
	linkAddress[0] = 0xc1;
	linkAddress[1] = 0xc1;
	linkAddress[2] = 0xc1;
	linkAddress[3] = 0xc1;
	linkAddress[4] = 0xc1;

	ipAddress = 0;
}

bool RF24Interface::up()
{
	if(!rf24.begin())
	{
		return false;
	}

	rf24.openWritingPipe(linkAddress);
	rf24.openReadingPipe(1, linkAddress);
	rf24.setPALevel(RF24_PA_MAX);
	rf24.setAutoAck(false);
	rf24.setPayloadSize(PAYLOAD_SIZE);
	rf24.openWritingPipe(linkAddress);
	rf24.openReadingPipe(1, linkAddress);
	rf24.startListening();

	return isChipConnected();
}

bool RF24Interface::isUp()
{
	return isChipConnected();
}

bool RF24Interface::isChipConnected()
{
	return rf24.isChipConnected();
}

void RF24Interface::setIpAddress(uint8_t address)
{
	ipAddress = address;
}

PingResult RF24Interface::ping(uint8_t dstAddress)
{
	PingResult pingResult;
	pingResult.packetSize = PAYLOAD_SIZE;
	pingResult.success = false;
	pingResult.timeInUs = 0;

	PingPacket pingPacket;

	unsigned long sentTime = micros();
	if (sendPacket(&pingPacket, dstAddress)) {
		unsigned long gotTime = micros();
		pingResult.timeInUs = gotTime - sentTime;
		pingResult.success = true;
	}

	return pingResult;
}

void RF24Interface::loop()
{
	processIncomingPackets();
}

uint8_t RF24Interface::getIpAddress()
{
	return ipAddress;
}

String RF24Interface::getLinkAddress()
{
	String result;

	result.concat(linkAddress[0]);
	result.concat(':');
	result.concat(linkAddress[1]);
	result.concat(':');
	result.concat(linkAddress[2]);
	result.concat(':');
	result.concat(linkAddress[3]);
	result.concat(':');
	result.concat(linkAddress[4]);

	return result;
}

uint8_t RF24Interface::getPALevel()
{
	return rf24.getPALevel();
}

uint8_t RF24Interface::getDataRate()
{
	return rf24.getDataRate();
}

uint8_t RF24Interface::getPayloadSize()
{
	return rf24.getPayloadSize();
}

uint8_t RF24Interface::getRFChannel()
{
	return rf24.getChannel();
}

String RF24Interface::getModel()
{
	if(rf24.isPVariant())
	{
		return "nRF24L01+";
	}

	return "nRF24L01";
}

bool RF24Interface::sendPacket(GenericPacketData* packet, uint8_t dstAddress)
{
	packet->setSrcAddress(ipAddress);
	packet->setDstAddress(dstAddress);

	return sendPacket(packet);
}

bool RF24Interface::sendPacket(GenericPacketData* packet)
{
	if(packet->getProtocol() == ICMP || packet->getProtocol() == TCP)
	{
		return sendPacketWaitForAck(packet);
	}

	return write(packet);
}


bool RF24Interface::sendPacketWaitForAck(GenericPacketData* packet)
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

bool RF24Interface::write(GenericPacketData* packet)
{
	rf24.stopListening();

	bool result = rf24.write(packet, PAYLOAD_SIZE);

	rf24.startListening();

	return result;
}

bool RF24Interface::hasAckArrived(GenericPacketData* sentPacket)
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

void RF24Interface::processIncomingPackets()
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

bool RF24Interface::available()
{
	return rf24.available() && isChipConnected();
}

bool RF24Interface::readIncomingPacket()
{
	if( !available()) {
		return false;
	}

	GenericPacketData incomingPacket;
	rf24.read(&incomingPacket, PAYLOAD_SIZE);

	if(!isChipConnected())
	{
		return false;
	}

	if(preProcessedIncomingPackets.size() >= NETWORK_LAYER_INCOMING_PACKETS_NUMBER)
	{
		// incoming queue is full, discard new packet
		#if IOT_DEBUG_WRITE_RADIO == ON
			Serial.println("Discarding packet because incoming buffer is full");
		#endif

			return false;
	}

	preProcessedIncomingPackets.push_back(incomingPacket);

	return true;
}
void RF24Interface::debugHexPrintToSerial(void* object, uint8_t length) {
	uint8_t* wsk = reinterpret_cast<uint8_t*>(object);
	for(uint8_t q = 0 ;q < PAYLOAD_SIZE ; q ++){
		Serial.print(*wsk++, HEX);
		Serial.print(" ");
	}
	Serial.println();
}
