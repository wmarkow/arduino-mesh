/*
 * RF24Receiver.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: witek
 */

#include "RF24Receiver.h"

IotPacket incomingPacketsTable[INCOMMING_PACKETS_BUFFER_SIZE];

RF24Receiver::RF24Receiver(RF24* rf24) : incomingPackets(StaticList<IotPacket>(incomingPacketsTable, INCOMMING_PACKETS_BUFFER_SIZE))
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

StaticList<IotPacket>* RF24Receiver::getIncomingPackets()
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

	IotPacket incomingPacket;
	rf24->read(&incomingPacket, DEFAULT_PACKET_SIZE);

	if(!rf24->isChipConnected())
	{
		return false;
	}

	if(incomingPackets.getSize() >= INCOMMING_PACKETS_BUFFER_SIZE)
	{
		// incoming queue is full, discard new packet
		#if IOT_DEBUG_WRITE_RADIO == ON
			Serial.println(F("Discarding packet because incoming buffer is full"));
		#endif

		return false;
	}

	incomingPackets.add(&incomingPacket);

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
