/*
 * MeshNode.h
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#ifndef MESH_NETWORK_NODE_MESHNODE_H_
#define MESH_NETWORK_NODE_MESHNODE_H_

#include <stdint.h>

#include "../node/MeshNodeCounters.h"
#include "../interface/Interface.h"

class MeshNode
{
private:
    uint8_t ipAddress;
    MeshNodeCounters counters;
    Interface *interface;
    IotPacket incomingPacketBuffer;
    IotPacket* incomingPacket;

protected:
    /** Network support */
    void processIncomingPackets(Interface* interface);
    virtual bool flood(IotPacket* packet);

public:
    MeshNode();

    /** Network support */
    uint8_t getIpAddress();
    virtual void setIpAddress(uint8_t ipAddress);
    virtual PingResult ping(uint8_t dst);
    virtual bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
    virtual void sendUdp(uint8_t dstAddress, uint8_t* data, uint8_t length);
    IotPacket* getIncomingPacket();
    void markIncomingPacketConsumed();

    /** Interfaces support */
    virtual uint8_t getInterfaceCount();
    virtual Interface* getInterface(uint8_t index);
    virtual void setInterface(uint8_t index, Interface* interfacePtr);

    /** Various features */
    MeshNodeCounters* getCounters();
    virtual void setWiresharkEnabled(bool enabled);
    virtual void loop();
};

#endif /* MESH_NETWORK_NODE_MESHNODE_H_ */
