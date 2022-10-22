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
   bool doesAckMatchToPacket(IotPacket* tcpPacket);

protected:
   inline void setAckedPacketId(uint8_t id);
   inline uint8_t getAckedPacketId();
};

#endif /* PACKET_ACKPACKET_H_ */
