#include "ComplexMeshNode.h"

ComplexMeshNode::ComplexMeshNode() : MeshNode()
{
    interface1 = NULL;
}

/** Network support */
void ComplexMeshNode::setIpAddress(uint8_t ipAddress)
{
    MeshNode::setIpAddress(ipAddress);

    if (interface1 != NULL)
    {
        interface1->setIpAddress(ipAddress);
    }
}

PingResult ComplexMeshNode::ping(uint8_t dstAddress)
{
    PingResult pingResult = MeshNode::ping(dstAddress);
    if (pingResult.success)
    {
        return pingResult;
    }

    if (interface1 == NULL)
    {
        pingResult.success = false;
        return pingResult;
    }

    return interface1->ping(dstAddress);
}

bool ComplexMeshNode::sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    if (MeshNode::sendTcp(dstAddress, data, length))
    {
        return true;
    }

    if (interface1 == NULL)
    {
        return false;
    }

    return interface1->sendTcp(dstAddress, data, length);
}

void ComplexMeshNode::sendUdp(uint8_t dstAddress, uint8_t* data, uint8_t length)
{
    MeshNode::sendUdp(dstAddress, data, length);

    if (interface1 == NULL)
    {
        return;
    }

    interface1->sendUdp(dstAddress, data, length);
}

/** Interfaces support */
uint8_t ComplexMeshNode::getInterfaceCount()
{
    return 2;
}

Interface* ComplexMeshNode::getInterface(uint8_t index)
{
    if(index == 0)
    {
        return MeshNode::getInterface(0);
    }

    return interface1;
}

void ComplexMeshNode::setInterface(uint8_t index, Interface* interfacePtr)
{
    if(index == 0)
    {
        MeshNode::setInterface(index, interfacePtr);
        return;
    }

    this->interface1 = interfacePtr;
}

/** Various features */
void ComplexMeshNode::loop()
{
    MeshNode::loop();

    if (interface1 == NULL)
    {
        return;
    }

    processIncomingPackets(interface1);
    interface1->loop();
}

void ComplexMeshNode::setWiresharkEnabled(bool enabled)
{
    MeshNode::setWiresharkEnabled(enabled);

    if (interface1 == NULL)
    {
        return;
    }
    interface1->setWiresharkEnabled(enabled);
}

bool ComplexMeshNode::flood(IotPacket* packet)
{
    if(MeshNode::flood(packet) == false)
    {
        return false;
    }

    // flood packet
    if (interface1 == NULL)
    {
        return true;
    }

    interface1->floodPacket(packet);

    return true;
}