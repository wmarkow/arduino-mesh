/*
 * Host.h
 *
 *  Created on: 15.07.2018
 *      Author: wmarkowski
 */

#ifndef MESH_NETWORK_HOST_HOST_H_
#define MESH_NETWORK_HOST_HOST_H_

#include <stdint.h>

class Host
{
private:
   uint8_t ipAddress;

public:
   Host();
   uint8_t getIpAddress();
   void setIpAddress(uint8_t ipAddress);
};


extern Host Localhost;

#endif /* MESH_NETWORK_HOST_HOST_H_ */
