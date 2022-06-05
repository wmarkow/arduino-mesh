/*
 * MeshNode.cpp
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#include "../node/MeshNode.h"

MeshNode::MeshNode()
{
    ipAddress = 1;
    interface = NULL;
    incomingPacket = NULL;
}

/** Network support */
uint8_t MeshNode::getIpAddress()
{
    return ipAddress;
}

void MeshNode::setIpAddress(uint8_t ipAddress)
{
    this->ipAddress = ipAddress;

    if (interface == NULL)
    {
        return;
    }

    interface->setIpAddress(ipAddress);
}

PingResult MeshNode::ping(uint8_t dstAddress)
{
    if (interface == NULL)
    {
        PingResult pingResult;
        pingResult.success = false;

        return pingResult;    
    }

    return interface->ping(dstAddress);
}

bool MeshNode::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    if (interface == NULL)
    {
        return false;
    }

    return interface->sendTcp(dstAddress, data, length);
}

void MeshNode::sendUdp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    if (interface == NULL)
    {
        return;
    }

    interface->sendUdp(dstAddress, data, length);
}

IotPacket* MeshNode::getIncomingPacket()
{
    return this->incomingPacket;
}

void MeshNode::markIncomingPacketConsumed()
{
    this->incomingPacket = NULL;
}

void MeshNode::processIncomingPackets(Interface* interface)
{
    if (interface == NULL)
    {
        return;
    }

    for (uint8_t index = 0; index < interface->getIncomingPackets()->getSize();
            index++)
    {
        IotPacket* packet = interface->getIncomingPackets()->peek(index);

        if (packet->getDstAddress() != getIpAddress())
        {
            // this packet is not addressed for me; flood that packet
            flood(packet);
            interface->getIncomingPackets()->remove(index);
            index--;
            continue;
        }

        // here we have some other packet addressed to me. Copy it - if possible - to incoming buffer.
        if (this->incomingPacket == NULL)
        {
            memcpy(&this->incomingPacketBuffer,
                    interface->getIncomingPackets()->peek(index),
                    sizeof(IotPacket));
            this->incomingPacket = &incomingPacketBuffer;

            interface->getIncomingPackets()->remove(index);
            index--;
        }
    }
}

bool MeshNode::flood(IotPacket* packet)
{
    if (packet->getSrcAddress() == getIpAddress())
    {
        // I'm originator of this packet; drop it
        counters.incDroppedCount();

        return false;
    }

    if (packet->decrementTTL() == 0)
    {
        // TTL reached 0; drop packet
        counters.incDroppedCount();

        return false;
    }

    // flood packet
    if (interface == NULL)
    {
        return true;
    }

    interface->floodPacket(packet);

    counters.incFloodedCount();

    return true;
}

/** Interfaces support */
uint8_t MeshNode::getInterfaceCount()
{
    return 1;
}

Interface* MeshNode::getInterface(uint8_t index)
{
    return interface;
}
void MeshNode::setInterface(uint8_t index, Interface* interfacePtr)
{
    this->interface = interfacePtr;
}

/** Various features */
void MeshNode::loop()
{
    if (interface == NULL)
    {
        return;
    }

    processIncomingPackets(interface);
    interface->loop();
}

MeshNodeCounters* MeshNode::getCounters()
{
    return &counters;
}

void MeshNode::setWiresharkEnabled(bool enabled)
{
    if (interface == NULL)
    {
        return;
    }

    interface->setWiresharkEnabled(enabled);
}


