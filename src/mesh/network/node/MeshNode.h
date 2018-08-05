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
   Interface *rf24interface;
   Interface *hc12interface;

   void processIncomingPackets(Interface* interface);
   void flood(IotPacket* packet);
public:
   MeshNode();
   uint8_t getIpAddress();
   void setIpAddress(uint8_t ipAddress);
   void setRF24Interface(Interface *interface);
   void setHC12Interface(Interface *interface);
   MeshNodeCounters* getCounters();
   PingResult ping(uint8_t dst);
   void loop();
   void setWiresharkEnabled(bool enabled);
};

extern MeshNode LocalMeshNode;

#endif /* MESH_NETWORK_NODE_MESHNODE_H_ */
