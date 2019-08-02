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
    rf24interface = NULL;
    hc12interface = NULL;
    incomingPacket = NULL;
}

uint8_t MeshNode::getIpAddress()
{
    return ipAddress;
}

void MeshNode::setIpAddress(uint8_t ipAddress)
{
    this->ipAddress = ipAddress;

    if (rf24interface != NULL)
    {
        rf24interface->setIpAddress(ipAddress);
    }
    if (hc12interface != NULL)
    {
        hc12interface->setIpAddress(ipAddress);
    }
}

void MeshNode::setRF24Interface(Interface *interface)
{
    this->rf24interface = interface;
}

void MeshNode::setHC12Interface(Interface *interface)
{
    this->hc12interface = interface;
}

Interface* MeshNode::getRF24Interface()
{
    return this->rf24interface;
}

Interface* MeshNode::getHC12Interface()
{
    return this->hc12interface;
}

PingResult MeshNode::ping(uint8_t dstAddress)
{
    if (rf24interface != NULL)
    {
        PingResult pingResult = rf24interface->ping(dstAddress);
        if (pingResult.success)
        {
            return pingResult;
        }
    }

    if (hc12interface != NULL)
    {
        PingResult pingResult = hc12interface->ping(dstAddress);
        if (pingResult.success)
        {
            return pingResult;
        }
    }

    PingResult pingResult;
    pingResult.success = false;

    return pingResult;
}

bool MeshNode::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    if (rf24interface != NULL)
    {
        if (rf24interface->sendTcp(dstAddress, data, length))
        {
            return true;
        }
    }

    if (hc12interface != NULL)
    {
        if (hc12interface->sendTcp(dstAddress, data, length))
        {
            return true;
        }
    }

    return false;
}

void MeshNode::sendUdp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    if (rf24interface != NULL)
    {
        rf24interface->sendUdp(dstAddress, data, length);
    }

    if (hc12interface != NULL)
    {
        hc12interface->sendUdp(dstAddress, data, length);
    }
}

void MeshNode::loop()
{
    if (rf24interface != NULL)
    {
        processIncomingPackets(rf24interface);
        rf24interface->loop();
    }

    if (hc12interface != NULL)
    {
        processIncomingPackets(hc12interface);
        hc12interface->loop();
    }
}

MeshNodeCounters* MeshNode::getCounters()
{
    return &counters;
}

void MeshNode::setWiresharkEnabled(bool enabled)
{
    if (rf24interface != NULL)
    {
        rf24interface->setWiresharkEnabled(enabled);
    }

    if (hc12interface != NULL)
    {
        hc12interface->setWiresharkEnabled(enabled);
    }
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

void MeshNode::flood(IotPacket* packet)
{
    if (packet->getSrcAddress() == getIpAddress())
    {
        // I'm originator of this packet; drop it
        counters.incDroppedCount();

        return;
    }

    if (packet->decrementTTL() == 0)
    {
        // TTL reached 0; drop packet
        counters.incDroppedCount();

        return;
    }

    // flood packet
    if (rf24interface != NULL)
    {
        rf24interface->floodPacket(packet);
    }
    if (hc12interface != NULL)
    {
        hc12interface->floodPacket(packet);
    }

    counters.incFloodedCount();
}

MeshNode LocalMeshNode;
