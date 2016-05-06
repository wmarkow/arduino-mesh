/*
 * AckPacket.h
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#ifndef PACKET_ACKPACKET_H_
#define PACKET_ACKPACKET_H_

#include "core/IotPacket.h"

class AckPacket : public IotPacket
{
public:
	AckPacket(IotPacket* packet);
};

#endif /* PACKET_ACKPACKET_H_ */
