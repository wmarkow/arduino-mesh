/*
 * TcpPacket.h
 *
 *  Created on: 9 maj 2016
 *      Author: witek
 */

#ifndef NETWORK_PACKET_TCPPACKET_H_
#define NETWORK_PACKET_TCPPACKET_H_

#include "../../../mesh/network/packet/core/IotPacket.h"

class TcpPacket : public IotPacket
{
public:
	TcpPacket(uint8_t* data, uint8_t length);
};

#endif /* NETWORK_PACKET_TCPPACKET_H_ */
