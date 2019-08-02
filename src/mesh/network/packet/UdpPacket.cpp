/*
 * UdpPacket.cpp
 *
 *  Created on: 2 sie 2019
 *      Author: wmarkowski
 */

#include "../../../mesh/network/packet/UdpPacket.h"

#include <string.h>

UdpPacket::UdpPacket(uint8_t* data, uint8_t length) :
        IotPacket()
{
    setProtocol(UDP);

    memcpy(payload, data, length);
}
