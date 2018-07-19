/*
 * Host.h
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#ifndef MESH_NETWORK_HOST_HOST_H_
#define MESH_NETWORK_HOST_HOST_H_

#include <stdint.h>

#include "../flooder/Flooder.h"
#include "../interface/Interface.h"

class Host
{
private:
   uint8_t ipAddress;
   Flooder flooder;
   Interface *rf24interface;
   Interface *hc12interface;
public:
   Host();
   uint8_t getIpAddress();
   void setIpAddress(uint8_t ipAddress);
   void setRF24Interface(Interface *interface);
   void setHC12Interface(Interface *interface);
   Flooder* getFlooder();
   PingResult ping(uint8_t dst);
};

extern Host Localhost;

#endif /* MESH_NETWORK_HOST_HOST_H_ */
