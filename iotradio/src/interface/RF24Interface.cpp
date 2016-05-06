/*
 * RF24Interface.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#include "RF24Interface.h"


RF24Interface::RF24Interface() : rf24(RF24(IOT_HARDWARE_CE_PIN, IOT_HARDWARE_CS_PIN)),
receiver(RF24Receiver(&rf24)),
transmitter(RF24Transmitter(&rf24))
{
	linkAddress[0] = 0xc1;
	linkAddress[1] = 0xc1;
	linkAddress[2] = 0xc1;
	linkAddress[3] = 0xc1;
	linkAddress[4] = 0xc1;

	ipAddress = 1;
	flooder= NULL;
}

void RF24Interface::setFlooder(Flooder *flooder)
{
	this->flooder = flooder;
}

bool RF24Interface::up()
{
	if(!rf24.begin())
	{
		return false;
	}

	rf24.openWritingPipe(linkAddress);
	rf24.openReadingPipe(1, linkAddress);
	rf24.setPALevel(RF24_PA_MIN);
	rf24.setDataRate(RF24_1MBPS);
	rf24.setAutoAck(false);
	rf24.setPayloadSize(DEFAULT_PACKET_SIZE);
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
	pingResult.packetSize = DEFAULT_PACKET_SIZE;
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

bool RF24Interface::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
	if(length > DEFAULT_PACKET_PAYLOAD_SIZE)
	{
		return false;
	}


}

void RF24Interface::loop()
{
	transmitter.loop();
	processIncomingPackets();
	receiver.loop();
}

PacketCounters* RF24Interface::getCounters()
{
	return &packetCounters;
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
		return F("nRF24L01+");
	}

	return F("nRF24L01");
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
		return sendTcpPacket(packet);
	}

	return sendUdpPacket(packet);
}


bool RF24Interface::sendTcpPacket(GenericPacketData* packet)
{
	transmitter.addPacketToTransmissionQueue(packet);
	tcpTransmitionState = WAITING_FOR_ACK;

	unsigned long startedWaitingAtMicros = micros();               // Set up a timeout period, get the current microseconds

	while (!hasAckArrived(packet)){
		if (micros() - startedWaitingAtMicros > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			Serial.println(F("Failed, response timed out."));

			tcpTransmitionState = IDLE;
			packetCounters.incTransmittedTcpFailed();

			return false;
		}
	}

	tcpTransmitionState = IDLE;
	packetCounters.incTransmittedTcpSuccess();

	return true;
}

bool RF24Interface::sendUdpPacket(GenericPacketData* packet)
{
	if(packet->getType() == ACK)
	{
		packetCounters.incTransmittedUdpAck();
	} else
	{
		packetCounters.incTransmittedUdpOther();
	}

	return transmitter.addPacketToTransmissionQueue(packet);
}

bool RF24Interface::hasAckArrived(GenericPacketData* sentPacket)
{
	loop();

	for (uint8_t index = 0 ; index < receiver.getIncomingPackets()->getSize(); index ++)
	{
		GenericPacketData* itr = receiver.getIncomingPackets()->get(index);

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

		receiver.getIncomingPackets()->remove(index);

		return true;
	}

	return false;
}

void RF24Interface::processIncomingPackets()
{
	for (uint8_t index = 0 ; index < receiver.getIncomingPackets()->getSize() ; index ++)
	{
		GenericPacketData* itr = receiver.getIncomingPackets()->get(index);

		if(itr->getDstAddress() != ipAddress)
		{
			// this packet is not addressed for me; flood that packet
			flooder->flood(itr);
			receiver.getIncomingPackets()->remove(index);
			index --;
			continue;
		}

		if(itr->getType() == ACK && tcpTransmitionState != WAITING_FOR_ACK)
		{
			// the transmitter is not waiting for ACK; purge that ACK
			receiver.getIncomingPackets()->remove(index);
			index --;
			continue;
		}

		if(itr->getProtocol() == ICMP && itr->getType() == REGULAR)
		{
			AckPacket ackPacket(itr);
			transmitter.addPacketToTransmissionQueue(&ackPacket);
			this->packetCounters.incTransmittedUdpAck();

			// ACK sent. Purge incoming ICMP packet
			receiver.getIncomingPackets()->remove(index);
			index --;
			continue;
		}
		receiver.getIncomingPackets()->remove(index);
		index --;

/*      // Some other packet addressed to me
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
	}
}

bool RF24Interface::floodToTransmitter(GenericPacketData* packet)
{
	return transmitter.addPacketToTransmissionQueue(packet);
}
