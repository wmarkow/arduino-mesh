/*
 * RF24Receiver.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#include "RF24Receiver.h"


RF24Receiver::RF24Receiver(RF24* rf24)
{
	this->rf24 = rf24;
}

void RF24Receiver::loop()
{
	if(available())
	{
		readIncomingPacket();
	}
}

SimpleList<GenericPacketData>* RF24Receiver::getIncomingPackets()
{
	return &incomingPackets;
}

bool RF24Receiver::available()
{
	return rf24->available() && rf24->isChipConnected();
}

bool RF24Receiver::readIncomingPacket()
{
	if( !available()) {
		return false;
	}

	GenericPacketData incomingPacket;
	rf24->read(&incomingPacket, DEFAULT_PACKET_SIZE);

	if(!rf24->isChipConnected())
	{
		return false;
	}

	if(incomingPackets.size() >= INCOMMING_PACKETS_BUFFER_SIZE)
	{
		// incoming queue is full, discard new packet
		#if IOT_DEBUG_WRITE_RADIO == ON
			Serial.println("Discarding packet because incoming buffer is full");
		#endif

		return false;
	}

	incomingPackets.push_back(incomingPacket);

	return true;
}
/*
void RF24Interface::debugHexPrintToSerial(void* object, uint8_t length) {
	uint8_t* wsk = reinterpret_cast<uint8_t*>(object);
	for(uint8_t q = 0 ;q < PAYLOAD_SIZE ; q ++){
		Serial.print(*wsk++, HEX);
		Serial.print(" ");
	}
	Serial.println();
}*/
