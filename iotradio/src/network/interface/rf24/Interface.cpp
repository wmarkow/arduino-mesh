/*
 * RF24Interface.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: witek
 */

#include "Interface.h"

#include "../../packet/AckPacket.h"
#include "../../packet/PingPacket.h"
#include "../../packet/TcpPacket.h"

Interface::Interface(Device *device) : transmitter(Transmitter(device)), receiver(Receiver(device))
{
	this->device = device;
	ipAddress = 1;
	flooder= NULL;
}

Device* Interface::getDevice()
{
	return device;
}

void Interface::setFlooder(Flooder *flooder)
{
	this->flooder = flooder;
}

bool Interface::up()
{
	return device->up();
}

bool Interface::isUp()
{
	return isChipConnected();
}

bool Interface::isChipConnected()
{
	return device->isChipConnected();
}

void Interface::setIpAddress(uint8_t address)
{
	ipAddress = address;
}

PingResult Interface::ping(uint8_t dstAddress)
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

bool Interface::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
	if(length > DEFAULT_PACKET_PAYLOAD_SIZE)
	{
		return false;
	}

	TcpPacket packet = TcpPacket(data, length);

	return sendPacket(&packet, dstAddress);
}

void Interface::loop()
{
	transmitter.loop();
	processIncomingPackets();
	receiver.loop();
}

PacketCounters* Interface::getCounters()
{
	return &packetCounters;
}

uint8_t Interface::getIpAddress()
{
	return ipAddress;
}

bool Interface::sendPacket(IotPacket* packet, uint8_t dstAddress)
{
	packet->setSrcAddress(ipAddress);
	packet->setDstAddress(dstAddress);

	return sendPacket(packet);
}


bool Interface::sendPacket(IotPacket* packet)
{
	if(packet->getProtocol() == ICMP || packet->getProtocol() == TCP)
	{
		return sendTcpPacket(packet);
	}

	return sendUdpPacket(packet);
}


bool Interface::sendTcpPacket(IotPacket* packet)
{
	transmitter.addPacketToTransmissionQueue(packet);
	tcpTransmitionState = WAITING_FOR_ACK;

	unsigned long startedWaitingAtMicros = micros();

	while (!hasAckArrived(packet)){
		if (micros() - startedWaitingAtMicros > 200000 ) {
			tcpTransmitionState = IDLE;
			packetCounters.incTransmittedTcpFailed();

			return false;
		}
	}

	tcpTransmitionState = IDLE;
	packetCounters.incTransmittedTcpSuccess();

	return true;
}

bool Interface::sendUdpPacket(IotPacket* packet)
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

bool Interface::hasAckArrived(IotPacket* sentPacket)
{
	loop();

	for (uint8_t index = 0 ; index < receiver.getIncomingPackets()->getSize(); index ++)
	{
		IotPacket* itr = receiver.getIncomingPackets()->get(index);

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

void Interface::processIncomingPackets()
{
	for (uint8_t index = 0 ; index < receiver.getIncomingPackets()->getSize() ; index ++)
	{
		IotPacket* itr = receiver.getIncomingPackets()->get(index);

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

bool Interface::floodToTransmitter(IotPacket* packet)
{
	return transmitter.addPacketToTransmissionQueue(packet);
}
