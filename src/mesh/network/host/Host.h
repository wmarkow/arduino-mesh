/*
 * Host.h
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#ifndef MESH_NETWORK_HOST_HOST_H_
#define MESH_NETWORK_HOST_HOST_H_

#include <stdint.h>

#include "../flooder/MeshNodeCounters.h"
#include "../interface/Interface.h"

class Host
{
private:
   uint8_t ipAddress;
   MeshNodeCounters counters;
   Interface *rf24interface;
   Interface *hc12interface;

   void processIncomingPackets(Interface* interface);
   void flood(IotPacket* packet);
public:
   Host();
   uint8_t getIpAddress();
   void setIpAddress(uint8_t ipAddress);
   void setRF24Interface(Interface *interface);
   void setHC12Interface(Interface *interface);
   MeshNodeCounters* getCounters();
   PingResult ping(uint8_t dst);
   void loop();
};

extern Host Localhost;

#endif /* MESH_NETWORK_HOST_HOST_H_ */
