/*
 * UdpPacket.h
 *
 *  Created on: 2 sie 2019
 *      Author: wmarkowski
 */

#ifndef LIBRARIES_ARDUINO_MESH_SRC_MESH_NETWORK_PACKET_UDPPACKET_H_
#define LIBRARIES_ARDUINO_MESH_SRC_MESH_NETWORK_PACKET_UDPPACKET_H_

#include "../../../mesh/network/packet/core/IotPacket.h"

class UdpPacket: public IotPacket
{
public:
    UdpPacket(uint8_t* data, uint8_t length);
};

#endif /* LIBRARIES_ARDUINO_MESH_SRC_MESH_NETWORK_PACKET_UDPPACKET_H_ */
