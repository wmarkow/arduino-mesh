/*
 * PingPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "PingPacket.h"
#include "core/IotProtocol.h"
#include "core/IotPort.h"

#include <string.h>

PingPacket::PingPacket() :
      IotPacket()
{
   setProtocol(IotProtocol::TCP);
   setType(IotPacketType::REGULAR);
   setPort(IotPort::ICMP);
   setPayloadSize(0);
}
;
