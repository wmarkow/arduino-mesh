/*
 * PingPacket.cpp
 *
 *  Created on: 6 maj 2016
 *      Author: witek
 */

#include "../../../mesh/network/packet/PingPacket.h"

#include <string.h>

PingPacket::PingPacket() :
      IotPacket()
{
   setProtocol(ICMP);
   setPayloadSize(0);
}
;
