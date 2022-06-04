/*
 * Receiver.cpp
 *
 *  Created on: 26.07.2018
 *      Author: wmarkowski
 */

#include "../../../mesh/network/interface/Interface.h"

bool Interface::readIncomingPacket()
{
    IotPacket incomingPacket;
    if (device->readPacket(&incomingPacket) == false)
    {
        return false;
    }

    if (!device->isChipConnected())
    {
        return false;
    }

    if (incomingPackets.getSize() >= INCOMMING_PACKETS_BUFFER_SIZE)
    {
        // incoming queue is full, discard new packet
#if IOT_DEBUG_WRITE_RADIO == ON
        Serial.println(F("Discarding packet because incoming buffer is full"));
#endif

        return false;
    }

    return processIncomingPacket(&incomingPacket);
}

bool Interface::processIncomingPacket(IotPacket* packetPtr)
{
    wiresharkPacket(packetPtr, true);

    // Special packet: ACK addressed to me
    if (packetPtr->getType() == ACK
            && packetPtr->getDstAddress() == ipAddress)
    {
        if (tcpPacketWaitingForAck != NULL)
        {
            if (((AckPacket*) packetPtr)->doesAckMatchToPacket(
                    tcpPacketWaitingForAck))
            {
                ackReceived = true;
            }
        }

        return false;
    }

    // Special packet: ping (ICMP) addressed to me
    if (packetPtr->getProtocol() == ICMP
            && packetPtr->getType() == REGULAR
            && packetPtr->getDstAddress() == ipAddress)
    {
        AckPacket ackPacket(packetPtr);
        outgoingPackets.add(&ackPacket);
        counters.incTransmittedUdpAck();

        // ACK sent. Purge incoming ICMP packet
        return false;
    }

    // TCP packet addressed to me
    if (packetPtr->getProtocol() == TCP
            && packetPtr->getType() == REGULAR
            && packetPtr->getDstAddress() == ipAddress)
    {
        // sent ACK back
        AckPacket ackPacket(packetPtr);
        outgoingPackets.add(&ackPacket);
        counters.incTransmittedUdpAck();
    }

    incomingPackets.add(packetPtr);

    return true;
}